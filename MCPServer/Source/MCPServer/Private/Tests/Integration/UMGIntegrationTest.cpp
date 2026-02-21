// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockUMGModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/CreateWidgetBlueprintImplTool.h"
#include "Tools/Impl/GetWidgetTreeImplTool.h"
#include "Tools/Impl/AddWidgetImplTool.h"
#include "Tools/Impl/RemoveWidgetImplTool.h"
#include "Tools/Impl/MoveWidgetImplTool.h"
#include "Tools/Impl/RenameWidgetImplTool.h"
#include "Tools/Impl/ReplaceWidgetImplTool.h"
#include "Tools/Impl/SetWidgetPropertyImplTool.h"
#include "Tools/Impl/GetWidgetPropertyImplTool.h"
#include "Tools/Impl/SetWidgetSlotImplTool.h"
#include "Tools/Impl/GetWidgetAnimationsImplTool.h"
#include "Tools/Impl/ExportWidgetsImplTool.h"
#include "Tools/Impl/ImportWidgetsImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUMGIntegrationWidgetLifecycleTest,
	"MCPServer.Integration.UMG.WidgetLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FUMGIntegrationWidgetLifecycleTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;

	// Step 1: Create a Widget Blueprint
	Mock.CreateWidgetBlueprintResult.bSuccess = true;
	Mock.CreateWidgetBlueprintResult.BlueprintName = TEXT("WBP_HUD");
	Mock.CreateWidgetBlueprintResult.BlueprintPath = TEXT("/Game/WBP_HUD");
	Mock.CreateWidgetBlueprintResult.RootWidgetClass = TEXT("CanvasPanel");

	FCreateWidgetBlueprintImplTool CreateTool(Mock);
	auto CreateArgs = MakeShared<FJsonObject>();
	CreateArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	CreateArgs->SetStringField(TEXT("root_widget_class"), TEXT("CanvasPanel"));
	TestTrue(TEXT("Create WBP success"), MCPTestUtils::IsSuccess(CreateTool.Execute(CreateArgs)));

	// Step 2: Get widget tree
	Mock.GetWidgetTreeResult.bSuccess = true;
	Mock.GetWidgetTreeResult.BlueprintName = TEXT("WBP_HUD");
	FWidgetInfo Root;
	Root.WidgetName = TEXT("RootCanvas");
	Root.WidgetClass = TEXT("CanvasPanel");
	Mock.GetWidgetTreeResult.RootWidget = Root;
	Mock.GetWidgetTreeResult.TotalWidgets = 1;

	FGetWidgetTreeImplTool TreeTool(Mock);
	auto TreeArgs = MakeShared<FJsonObject>();
	TreeArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	TestTrue(TEXT("Get tree success"), MCPTestUtils::IsSuccess(TreeTool.Execute(TreeArgs)));

	// Step 3: Add a VerticalBox to the canvas
	Mock.AddWidgetResult.bSuccess = true;
	Mock.AddWidgetResult.WidgetName = TEXT("VBox_Main");
	Mock.AddWidgetResult.WidgetClass = TEXT("VerticalBox");
	Mock.AddWidgetResult.ParentName = TEXT("RootCanvas");

	FAddWidgetImplTool AddTool(Mock);
	auto AddArgs = MakeShared<FJsonObject>();
	AddArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	AddArgs->SetStringField(TEXT("widget_class"), TEXT("VerticalBox"));
	AddArgs->SetStringField(TEXT("parent_name"), TEXT("RootCanvas"));
	AddArgs->SetStringField(TEXT("widget_name"), TEXT("VBox_Main"));
	TestTrue(TEXT("Add VBox success"), MCPTestUtils::IsSuccess(AddTool.Execute(AddArgs)));

	// Step 4: Add a TextBlock to the VerticalBox
	Mock.AddWidgetResult.WidgetName = TEXT("TitleText");
	Mock.AddWidgetResult.WidgetClass = TEXT("TextBlock");
	Mock.AddWidgetResult.ParentName = TEXT("VBox_Main");

	auto AddTextArgs = MakeShared<FJsonObject>();
	AddTextArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	AddTextArgs->SetStringField(TEXT("widget_class"), TEXT("TextBlock"));
	AddTextArgs->SetStringField(TEXT("parent_name"), TEXT("VBox_Main"));
	AddTextArgs->SetStringField(TEXT("widget_name"), TEXT("TitleText"));
	TestTrue(TEXT("Add TextBlock success"), MCPTestUtils::IsSuccess(AddTool.Execute(AddTextArgs)));

	// Step 5: Set a property on the TextBlock
	Mock.SetWidgetPropertyResult.bSuccess = true;

	FSetWidgetPropertyImplTool SetPropTool(Mock);
	auto SetPropArgs = MakeShared<FJsonObject>();
	SetPropArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	SetPropArgs->SetStringField(TEXT("widget_name"), TEXT("TitleText"));
	SetPropArgs->SetStringField(TEXT("property_name"), TEXT("Text"));
	SetPropArgs->SetStringField(TEXT("property_value"), TEXT("Hello HUD"));
	TestTrue(TEXT("Set property success"), MCPTestUtils::IsSuccess(SetPropTool.Execute(SetPropArgs)));

	// Step 6: Get the property back
	Mock.GetWidgetPropertyResult.bSuccess = true;
	Mock.GetWidgetPropertyResult.PropertyValue = TEXT("Hello HUD");

	FGetWidgetPropertyImplTool GetPropTool(Mock);
	auto GetPropArgs = MakeShared<FJsonObject>();
	GetPropArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	GetPropArgs->SetStringField(TEXT("widget_name"), TEXT("TitleText"));
	GetPropArgs->SetStringField(TEXT("property_name"), TEXT("Text"));
	auto GetPropResult = GetPropTool.Execute(GetPropArgs);
	TestTrue(TEXT("Get property success"), MCPTestUtils::IsSuccess(GetPropResult));
	TestTrue(TEXT("Property value"), MCPTestUtils::GetResultText(GetPropResult).Contains(TEXT("Hello HUD")));

	// Step 7: Set widget slot
	Mock.SetWidgetSlotResult.bSuccess = true;

	FSetWidgetSlotImplTool SlotTool(Mock);
	auto SlotArgs = MakeShared<FJsonObject>();
	SlotArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	SlotArgs->SetStringField(TEXT("widget_name"), TEXT("TitleText"));
	auto SlotProps = MakeShared<FJsonObject>();
	SlotProps->SetStringField(TEXT("HorizontalAlignment"), TEXT("Center"));
	SlotArgs->SetObjectField(TEXT("slot_properties"), SlotProps);
	TestTrue(TEXT("Set slot success"), MCPTestUtils::IsSuccess(SlotTool.Execute(SlotArgs)));

	// Step 8: Rename the widget
	Mock.RenameWidgetResult.bSuccess = true;
	Mock.RenameWidgetResult.OldName = TEXT("TitleText");
	Mock.RenameWidgetResult.NewName = TEXT("HeaderLabel");

	FRenameWidgetImplTool RenameTool(Mock);
	auto RenameArgs = MakeShared<FJsonObject>();
	RenameArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	RenameArgs->SetStringField(TEXT("widget_name"), TEXT("TitleText"));
	RenameArgs->SetStringField(TEXT("new_name"), TEXT("HeaderLabel"));
	TestTrue(TEXT("Rename success"), MCPTestUtils::IsSuccess(RenameTool.Execute(RenameArgs)));

	// Step 9: Replace the TextBlock with a RichTextBlock
	Mock.ReplaceWidgetResult.bSuccess = true;
	Mock.ReplaceWidgetResult.WidgetName = TEXT("HeaderLabel");
	Mock.ReplaceWidgetResult.OldClass = TEXT("TextBlock");
	Mock.ReplaceWidgetResult.NewClass = TEXT("RichTextBlock");

	FReplaceWidgetImplTool ReplaceTool(Mock);
	auto ReplaceArgs = MakeShared<FJsonObject>();
	ReplaceArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	ReplaceArgs->SetStringField(TEXT("widget_name"), TEXT("HeaderLabel"));
	ReplaceArgs->SetStringField(TEXT("new_widget_class"), TEXT("RichTextBlock"));
	TestTrue(TEXT("Replace success"), MCPTestUtils::IsSuccess(ReplaceTool.Execute(ReplaceArgs)));

	// Step 10: Export widgets
	Mock.ExportWidgetsResult.bSuccess = true;
	Mock.ExportWidgetsResult.ExportedText = TEXT("Begin Object Class=RichTextBlock...");

	FExportWidgetsImplTool ExportTool(Mock);
	auto ExportArgs = MakeShared<FJsonObject>();
	ExportArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	auto ExportResult = ExportTool.Execute(ExportArgs);
	TestTrue(TEXT("Export success"), MCPTestUtils::IsSuccess(ExportResult));
	TestTrue(TEXT("Exported text"), MCPTestUtils::GetResultText(ExportResult).Contains(TEXT("Begin Object")));

	// Step 11: Remove the widget
	Mock.RemoveWidgetResult.bSuccess = true;

	FRemoveWidgetImplTool RemoveTool(Mock);
	auto RemoveArgs = MakeShared<FJsonObject>();
	RemoveArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	RemoveArgs->SetStringField(TEXT("widget_name"), TEXT("HeaderLabel"));
	TestTrue(TEXT("Remove success"), MCPTestUtils::IsSuccess(RemoveTool.Execute(RemoveArgs)));

	// Step 12: Import widgets back
	Mock.ImportWidgetsResult.bSuccess = true;
	Mock.ImportWidgetsResult.WidgetsImported = 1;

	FImportWidgetsImplTool ImportTool(Mock);
	auto ImportArgs = MakeShared<FJsonObject>();
	ImportArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	ImportArgs->SetStringField(TEXT("exported_text"), TEXT("Begin Object Class=RichTextBlock..."));
	ImportArgs->SetStringField(TEXT("parent_name"), TEXT("VBox_Main"));
	TestTrue(TEXT("Import success"), MCPTestUtils::IsSuccess(ImportTool.Execute(ImportArgs)));

	// Step 13: Get animations
	Mock.GetWidgetAnimationsResult.bSuccess = true;
	FWidgetAnimationInfo Anim;
	Anim.AnimationName = TEXT("FadeIn");
	Anim.StartTime = 0.0f;
	Anim.EndTime = 1.5f;
	Mock.GetWidgetAnimationsResult.Animations.Add(Anim);

	FGetWidgetAnimationsImplTool AnimTool(Mock);
	auto AnimArgs = MakeShared<FJsonObject>();
	AnimArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_HUD"));
	TestTrue(TEXT("Get animations success"), MCPTestUtils::IsSuccess(AnimTool.Execute(AnimArgs)));

	// Verify call counts
	TestEqual(TEXT("CreateWidgetBlueprint calls"), Mock.Recorder.GetCallCount(TEXT("CreateWidgetBlueprint")), 1);
	TestEqual(TEXT("GetWidgetTree calls"), Mock.Recorder.GetCallCount(TEXT("GetWidgetTree")), 1);
	TestEqual(TEXT("AddWidget calls"), Mock.Recorder.GetCallCount(TEXT("AddWidget")), 2);
	TestEqual(TEXT("SetWidgetProperty calls"), Mock.Recorder.GetCallCount(TEXT("SetWidgetProperty")), 1);
	TestEqual(TEXT("GetWidgetProperty calls"), Mock.Recorder.GetCallCount(TEXT("GetWidgetProperty")), 1);
	TestEqual(TEXT("SetWidgetSlot calls"), Mock.Recorder.GetCallCount(TEXT("SetWidgetSlot")), 1);
	TestEqual(TEXT("RenameWidget calls"), Mock.Recorder.GetCallCount(TEXT("RenameWidget")), 1);
	TestEqual(TEXT("ReplaceWidget calls"), Mock.Recorder.GetCallCount(TEXT("ReplaceWidget")), 1);
	TestEqual(TEXT("ExportWidgets calls"), Mock.Recorder.GetCallCount(TEXT("ExportWidgets")), 1);
	TestEqual(TEXT("RemoveWidget calls"), Mock.Recorder.GetCallCount(TEXT("RemoveWidget")), 1);
	TestEqual(TEXT("ImportWidgets calls"), Mock.Recorder.GetCallCount(TEXT("ImportWidgets")), 1);
	TestEqual(TEXT("GetWidgetAnimations calls"), Mock.Recorder.GetCallCount(TEXT("GetWidgetAnimations")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 13);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUMGIntegrationMoveWidgetTest,
	"MCPServer.Integration.UMG.MoveWidgetBetweenPanels",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FUMGIntegrationMoveWidgetTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;

	// Step 1: Add a widget
	Mock.AddWidgetResult.bSuccess = true;
	Mock.AddWidgetResult.WidgetName = TEXT("Button_0");
	Mock.AddWidgetResult.WidgetClass = TEXT("Button");
	Mock.AddWidgetResult.ParentName = TEXT("HBox_Top");

	FAddWidgetImplTool AddTool(Mock);
	auto AddArgs = MakeShared<FJsonObject>();
	AddArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	AddArgs->SetStringField(TEXT("widget_class"), TEXT("Button"));
	AddArgs->SetStringField(TEXT("parent_name"), TEXT("HBox_Top"));
	TestTrue(TEXT("Add widget"), MCPTestUtils::IsSuccess(AddTool.Execute(AddArgs)));

	// Step 2: Move it to another panel
	Mock.MoveWidgetResult.bSuccess = true;

	FMoveWidgetImplTool MoveTool(Mock);
	auto MoveArgs = MakeShared<FJsonObject>();
	MoveArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	MoveArgs->SetStringField(TEXT("widget_name"), TEXT("Button_0"));
	MoveArgs->SetStringField(TEXT("new_parent_name"), TEXT("VBox_Bottom"));
	MoveArgs->SetNumberField(TEXT("insert_index"), 0);
	TestTrue(TEXT("Move widget"), MCPTestUtils::IsSuccess(MoveTool.Execute(MoveArgs)));

	// Step 3: Verify tree after move
	Mock.GetWidgetTreeResult.bSuccess = true;
	Mock.GetWidgetTreeResult.BlueprintName = TEXT("WBP_Test");
	Mock.GetWidgetTreeResult.TotalWidgets = 5;

	FGetWidgetTreeImplTool TreeTool(Mock);
	auto TreeArgs = MakeShared<FJsonObject>();
	TreeArgs->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	TestTrue(TEXT("Get tree after move"), MCPTestUtils::IsSuccess(TreeTool.Execute(TreeArgs)));

	TestEqual(TEXT("AddWidget calls"), Mock.Recorder.GetCallCount(TEXT("AddWidget")), 1);
	TestEqual(TEXT("MoveWidget calls"), Mock.Recorder.GetCallCount(TEXT("MoveWidget")), 1);
	TestEqual(TEXT("GetWidgetTree calls"), Mock.Recorder.GetCallCount(TEXT("GetWidgetTree")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 3);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
