// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/UMGImplModule.h"

#include "WidgetBlueprint.h"
#include "WidgetBlueprintEditorUtils.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/GridPanel.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/Widget.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompilerModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/PropertyAccessUtil.h"
#include "Editor.h"

namespace UMGImplHelpers
{
	static UWidgetBlueprint* LoadWidgetBlueprint(const FString& BlueprintPath, FString& OutError)
	{
		UObject* Asset = LoadObject<UObject>(nullptr, *BlueprintPath);
		if (!Asset)
		{
			OutError = FString::Printf(TEXT("Asset not found: %s"), *BlueprintPath);
			return nullptr;
		}

		UWidgetBlueprint* WBP = Cast<UWidgetBlueprint>(Asset);
		if (!WBP)
		{
			OutError = FString::Printf(TEXT("Asset is not a Widget Blueprint: %s"), *BlueprintPath);
			return nullptr;
		}

		return WBP;
	}

	static UWidget* FindWidgetByName(UWidgetBlueprint* WBP, const FString& WidgetName, FString& OutError)
	{
		if (!WBP->WidgetTree)
		{
			OutError = TEXT("Widget tree is null");
			return nullptr;
		}

		UWidget* Widget = WBP->WidgetTree->FindWidget(FName(*WidgetName));
		if (!Widget)
		{
			OutError = FString::Printf(TEXT("Widget not found: %s"), *WidgetName);
			return nullptr;
		}

		return Widget;
	}

	static UClass* FindWidgetClass(const FString& ClassName, FString& OutError)
	{
		// Try common short names first
		FString FullPath = FString::Printf(TEXT("/Script/UMG.%s"), *ClassName);
		UClass* WidgetClass = LoadObject<UClass>(nullptr, *FullPath);

		if (!WidgetClass)
		{
			// Try with 'U' prefix
			FullPath = FString::Printf(TEXT("/Script/UMG.U%s"), *ClassName);
			WidgetClass = LoadObject<UClass>(nullptr, *FullPath);
		}

		if (!WidgetClass)
		{
			// Try direct path
			WidgetClass = LoadObject<UClass>(nullptr, *ClassName);
		}

		if (!WidgetClass)
		{
			WidgetClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *ClassName));
		}

		if (!WidgetClass)
		{
			OutError = FString::Printf(TEXT("Widget class not found: %s"), *ClassName);
			return nullptr;
		}

		if (!WidgetClass->IsChildOf(UWidget::StaticClass()))
		{
			OutError = FString::Printf(TEXT("Class is not a UWidget subclass: %s"), *ClassName);
			return nullptr;
		}

		return WidgetClass;
	}

	static void CollectWidgetTree(UWidget* Widget, FWidgetInfo& OutInfo)
	{
		OutInfo.WidgetName = Widget->GetName();
		OutInfo.WidgetClass = Widget->GetClass()->GetName();

		if (Widget->Slot)
		{
			OutInfo.SlotClass = Widget->Slot->GetClass()->GetName();
		}

		UPanelWidget* Panel = Cast<UPanelWidget>(Widget);
		if (Panel)
		{
			for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
			{
				UWidget* Child = Panel->GetChildAt(i);
				if (Child)
				{
					FWidgetInfo ChildInfo;
					ChildInfo.ChildIndex = i;
					ChildInfo.ParentName = Widget->GetName();
					CollectWidgetTree(Child, ChildInfo);
					OutInfo.Children.Add(ChildInfo);
				}
			}
		}
	}

	static void FormatWidgetTree(const FWidgetInfo& Info, FString& OutText, int32 Depth)
	{
		FString Indent;
		for (int32 i = 0; i < Depth; ++i)
		{
			Indent += TEXT("  ");
		}

		FString SlotStr;
		if (!Info.SlotClass.IsEmpty())
		{
			SlotStr = FString::Printf(TEXT(" [slot: %s]"), *Info.SlotClass);
		}

		if (Info.ChildIndex >= 0)
		{
			OutText += FString::Printf(TEXT("%s[%d] %s (%s)%s\n"), *Indent, Info.ChildIndex, *Info.WidgetName, *Info.WidgetClass, *SlotStr);
		}
		else
		{
			OutText += FString::Printf(TEXT("%s%s (%s)%s\n"), *Indent, *Info.WidgetName, *Info.WidgetClass, *SlotStr);
		}

		for (const FWidgetInfo& Child : Info.Children)
		{
			FormatWidgetTree(Child, OutText, Depth + 1);
		}
	}

	static EHorizontalAlignment ParseHAlign(const FString& Value)
	{
		if (Value.Equals(TEXT("Left"), ESearchCase::IgnoreCase)) return EHorizontalAlignment::HAlign_Left;
		if (Value.Equals(TEXT("Center"), ESearchCase::IgnoreCase)) return EHorizontalAlignment::HAlign_Center;
		if (Value.Equals(TEXT("Right"), ESearchCase::IgnoreCase)) return EHorizontalAlignment::HAlign_Right;
		if (Value.Equals(TEXT("Fill"), ESearchCase::IgnoreCase)) return EHorizontalAlignment::HAlign_Fill;
		return EHorizontalAlignment::HAlign_Fill;
	}

	static EVerticalAlignment ParseVAlign(const FString& Value)
	{
		if (Value.Equals(TEXT("Top"), ESearchCase::IgnoreCase)) return EVerticalAlignment::VAlign_Top;
		if (Value.Equals(TEXT("Center"), ESearchCase::IgnoreCase)) return EVerticalAlignment::VAlign_Center;
		if (Value.Equals(TEXT("Bottom"), ESearchCase::IgnoreCase)) return EVerticalAlignment::VAlign_Bottom;
		if (Value.Equals(TEXT("Fill"), ESearchCase::IgnoreCase)) return EVerticalAlignment::VAlign_Fill;
		return EVerticalAlignment::VAlign_Fill;
	}
}

FCreateWidgetBlueprintResult FUMGImplModule::CreateWidgetBlueprint(const FString& BlueprintPath, const FString* RootWidgetClass)
{
	FCreateWidgetBlueprintResult Result;

	FString PackagePath = FPackageName::GetLongPackagePath(BlueprintPath);
	FString AssetName = FPackageName::GetLongPackageAssetName(BlueprintPath);

	if (AssetName.IsEmpty())
	{
		Result.ErrorMessage = TEXT("Invalid Blueprint path: could not extract asset name");
		return Result;
	}

	UClass* ParentClass = UUserWidget::StaticClass();

	UClass* BpClass = nullptr;
	UClass* BpGenClass = nullptr;
	IKismetCompilerInterface& Compiler = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	Compiler.GetBlueprintTypesForClass(ParentClass, BpClass, BpGenClass);

	UPackage* Package = CreatePackage(*BlueprintPath);
	if (!Package)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create package: %s"), *BlueprintPath);
		return Result;
	}

	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		Package,
		FName(*AssetName),
		BPTYPE_Normal,
		BpClass,
		BpGenClass
	);

	UWidgetBlueprint* WBP = Cast<UWidgetBlueprint>(Blueprint);
	if (!WBP)
	{
		Result.ErrorMessage = TEXT("Failed to create Widget Blueprint");
		return Result;
	}

	// Set root widget
	FString RootClassName = TEXT("CanvasPanel");
	if (RootWidgetClass && !RootWidgetClass->IsEmpty())
	{
		RootClassName = *RootWidgetClass;
	}

	FString ClassError;
	UClass* RootClass = UMGImplHelpers::FindWidgetClass(RootClassName, ClassError);
	if (!RootClass)
	{
		// Default to CanvasPanel if class not found
		RootClass = UCanvasPanel::StaticClass();
		RootClassName = TEXT("CanvasPanel");
	}

	if (WBP->WidgetTree && WBP->WidgetTree->RootWidget == nullptr)
	{
		UWidget* RootWidget = WBP->WidgetTree->ConstructWidget<UWidget>(RootClass);
		WBP->WidgetTree->RootWidget = RootWidget;
	}

	FAssetRegistryModule::AssetCreated(WBP);
	WBP->MarkPackageDirty();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.BlueprintName = AssetName;
	Result.BlueprintPath = WBP->GetPathName();
	Result.RootWidgetClass = RootClassName;
	return Result;
}

FGetWidgetTreeResult FUMGImplModule::GetWidgetTree(const FString& BlueprintPath)
{
	FGetWidgetTreeResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP)
	{
		Result.ErrorMessage = Error;
		return Result;
	}

	if (!WBP->WidgetTree || !WBP->WidgetTree->RootWidget)
	{
		Result.ErrorMessage = TEXT("Widget Blueprint has no widget tree or root widget");
		return Result;
	}

	UWidget* Root = WBP->WidgetTree->RootWidget;
	Result.RootWidgetName = Root->GetName();
	Result.RootWidgetClass = Root->GetClass()->GetName();

	// Collect all widgets into tree structure
	TArray<UWidget*> AllWidgets;
	WBP->WidgetTree->GetAllWidgets(AllWidgets);

	FWidgetInfo RootInfo;
	UMGImplHelpers::CollectWidgetTree(Root, RootInfo);
	Result.Widgets.Add(RootInfo);

	Result.bSuccess = true;
	return Result;
}

FAddWidgetResult FUMGImplModule::AddWidget(const FString& BlueprintPath, const FString& WidgetClass, const FString& ParentName, const FString* WidgetName, const int32* InsertIndex)
{
	FAddWidgetResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* ParentWidget = UMGImplHelpers::FindWidgetByName(WBP, ParentName, Error);
	if (!ParentWidget) { Result.ErrorMessage = Error; return Result; }

	UPanelWidget* Panel = Cast<UPanelWidget>(ParentWidget);
	if (!Panel)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Parent widget '%s' is not a panel widget"), *ParentName);
		return Result;
	}

	if (!Panel->CanHaveMultipleChildren() && Panel->GetChildrenCount() > 0)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Parent '%s' already has a child and does not support multiple children"), *ParentName);
		return Result;
	}

	UClass* NewWidgetClass = UMGImplHelpers::FindWidgetClass(WidgetClass, Error);
	if (!NewWidgetClass) { Result.ErrorMessage = Error; return Result; }

	FName NewWidgetFName = NAME_None;
	if (WidgetName && !WidgetName->IsEmpty())
	{
		NewWidgetFName = FName(**WidgetName);
	}

	WBP->Modify();

	UWidget* NewWidget = WBP->WidgetTree->ConstructWidget<UWidget>(NewWidgetClass, NewWidgetFName);
	if (!NewWidget)
	{
		Result.ErrorMessage = TEXT("Failed to construct widget");
		return Result;
	}

	UPanelSlot* Slot = nullptr;
	if (InsertIndex && *InsertIndex >= 0)
	{
		Slot = Panel->InsertChildAt(*InsertIndex, NewWidget);
	}
	else
	{
		Slot = Panel->AddChild(NewWidget);
	}

	if (!Slot)
	{
		Result.ErrorMessage = TEXT("Failed to add widget to parent panel");
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.WidgetName = NewWidget->GetName();
	Result.WidgetClass = NewWidget->GetClass()->GetName();
	Result.ParentName = ParentName;
	return Result;
}

FRemoveWidgetResult FUMGImplModule::RemoveWidget(const FString& BlueprintPath, const FString& WidgetName)
{
	FRemoveWidgetResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	if (Widget == WBP->WidgetTree->RootWidget)
	{
		Result.ErrorMessage = TEXT("Cannot remove the root widget");
		return Result;
	}

	WBP->Modify();

	TSet<UWidget*> WidgetsToDelete;
	WidgetsToDelete.Add(Widget);
	FWidgetBlueprintEditorUtils::DeleteWidgets(WBP, WidgetsToDelete, FWidgetBlueprintEditorUtils::EDeleteWidgetWarningType::DeleteSilently);

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	return Result;
}

FMoveWidgetResult FUMGImplModule::MoveWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewParentName, const int32* InsertIndex)
{
	FMoveWidgetResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	UWidget* NewParent = UMGImplHelpers::FindWidgetByName(WBP, NewParentName, Error);
	if (!NewParent) { Result.ErrorMessage = Error; return Result; }

	UPanelWidget* NewPanel = Cast<UPanelWidget>(NewParent);
	if (!NewPanel)
	{
		Result.ErrorMessage = FString::Printf(TEXT("New parent '%s' is not a panel widget"), *NewParentName);
		return Result;
	}

	// Check for circular move (moving widget into its own descendant)
	bool bIsDescendant = false;
	UPanelWidget* WidgetAsPanel = Cast<UPanelWidget>(Widget);
	if (WidgetAsPanel)
	{
		TArray<UWidget*> Descendants;
		UWidgetTree::GetChildWidgets(Widget, Descendants);
		for (UWidget* Desc : Descendants)
		{
			if (Desc == NewParent)
			{
				bIsDescendant = true;
				break;
			}
		}
	}

	if (bIsDescendant)
	{
		Result.ErrorMessage = TEXT("Cannot move widget into its own descendant");
		return Result;
	}

	WBP->Modify();

	// Remove from current parent
	UPanelWidget* OldParent = Widget->GetParent();
	if (OldParent)
	{
		OldParent->RemoveChild(Widget);
	}

	// Add to new parent
	if (InsertIndex && *InsertIndex >= 0)
	{
		NewPanel->InsertChildAt(*InsertIndex, Widget);
	}
	else
	{
		NewPanel->AddChild(Widget);
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.WidgetName = WidgetName;
	Result.NewParentName = NewParentName;
	return Result;
}

FRenameWidgetResult FUMGImplModule::RenameWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewName)
{
	FRenameWidgetResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	// Check name uniqueness
	UWidget* Existing = WBP->WidgetTree->FindWidget(FName(*NewName));
	if (Existing && Existing != Widget)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Name '%s' is already in use"), *NewName);
		return Result;
	}

	FName OldName = Widget->GetFName();

	WBP->Modify();
	Widget->Modify();

	Widget->SetDisplayLabel(NewName);
	Widget->Rename(*NewName);

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.OldName = OldName.ToString();
	Result.NewName = NewName;
	return Result;
}

FReplaceWidgetResult FUMGImplModule::ReplaceWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewWidgetClass)
{
	FReplaceWidgetResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	UClass* NewClass = UMGImplHelpers::FindWidgetClass(NewWidgetClass, Error);
	if (!NewClass) { Result.ErrorMessage = Error; return Result; }

	FString OldClassName = Widget->GetClass()->GetName();

	WBP->Modify();

	TSet<UWidget*> WidgetsToReplace;
	WidgetsToReplace.Add(Widget);
	FWidgetBlueprintEditorUtils::ReplaceWidgets(WBP, WidgetsToReplace, NewClass, FWidgetBlueprintEditorUtils::EReplaceWidgetNamingMethod::MaintainNameAndReferences);

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.WidgetName = WidgetName;
	Result.OldClass = OldClassName;
	Result.NewClass = NewClass->GetName();
	return Result;
}

FSetWidgetPropertyResult FUMGImplModule::SetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName, const FString& PropertyValue)
{
	FSetWidgetPropertyResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	FProperty* Property = Widget->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Property '%s' not found on widget '%s' (%s)"), *PropertyName, *WidgetName, *Widget->GetClass()->GetName());
		return Result;
	}

	WBP->Modify();
	Widget->Modify();

	void* PropertyAddr = Property->ContainerPtrToValuePtr<void>(Widget);
	if (!Property->ImportText_Direct(*PropertyValue, PropertyAddr, Widget, PPF_None))
	{
		Result.ErrorMessage = FString::Printf(TEXT("Failed to set property '%s' to value '%s'"), *PropertyName, *PropertyValue);
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	return Result;
}

FGetWidgetPropertyResult FUMGImplModule::GetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName)
{
	FGetWidgetPropertyResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	FProperty* Property = Widget->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Property '%s' not found on widget '%s' (%s)"), *PropertyName, *WidgetName, *Widget->GetClass()->GetName());
		return Result;
	}

	FString ValueStr;
	const void* PropertyAddr = Property->ContainerPtrToValuePtr<void>(Widget);
	Property->ExportTextItem_Direct(ValueStr, PropertyAddr, nullptr, Widget, PPF_None);

	Result.bSuccess = true;
	Result.PropertyValue = ValueStr;
	return Result;
}

FSetWidgetSlotResult FUMGImplModule::SetWidgetSlot(const FString& BlueprintPath, const FString& WidgetName, const TMap<FString, FString>& SlotProperties)
{
	FSetWidgetSlotResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, WidgetName, Error);
	if (!Widget) { Result.ErrorMessage = Error; return Result; }

	UPanelSlot* Slot = Widget->Slot;
	if (!Slot)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Widget '%s' has no slot (not parented)"), *WidgetName);
		return Result;
	}

	WBP->Modify();
	Slot->Modify();

	// CanvasPanelSlot
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		FAnchorData Layout = CanvasSlot->GetLayout();

		if (const FString* Val = SlotProperties.Find(TEXT("position_x")))     { Layout.Offsets.Left = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("position_y")))     { Layout.Offsets.Top = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("size_x")))         { Layout.Offsets.Right = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("size_y")))         { Layout.Offsets.Bottom = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("anchor_min_x")))   { Layout.Anchors.Minimum.X = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("anchor_min_y")))   { Layout.Anchors.Minimum.Y = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("anchor_max_x")))   { Layout.Anchors.Maximum.X = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("anchor_max_y")))   { Layout.Anchors.Maximum.Y = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("alignment_x")))    { Layout.Alignment.X = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("alignment_y")))    { Layout.Alignment.Y = FCString::Atof(**Val); }

		CanvasSlot->SetLayout(Layout);

		if (const FString* Val = SlotProperties.Find(TEXT("auto_size")))
		{
			CanvasSlot->SetAutoSize(Val->Equals(TEXT("true"), ESearchCase::IgnoreCase));
		}
		if (const FString* Val = SlotProperties.Find(TEXT("z_order")))
		{
			CanvasSlot->SetZOrder(FCString::Atoi(**Val));
		}
	}
	// HorizontalBoxSlot
	else if (UHorizontalBoxSlot* HBoxSlot = Cast<UHorizontalBoxSlot>(Slot))
	{
		FMargin Padding = HBoxSlot->GetPadding();
		if (const FString* Val = SlotProperties.Find(TEXT("padding_left")))   { Padding.Left = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_top")))    { Padding.Top = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_right")))  { Padding.Right = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_bottom"))) { Padding.Bottom = FCString::Atof(**Val); }
		HBoxSlot->SetPadding(Padding);

		if (const FString* Val = SlotProperties.Find(TEXT("size_rule")))
		{
			FSlateChildSize ChildSize;
			if (Val->Equals(TEXT("Auto"), ESearchCase::IgnoreCase))
			{
				ChildSize.SizeRule = ESlateSizeRule::Automatic;
			}
			else
			{
				ChildSize.SizeRule = ESlateSizeRule::Fill;
				if (const FString* FillVal = SlotProperties.Find(TEXT("fill_weight")))
				{
					ChildSize.Value = FCString::Atof(**FillVal);
				}
			}
			HBoxSlot->SetSize(ChildSize);
		}

		if (const FString* Val = SlotProperties.Find(TEXT("h_align"))) { HBoxSlot->SetHorizontalAlignment(UMGImplHelpers::ParseHAlign(*Val)); }
		if (const FString* Val = SlotProperties.Find(TEXT("v_align"))) { HBoxSlot->SetVerticalAlignment(UMGImplHelpers::ParseVAlign(*Val)); }
	}
	// VerticalBoxSlot
	else if (UVerticalBoxSlot* VBoxSlot = Cast<UVerticalBoxSlot>(Slot))
	{
		FMargin Padding = VBoxSlot->GetPadding();
		if (const FString* Val = SlotProperties.Find(TEXT("padding_left")))   { Padding.Left = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_top")))    { Padding.Top = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_right")))  { Padding.Right = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_bottom"))) { Padding.Bottom = FCString::Atof(**Val); }
		VBoxSlot->SetPadding(Padding);

		if (const FString* Val = SlotProperties.Find(TEXT("size_rule")))
		{
			FSlateChildSize ChildSize;
			if (Val->Equals(TEXT("Auto"), ESearchCase::IgnoreCase))
			{
				ChildSize.SizeRule = ESlateSizeRule::Automatic;
			}
			else
			{
				ChildSize.SizeRule = ESlateSizeRule::Fill;
				if (const FString* FillVal = SlotProperties.Find(TEXT("fill_weight")))
				{
					ChildSize.Value = FCString::Atof(**FillVal);
				}
			}
			VBoxSlot->SetSize(ChildSize);
		}

		if (const FString* Val = SlotProperties.Find(TEXT("h_align"))) { VBoxSlot->SetHorizontalAlignment(UMGImplHelpers::ParseHAlign(*Val)); }
		if (const FString* Val = SlotProperties.Find(TEXT("v_align"))) { VBoxSlot->SetVerticalAlignment(UMGImplHelpers::ParseVAlign(*Val)); }
	}
	// OverlaySlot
	else if (UOverlaySlot* OvSlot = Cast<UOverlaySlot>(Slot))
	{
		FMargin Padding = OvSlot->GetPadding();
		if (const FString* Val = SlotProperties.Find(TEXT("padding_left")))   { Padding.Left = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_top")))    { Padding.Top = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_right")))  { Padding.Right = FCString::Atof(**Val); }
		if (const FString* Val = SlotProperties.Find(TEXT("padding_bottom"))) { Padding.Bottom = FCString::Atof(**Val); }
		OvSlot->SetPadding(Padding);

		if (const FString* Val = SlotProperties.Find(TEXT("h_align"))) { OvSlot->SetHorizontalAlignment(UMGImplHelpers::ParseHAlign(*Val)); }
		if (const FString* Val = SlotProperties.Find(TEXT("v_align"))) { OvSlot->SetVerticalAlignment(UMGImplHelpers::ParseVAlign(*Val)); }
	}
	else
	{
		// For unsupported slot types, try property reflection as fallback
		for (const auto& Pair : SlotProperties)
		{
			FProperty* Property = Slot->GetClass()->FindPropertyByName(FName(*Pair.Key));
			if (Property)
			{
				void* PropertyAddr = Property->ContainerPtrToValuePtr<void>(Slot);
				Property->ImportText_Direct(*Pair.Value, PropertyAddr, Slot, PPF_None);
			}
		}
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	return Result;
}

FGetWidgetAnimationsResult FUMGImplModule::GetWidgetAnimations(const FString& BlueprintPath)
{
	FGetWidgetAnimationsResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	for (UWidgetAnimation* Anim : WBP->Animations)
	{
		if (Anim)
		{
			FWidgetAnimationInfo Info;
			Info.AnimationName = Anim->GetDisplayLabel().IsEmpty() ? Anim->GetName() : Anim->GetDisplayLabel();
			Info.StartTime = Anim->GetStartTime();
			Info.EndTime = Anim->GetEndTime();
			Result.Animations.Add(Info);
		}
	}

	Result.bSuccess = true;
	return Result;
}

FExportWidgetsResult FUMGImplModule::ExportWidgets(const FString& BlueprintPath, const TArray<FString>* WidgetNames)
{
	FExportWidgetsResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	TArray<UWidget*> WidgetsToExport;

	if (WidgetNames && WidgetNames->Num() > 0)
	{
		for (const FString& Name : *WidgetNames)
		{
			UWidget* Widget = UMGImplHelpers::FindWidgetByName(WBP, Name, Error);
			if (!Widget)
			{
				Result.ErrorMessage = Error;
				return Result;
			}
			WidgetsToExport.Add(Widget);
		}
	}
	else
	{
		WBP->WidgetTree->GetAllWidgets(WidgetsToExport);
	}

	FString ExportedText;
	FWidgetBlueprintEditorUtils::ExportWidgetsToText(WidgetsToExport, ExportedText);

	Result.bSuccess = true;
	Result.ExportedText = ExportedText;
	return Result;
}

FImportWidgetsResult FUMGImplModule::ImportWidgets(const FString& BlueprintPath, const FString& ExportedText, const FString* ParentName)
{
	FImportWidgetsResult Result;

	FString Error;
	UWidgetBlueprint* WBP = UMGImplHelpers::LoadWidgetBlueprint(BlueprintPath, Error);
	if (!WBP) { Result.ErrorMessage = Error; return Result; }

	WBP->Modify();

	TSet<UWidget*> ImportedWidgets;
	TMap<FName, UWidgetSlotPair*> PastedExtraSlotData;

	FWidgetBlueprintEditorUtils::ImportWidgetsFromText(WBP, ExportedText, ImportedWidgets, PastedExtraSlotData);

	if (ImportedWidgets.Num() == 0)
	{
		Result.ErrorMessage = TEXT("No widgets were imported (invalid exported text?)");
		return Result;
	}

	// If parent specified, reparent imported widgets
	if (ParentName && !ParentName->IsEmpty())
	{
		UWidget* Parent = UMGImplHelpers::FindWidgetByName(WBP, *ParentName, Error);
		if (Parent)
		{
			UPanelWidget* ParentPanel = Cast<UPanelWidget>(Parent);
			if (ParentPanel)
			{
				for (UWidget* Imported : ImportedWidgets)
				{
					if (Imported->GetParent() == nullptr)
					{
						ParentPanel->AddChild(Imported);
					}
				}
			}
		}
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(WBP);

	Result.bSuccess = true;
	Result.WidgetsImported = ImportedWidgets.Num();
	return Result;
}
