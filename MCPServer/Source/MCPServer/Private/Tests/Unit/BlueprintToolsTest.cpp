// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/CreateBlueprintImplTool.h"
#include "Tools/Impl/CreateBlueprintFromActorImplTool.h"
#include "Tools/Impl/CompileBlueprintImplTool.h"
#include "Tools/Impl/AddBlueprintVariableImplTool.h"
#include "Tools/Impl/RemoveBlueprintVariableImplTool.h"
#include "Tools/Impl/AddBlueprintFunctionImplTool.h"
#include "Tools/Impl/AddBlueprintInterfaceImplTool.h"
#include "Tools/Impl/GetBlueprintGraphsImplTool.h"
#include "Tools/Impl/ReparentBlueprintImplTool.h"
#include "Tools/Impl/OpenBlueprintEditorImplTool.h"
#include "Tests/Mocks/MockBlueprintModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// CreateBlueprint
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintMetadataTest,
	"MCPServer.Unit.Blueprints.CreateBlueprint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCreateBlueprintImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_blueprint"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintSuccessTest,
	"MCPServer.Unit.Blueprints.CreateBlueprint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CreateBlueprintResult.bSuccess = true;
	Mock.CreateBlueprintResult.BlueprintName = TEXT("BP_Test");
	Mock.CreateBlueprintResult.ParentClass = TEXT("Actor");

	FCreateBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("parent_class"), TEXT("Actor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains bp name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("BP_Test")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintMissingArgsTest,
	"MCPServer.Unit.Blueprints.CreateBlueprint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCreateBlueprintImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("CreateBlueprint")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintModuleFailureTest,
	"MCPServer.Unit.Blueprints.CreateBlueprint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CreateBlueprintResult.bSuccess = false;
	Mock.CreateBlueprintResult.ErrorMessage = TEXT("Create blueprint failed");

	FCreateBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("parent_class"), TEXT("Actor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Create blueprint failed")));
	return true;
}

// ============================================================================
// CreateBlueprintFromActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintFromActorMetadataTest,
	"MCPServer.Unit.Blueprints.CreateBlueprintFromActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintFromActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCreateBlueprintFromActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_blueprint_from_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintFromActorSuccessTest,
	"MCPServer.Unit.Blueprints.CreateBlueprintFromActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintFromActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CreateBlueprintFromActorResult.bSuccess = true;
	Mock.CreateBlueprintFromActorResult.BlueprintName = TEXT("BP_FromActor");

	FCreateBlueprintFromActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_FromActor"));
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains bp name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("BP_FromActor")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintFromActorMissingArgsTest,
	"MCPServer.Unit.Blueprints.CreateBlueprintFromActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintFromActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCreateBlueprintFromActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("CreateBlueprintFromActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateBlueprintFromActorModuleFailureTest,
	"MCPServer.Unit.Blueprints.CreateBlueprintFromActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateBlueprintFromActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CreateBlueprintFromActorResult.bSuccess = false;
	Mock.CreateBlueprintFromActorResult.ErrorMessage = TEXT("Actor not found");

	FCreateBlueprintFromActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_FromActor"));
	Args->SetStringField(TEXT("actor_identifier"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor not found")));
	return true;
}

// ============================================================================
// CompileBlueprint
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCompileBlueprintMetadataTest,
	"MCPServer.Unit.Blueprints.CompileBlueprint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCompileBlueprintMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCompileBlueprintImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("compile_blueprint"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCompileBlueprintSuccessTest,
	"MCPServer.Unit.Blueprints.CompileBlueprint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCompileBlueprintSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CompileBlueprintResult.bSuccess = true;
	Mock.CompileBlueprintResult.NumErrors = 0;
	Mock.CompileBlueprintResult.NumWarnings = 0;

	FCompileBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCompileBlueprintMissingArgsTest,
	"MCPServer.Unit.Blueprints.CompileBlueprint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCompileBlueprintMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FCompileBlueprintImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("CompileBlueprint")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCompileBlueprintModuleFailureTest,
	"MCPServer.Unit.Blueprints.CompileBlueprint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCompileBlueprintModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.CompileBlueprintResult.bSuccess = false;
	Mock.CompileBlueprintResult.ErrorMessage = TEXT("Compile failed");

	FCompileBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Compile failed")));
	return true;
}

// ============================================================================
// AddBlueprintVariable
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintVariableMetadataTest,
	"MCPServer.Unit.Blueprints.AddBlueprintVariable.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintVariableMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintVariableImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_blueprint_variable"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintVariableSuccessTest,
	"MCPServer.Unit.Blueprints.AddBlueprintVariable.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintVariableSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintVariableResult.bSuccess = true;
	Mock.AddBlueprintVariableResult.VariableName = TEXT("Health");

	FAddBlueprintVariableImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("variable_name"), TEXT("Health"));
	Args->SetStringField(TEXT("variable_type"), TEXT("float"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains var name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Health")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintVariableMissingArgsTest,
	"MCPServer.Unit.Blueprints.AddBlueprintVariable.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintVariableMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintVariableImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintVariable")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintVariableModuleFailureTest,
	"MCPServer.Unit.Blueprints.AddBlueprintVariable.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintVariableModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintVariableResult.bSuccess = false;
	Mock.AddBlueprintVariableResult.ErrorMessage = TEXT("Add variable failed");

	FAddBlueprintVariableImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("variable_name"), TEXT("Health"));
	Args->SetStringField(TEXT("variable_type"), TEXT("float"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Add variable failed")));
	return true;
}

// ============================================================================
// RemoveBlueprintVariable
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintVariableMetadataTest,
	"MCPServer.Unit.Blueprints.RemoveBlueprintVariable.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintVariableMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FRemoveBlueprintVariableImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_blueprint_variable"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintVariableSuccessTest,
	"MCPServer.Unit.Blueprints.RemoveBlueprintVariable.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintVariableSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.RemoveBlueprintVariableResult.bSuccess = true;
	Mock.RemoveBlueprintVariableResult.VariableName = TEXT("OldVar");

	FRemoveBlueprintVariableImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("variable_name"), TEXT("OldVar"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintVariableMissingArgsTest,
	"MCPServer.Unit.Blueprints.RemoveBlueprintVariable.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintVariableMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FRemoveBlueprintVariableImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("RemoveBlueprintVariable")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintVariableModuleFailureTest,
	"MCPServer.Unit.Blueprints.RemoveBlueprintVariable.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintVariableModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.RemoveBlueprintVariableResult.bSuccess = false;
	Mock.RemoveBlueprintVariableResult.ErrorMessage = TEXT("Remove variable failed");

	FRemoveBlueprintVariableImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("variable_name"), TEXT("OldVar"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Remove variable failed")));
	return true;
}

// ============================================================================
// AddBlueprintFunction
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintFunctionMetadataTest,
	"MCPServer.Unit.Blueprints.AddBlueprintFunction.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintFunctionMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintFunctionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_blueprint_function"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintFunctionSuccessTest,
	"MCPServer.Unit.Blueprints.AddBlueprintFunction.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintFunctionSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintFunctionResult.bSuccess = true;
	Mock.AddBlueprintFunctionResult.FunctionName = TEXT("DoSomething");

	FAddBlueprintFunctionImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("function_name"), TEXT("DoSomething"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains func name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("DoSomething")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintFunctionMissingArgsTest,
	"MCPServer.Unit.Blueprints.AddBlueprintFunction.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintFunctionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintFunctionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintFunction")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintFunctionModuleFailureTest,
	"MCPServer.Unit.Blueprints.AddBlueprintFunction.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintFunctionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintFunctionResult.bSuccess = false;
	Mock.AddBlueprintFunctionResult.ErrorMessage = TEXT("Add function failed");

	FAddBlueprintFunctionImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("function_name"), TEXT("DoSomething"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Add function failed")));
	return true;
}

// ============================================================================
// AddBlueprintInterface
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintInterfaceMetadataTest,
	"MCPServer.Unit.Blueprints.AddBlueprintInterface.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintInterfaceMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintInterfaceImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_blueprint_interface"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintInterfaceSuccessTest,
	"MCPServer.Unit.Blueprints.AddBlueprintInterface.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintInterfaceSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintInterfaceResult.bSuccess = true;
	Mock.AddBlueprintInterfaceResult.InterfaceName = TEXT("BPI_Interactable");

	FAddBlueprintInterfaceImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("interface_path"), TEXT("/Game/BPI_Interactable"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintInterfaceMissingArgsTest,
	"MCPServer.Unit.Blueprints.AddBlueprintInterface.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintInterfaceMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintInterfaceImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintInterface")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintInterfaceModuleFailureTest,
	"MCPServer.Unit.Blueprints.AddBlueprintInterface.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintInterfaceModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintInterfaceResult.bSuccess = false;
	Mock.AddBlueprintInterfaceResult.ErrorMessage = TEXT("Add interface failed");

	FAddBlueprintInterfaceImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("interface_path"), TEXT("/Game/BPI_Interactable"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Add interface failed")));
	return true;
}

// ============================================================================
// GetBlueprintGraphs
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintGraphsMetadataTest,
	"MCPServer.Unit.Blueprints.GetBlueprintGraphs.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintGraphsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintGraphsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_blueprint_graphs"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintGraphsSuccessTest,
	"MCPServer.Unit.Blueprints.GetBlueprintGraphs.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintGraphsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintGraphsResult.bSuccess = true;
	FBlueprintGraphInfo G1;
	G1.GraphName = TEXT("EventGraph");
	G1.GraphType = TEXT("EventGraph");
	FBlueprintGraphInfo G2;
	G2.GraphName = TEXT("ConstructionScript");
	G2.GraphType = TEXT("Function");
	Mock.GetBlueprintGraphsResult.Graphs.Add(G1);
	Mock.GetBlueprintGraphsResult.Graphs.Add(G2);

	FGetBlueprintGraphsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains graph name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("EventGraph")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintGraphsMissingArgsTest,
	"MCPServer.Unit.Blueprints.GetBlueprintGraphs.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintGraphsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintGraphsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintGraphs")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintGraphsModuleFailureTest,
	"MCPServer.Unit.Blueprints.GetBlueprintGraphs.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintGraphsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintGraphsResult.bSuccess = false;
	Mock.GetBlueprintGraphsResult.ErrorMessage = TEXT("Get graphs failed");

	FGetBlueprintGraphsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Get graphs failed")));
	return true;
}

// ============================================================================
// ReparentBlueprint
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReparentBlueprintMetadataTest,
	"MCPServer.Unit.Blueprints.ReparentBlueprint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReparentBlueprintMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FReparentBlueprintImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("reparent_blueprint"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReparentBlueprintSuccessTest,
	"MCPServer.Unit.Blueprints.ReparentBlueprint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReparentBlueprintSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ReparentBlueprintResult.bSuccess = true;
	Mock.ReparentBlueprintResult.NewParentClass = TEXT("Character");

	FReparentBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("new_parent_class"), TEXT("Character"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReparentBlueprintMissingArgsTest,
	"MCPServer.Unit.Blueprints.ReparentBlueprint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReparentBlueprintMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FReparentBlueprintImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ReparentBlueprint")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReparentBlueprintModuleFailureTest,
	"MCPServer.Unit.Blueprints.ReparentBlueprint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReparentBlueprintModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ReparentBlueprintResult.bSuccess = false;
	Mock.ReparentBlueprintResult.ErrorMessage = TEXT("Reparent failed");

	FReparentBlueprintImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("new_parent_class"), TEXT("Character"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Reparent failed")));
	return true;
}

// ============================================================================
// OpenBlueprintEditor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenBlueprintEditorMetadataTest,
	"MCPServer.Unit.Blueprints.OpenBlueprintEditor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenBlueprintEditorMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FOpenBlueprintEditorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("open_blueprint_editor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenBlueprintEditorSuccessTest,
	"MCPServer.Unit.Blueprints.OpenBlueprintEditor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenBlueprintEditorSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.OpenBlueprintEditorResult.bSuccess = true;
	Mock.OpenBlueprintEditorResult.BlueprintName = TEXT("BP_Test");

	FOpenBlueprintEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenBlueprintEditorMissingArgsTest,
	"MCPServer.Unit.Blueprints.OpenBlueprintEditor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenBlueprintEditorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FOpenBlueprintEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("OpenBlueprintEditor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenBlueprintEditorModuleFailureTest,
	"MCPServer.Unit.Blueprints.OpenBlueprintEditor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenBlueprintEditorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.OpenBlueprintEditorResult.bSuccess = false;
	Mock.OpenBlueprintEditorResult.ErrorMessage = TEXT("Open editor failed");

	FOpenBlueprintEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Open editor failed")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
