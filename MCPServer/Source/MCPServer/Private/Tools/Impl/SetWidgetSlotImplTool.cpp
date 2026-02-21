// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetWidgetSlotImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FSetWidgetSlotImplTool::FSetWidgetSlotImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FSetWidgetSlotImplTool::GetName() const
{
	return TEXT("set_widget_slot");
}

FString FSetWidgetSlotImplTool::GetDescription() const
{
	return TEXT("Set layout slot properties on a widget (anchors, padding, alignment, size)");
}

TSharedPtr<FJsonObject> FSetWidgetSlotImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> WidgetNameProp = MakeShared<FJsonObject>();
	WidgetNameProp->SetStringField(TEXT("type"), TEXT("string"));
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Name of the target widget"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> SlotPropertiesProp = MakeShared<FJsonObject>();
	SlotPropertiesProp->SetStringField(TEXT("type"), TEXT("object"));
	SlotPropertiesProp->SetStringField(TEXT("description"), TEXT("Slot property key-value pairs. Keys depend on parent type. CanvasPanelSlot: position_x, position_y, size_x, size_y, anchor_min_x, anchor_min_y, anchor_max_x, anchor_max_y, alignment_x, alignment_y, auto_size, z_order. BoxSlot: padding_left, padding_top, padding_right, padding_bottom, size_rule (Auto/Fill), fill_weight, h_align (Left/Center/Right/Fill), v_align (Top/Center/Bottom/Fill). OverlaySlot: padding_*, h_align, v_align"));
	TSharedPtr<FJsonObject> AdditionalProps = MakeShared<FJsonObject>();
	AdditionalProps->SetStringField(TEXT("type"), TEXT("string"));
	SlotPropertiesProp->SetObjectField(TEXT("additionalProperties"), AdditionalProps);
	Properties->SetObjectField(TEXT("slot_properties"), SlotPropertiesProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("slot_properties")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetWidgetSlotImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("set_widget_slot is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
