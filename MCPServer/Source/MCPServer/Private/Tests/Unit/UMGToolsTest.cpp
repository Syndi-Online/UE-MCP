// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
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
#include "Tests/Mocks/MockUMGModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// CreateWidgetBlueprint
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateWidgetBlueprintMetadataTest,
	"MCPServer.Unit.UMG.CreateWidgetBlueprint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateWidgetBlueprintMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FCreateWidgetBlueprintImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_widget_blueprint"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateWidgetBlueprintSuccessTest,
	"MCPServer.Unit.UMG.CreateWidgetBlueprint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateWidgetBlueprintSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.CreateWidgetBlueprintResult.bSuccess = true;
	Mock.CreateWidgetBlueprintResult.BlueprintName = TEXT("WBP_Test");
	Mock.CreateWidgetBlueprintResult.BlueprintPath = TEXT("/Game/WBP_Test");
	Mock.CreateWidgetBlueprintResult.RootWidgetClass = TEXT("CanvasPanel");

	FCreateWidgetBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("WBP_Test")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("CreateWidgetBlueprint")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateWidgetBlueprintMissingArgsTest,
	"MCPServer.Unit.UMG.CreateWidgetBlueprint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateWidgetBlueprintMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FCreateWidgetBlueprintImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("CreateWidgetBlueprint")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateWidgetBlueprintModuleFailureTest,
	"MCPServer.Unit.UMG.CreateWidgetBlueprint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateWidgetBlueprintModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.CreateWidgetBlueprintResult.bSuccess = false;
	Mock.CreateWidgetBlueprintResult.ErrorMessage = TEXT("Failed to create widget blueprint");

	FCreateWidgetBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Failed to create widget blueprint")));
	return true;
}

// ============================================================================
// GetWidgetTree
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetTreeMetadataTest,
	"MCPServer.Unit.UMG.GetWidgetTree.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetTreeMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetTreeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_widget_tree"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetTreeSuccessTest,
	"MCPServer.Unit.UMG.GetWidgetTree.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetTreeSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetTreeResult.bSuccess = true;
	Mock.GetWidgetTreeResult.BlueprintName = TEXT("WBP_Test");
	FWidgetInfo Root;
	Root.WidgetName = TEXT("RootCanvas");
	Root.WidgetClass = TEXT("CanvasPanel");
	Mock.GetWidgetTreeResult.RootWidget = Root;
	Mock.GetWidgetTreeResult.TotalWidgets = 1;

	FGetWidgetTreeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetTree")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetTreeMissingArgsTest,
	"MCPServer.Unit.UMG.GetWidgetTree.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetTreeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetTreeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetTree")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetTreeModuleFailureTest,
	"MCPServer.Unit.UMG.GetWidgetTree.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetTreeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetTreeResult.bSuccess = false;
	Mock.GetWidgetTreeResult.ErrorMessage = TEXT("Blueprint not found");

	FGetWidgetTreeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Blueprint not found")));
	return true;
}

// ============================================================================
// AddWidget
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddWidgetMetadataTest,
	"MCPServer.Unit.UMG.AddWidget.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddWidgetMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FAddWidgetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_widget"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddWidgetSuccessTest,
	"MCPServer.Unit.UMG.AddWidget.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddWidgetSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.AddWidgetResult.bSuccess = true;
	Mock.AddWidgetResult.WidgetName = TEXT("TextBlock_0");
	Mock.AddWidgetResult.WidgetClass = TEXT("TextBlock");
	Mock.AddWidgetResult.ParentName = TEXT("RootCanvas");

	FAddWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_class"), TEXT("TextBlock"));
	Args->SetStringField(TEXT("parent_name"), TEXT("RootCanvas"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains widget name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TextBlock_0")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("AddWidget")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddWidgetMissingArgsTest,
	"MCPServer.Unit.UMG.AddWidget.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddWidgetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FAddWidgetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("AddWidget")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddWidgetModuleFailureTest,
	"MCPServer.Unit.UMG.AddWidget.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddWidgetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.AddWidgetResult.bSuccess = false;
	Mock.AddWidgetResult.ErrorMessage = TEXT("Parent is not a panel widget");

	FAddWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_class"), TEXT("TextBlock"));
	Args->SetStringField(TEXT("parent_name"), TEXT("TextBlock_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Parent is not a panel widget")));
	return true;
}

// ============================================================================
// RemoveWidget
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveWidgetMetadataTest,
	"MCPServer.Unit.UMG.RemoveWidget.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveWidgetMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FRemoveWidgetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_widget"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveWidgetSuccessTest,
	"MCPServer.Unit.UMG.RemoveWidget.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveWidgetSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.RemoveWidgetResult.bSuccess = true;

	FRemoveWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RemoveWidget")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveWidgetMissingArgsTest,
	"MCPServer.Unit.UMG.RemoveWidget.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveWidgetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FRemoveWidgetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("RemoveWidget")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveWidgetModuleFailureTest,
	"MCPServer.Unit.UMG.RemoveWidget.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveWidgetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.RemoveWidgetResult.bSuccess = false;
	Mock.RemoveWidgetResult.ErrorMessage = TEXT("Widget not found");

	FRemoveWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Widget not found")));
	return true;
}

// ============================================================================
// MoveWidget
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveWidgetMetadataTest,
	"MCPServer.Unit.UMG.MoveWidget.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveWidgetMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FMoveWidgetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("move_widget"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveWidgetSuccessTest,
	"MCPServer.Unit.UMG.MoveWidget.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveWidgetSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.MoveWidgetResult.bSuccess = true;

	FMoveWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("new_parent_name"), TEXT("VerticalBox_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("MoveWidget")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveWidgetMissingArgsTest,
	"MCPServer.Unit.UMG.MoveWidget.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveWidgetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FMoveWidgetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("MoveWidget")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveWidgetModuleFailureTest,
	"MCPServer.Unit.UMG.MoveWidget.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveWidgetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.MoveWidgetResult.bSuccess = false;
	Mock.MoveWidgetResult.ErrorMessage = TEXT("Circular reference detected");

	FMoveWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("Canvas_0"));
	Args->SetStringField(TEXT("new_parent_name"), TEXT("Canvas_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Circular reference detected")));
	return true;
}

// ============================================================================
// RenameWidget
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameWidgetMetadataTest,
	"MCPServer.Unit.UMG.RenameWidget.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameWidgetMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FRenameWidgetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("rename_widget"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameWidgetSuccessTest,
	"MCPServer.Unit.UMG.RenameWidget.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameWidgetSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.RenameWidgetResult.bSuccess = true;
	Mock.RenameWidgetResult.OldName = TEXT("TextBlock_0");
	Mock.RenameWidgetResult.NewName = TEXT("TitleText");

	FRenameWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("new_name"), TEXT("TitleText"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains new name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TitleText")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RenameWidget")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameWidgetMissingArgsTest,
	"MCPServer.Unit.UMG.RenameWidget.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameWidgetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FRenameWidgetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("RenameWidget")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameWidgetModuleFailureTest,
	"MCPServer.Unit.UMG.RenameWidget.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameWidgetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.RenameWidgetResult.bSuccess = false;
	Mock.RenameWidgetResult.ErrorMessage = TEXT("Name already in use");

	FRenameWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("new_name"), TEXT("Existing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Name already in use")));
	return true;
}

// ============================================================================
// ReplaceWidget
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReplaceWidgetMetadataTest,
	"MCPServer.Unit.UMG.ReplaceWidget.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReplaceWidgetMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FReplaceWidgetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("replace_widget"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReplaceWidgetSuccessTest,
	"MCPServer.Unit.UMG.ReplaceWidget.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReplaceWidgetSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ReplaceWidgetResult.bSuccess = true;
	Mock.ReplaceWidgetResult.WidgetName = TEXT("TextBlock_0");
	Mock.ReplaceWidgetResult.OldClass = TEXT("TextBlock");
	Mock.ReplaceWidgetResult.NewClass = TEXT("RichTextBlock");

	FReplaceWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("new_widget_class"), TEXT("RichTextBlock"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains class"), MCPTestUtils::GetResultText(Result).Contains(TEXT("RichTextBlock")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ReplaceWidget")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReplaceWidgetMissingArgsTest,
	"MCPServer.Unit.UMG.ReplaceWidget.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReplaceWidgetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FReplaceWidgetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ReplaceWidget")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReplaceWidgetModuleFailureTest,
	"MCPServer.Unit.UMG.ReplaceWidget.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReplaceWidgetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ReplaceWidgetResult.bSuccess = false;
	Mock.ReplaceWidgetResult.ErrorMessage = TEXT("Widget class not found");

	FReplaceWidgetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("new_widget_class"), TEXT("NonExistentWidget"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Widget class not found")));
	return true;
}

// ============================================================================
// SetWidgetProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetPropertyMetadataTest,
	"MCPServer.Unit.UMG.SetWidgetProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FSetWidgetPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_widget_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetPropertySuccessTest,
	"MCPServer.Unit.UMG.SetWidgetProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.SetWidgetPropertyResult.bSuccess = true;

	FSetWidgetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("property_name"), TEXT("Text"));
	Args->SetStringField(TEXT("property_value"), TEXT("Hello World"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetWidgetProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetPropertyMissingArgsTest,
	"MCPServer.Unit.UMG.SetWidgetProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FSetWidgetPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetWidgetProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetPropertyModuleFailureTest,
	"MCPServer.Unit.UMG.SetWidgetProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.SetWidgetPropertyResult.bSuccess = false;
	Mock.SetWidgetPropertyResult.ErrorMessage = TEXT("Property not found");

	FSetWidgetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("property_name"), TEXT("BadProp"));
	Args->SetStringField(TEXT("property_value"), TEXT("value"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ============================================================================
// GetWidgetProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetPropertyMetadataTest,
	"MCPServer.Unit.UMG.GetWidgetProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_widget_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetPropertySuccessTest,
	"MCPServer.Unit.UMG.GetWidgetProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetPropertyResult.bSuccess = true;
	Mock.GetWidgetPropertyResult.PropertyValue = TEXT("Hello World");

	FGetWidgetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	Args->SetStringField(TEXT("property_name"), TEXT("Text"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Hello World")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetPropertyMissingArgsTest,
	"MCPServer.Unit.UMG.GetWidgetProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetPropertyModuleFailureTest,
	"MCPServer.Unit.UMG.GetWidgetProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetPropertyResult.bSuccess = false;
	Mock.GetWidgetPropertyResult.ErrorMessage = TEXT("Widget not found");

	FGetWidgetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("Missing"));
	Args->SetStringField(TEXT("property_name"), TEXT("Text"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Widget not found")));
	return true;
}

// ============================================================================
// SetWidgetSlot
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetSlotMetadataTest,
	"MCPServer.Unit.UMG.SetWidgetSlot.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetSlotMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FSetWidgetSlotImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_widget_slot"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetSlotSuccessTest,
	"MCPServer.Unit.UMG.SetWidgetSlot.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetSlotSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.SetWidgetSlotResult.bSuccess = true;

	FSetWidgetSlotImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("TextBlock_0"));
	auto SlotProps = MakeShared<FJsonObject>();
	SlotProps->SetStringField(TEXT("HorizontalAlignment"), TEXT("Center"));
	Args->SetObjectField(TEXT("slot_properties"), SlotProps);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetWidgetSlot")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetSlotMissingArgsTest,
	"MCPServer.Unit.UMG.SetWidgetSlot.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetSlotMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FSetWidgetSlotImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetWidgetSlot")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetWidgetSlotModuleFailureTest,
	"MCPServer.Unit.UMG.SetWidgetSlot.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetWidgetSlotModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.SetWidgetSlotResult.bSuccess = false;
	Mock.SetWidgetSlotResult.ErrorMessage = TEXT("Widget has no slot");

	FSetWidgetSlotImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("widget_name"), TEXT("RootCanvas"));
	auto SlotProps = MakeShared<FJsonObject>();
	SlotProps->SetStringField(TEXT("Padding"), TEXT("10"));
	Args->SetObjectField(TEXT("slot_properties"), SlotProps);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Widget has no slot")));
	return true;
}

// ============================================================================
// GetWidgetAnimations
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetAnimationsMetadataTest,
	"MCPServer.Unit.UMG.GetWidgetAnimations.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetAnimationsMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetAnimationsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_widget_animations"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetAnimationsSuccessTest,
	"MCPServer.Unit.UMG.GetWidgetAnimations.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetAnimationsSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetAnimationsResult.bSuccess = true;
	FWidgetAnimationInfo Anim;
	Anim.AnimationName = TEXT("FadeIn");
	Anim.StartTime = 0.0f;
	Anim.EndTime = 1.0f;
	Mock.GetWidgetAnimationsResult.Animations.Add(Anim);

	FGetWidgetAnimationsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetAnimations")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetAnimationsMissingArgsTest,
	"MCPServer.Unit.UMG.GetWidgetAnimations.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetAnimationsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FGetWidgetAnimationsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetWidgetAnimations")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetWidgetAnimationsModuleFailureTest,
	"MCPServer.Unit.UMG.GetWidgetAnimations.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetWidgetAnimationsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.GetWidgetAnimationsResult.bSuccess = false;
	Mock.GetWidgetAnimationsResult.ErrorMessage = TEXT("Not a widget blueprint");

	FGetWidgetAnimationsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_NotWidget"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Not a widget blueprint")));
	return true;
}

// ============================================================================
// ExportWidgets
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportWidgetsMetadataTest,
	"MCPServer.Unit.UMG.ExportWidgets.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportWidgetsMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FExportWidgetsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("export_widgets"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportWidgetsSuccessTest,
	"MCPServer.Unit.UMG.ExportWidgets.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportWidgetsSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ExportWidgetsResult.bSuccess = true;
	Mock.ExportWidgetsResult.ExportedText = TEXT("Begin Object Class=TextBlock...");

	FExportWidgetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains exported text"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Begin Object")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ExportWidgets")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportWidgetsMissingArgsTest,
	"MCPServer.Unit.UMG.ExportWidgets.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportWidgetsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FExportWidgetsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ExportWidgets")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportWidgetsModuleFailureTest,
	"MCPServer.Unit.UMG.ExportWidgets.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportWidgetsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ExportWidgetsResult.bSuccess = false;
	Mock.ExportWidgetsResult.ErrorMessage = TEXT("No widgets to export");

	FExportWidgetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Empty"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No widgets to export")));
	return true;
}

// ============================================================================
// ImportWidgets
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportWidgetsMetadataTest,
	"MCPServer.Unit.UMG.ImportWidgets.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportWidgetsMetadataTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FImportWidgetsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("import_widgets"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportWidgetsSuccessTest,
	"MCPServer.Unit.UMG.ImportWidgets.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportWidgetsSuccessTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ImportWidgetsResult.bSuccess = true;
	Mock.ImportWidgetsResult.WidgetsImported = 3;

	FImportWidgetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("exported_text"), TEXT("Begin Object Class=TextBlock..."));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ImportWidgets")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportWidgetsMissingArgsTest,
	"MCPServer.Unit.UMG.ImportWidgets.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportWidgetsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	FImportWidgetsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ImportWidgets")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportWidgetsModuleFailureTest,
	"MCPServer.Unit.UMG.ImportWidgets.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportWidgetsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUMGModule Mock;
	Mock.ImportWidgetsResult.bSuccess = false;
	Mock.ImportWidgetsResult.ErrorMessage = TEXT("Invalid exported text format");

	FImportWidgetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/WBP_Test"));
	Args->SetStringField(TEXT("exported_text"), TEXT("garbage"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid exported text format")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
