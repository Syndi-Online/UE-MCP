// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockBlueprintModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/CreateBlueprintImplTool.h"
#include "Tools/Impl/AddBlueprintVariableImplTool.h"
#include "Tools/Impl/RemoveBlueprintVariableImplTool.h"
#include "Tools/Impl/AddBlueprintFunctionImplTool.h"
#include "Tools/Impl/GetBlueprintGraphsImplTool.h"
#include "Tools/Impl/CompileBlueprintImplTool.h"
#include "Tools/Impl/ReparentBlueprintImplTool.h"
#include "Tools/Impl/OpenBlueprintEditorImplTool.h"
#include "Tools/Impl/FindFunctionImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintIntegrationTest,
	"MCPServer.Integration.Blueprint.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBlueprintIntegrationTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;

	const FString BPPath = TEXT("/Game/Blueprints/BP_TestActor");

	// Step 1: CreateBlueprint
	Mock.CreateBlueprintResult.bSuccess = true;
	Mock.CreateBlueprintResult.BlueprintName = TEXT("BP_TestActor");
	Mock.CreateBlueprintResult.BlueprintPath = BPPath;
	Mock.CreateBlueprintResult.ParentClass = TEXT("/Script/Engine.Actor");

	FCreateBlueprintImplTool CreateTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("parent_class"), TEXT("/Script/Engine.Actor"));
	auto Result = CreateTool.Execute(Args);
	TestTrue(TEXT("CreateBlueprint success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("CreateBlueprint result contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("BP_TestActor")));

	// Step 2: AddBlueprintVariable ("Health", "float")
	Mock.AddBlueprintVariableResult.bSuccess = true;
	Mock.AddBlueprintVariableResult.VariableName = TEXT("Health");

	FAddBlueprintVariableImplTool AddVarTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("variable_name"), TEXT("Health"));
	Args->SetStringField(TEXT("variable_type"), TEXT("float"));
	TestTrue(TEXT("AddVariable Health success"), MCPTestUtils::IsSuccess(AddVarTool.Execute(Args)));

	// Step 3: AddBlueprintVariable ("Temp", "int")
	Mock.AddBlueprintVariableResult.bSuccess = true;
	Mock.AddBlueprintVariableResult.VariableName = TEXT("Temp");

	FAddBlueprintVariableImplTool AddVarTool2(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("variable_name"), TEXT("Temp"));
	Args->SetStringField(TEXT("variable_type"), TEXT("int"));
	TestTrue(TEXT("AddVariable Temp success"), MCPTestUtils::IsSuccess(AddVarTool2.Execute(Args)));

	// Step 4: RemoveBlueprintVariable ("Temp")
	Mock.RemoveBlueprintVariableResult.bSuccess = true;
	Mock.RemoveBlueprintVariableResult.VariableName = TEXT("Temp");

	FRemoveBlueprintVariableImplTool RemoveVarTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("variable_name"), TEXT("Temp"));
	TestTrue(TEXT("RemoveVariable Temp success"), MCPTestUtils::IsSuccess(RemoveVarTool.Execute(Args)));

	// Step 5: AddBlueprintFunction ("CalcDamage")
	Mock.AddBlueprintFunctionResult.bSuccess = true;
	Mock.AddBlueprintFunctionResult.FunctionName = TEXT("CalcDamage");

	FAddBlueprintFunctionImplTool AddFuncTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("function_name"), TEXT("CalcDamage"));
	TestTrue(TEXT("AddFunction CalcDamage success"), MCPTestUtils::IsSuccess(AddFuncTool.Execute(Args)));

	// Step 6: GetBlueprintGraphs (verify)
	FBlueprintGraphInfo EventGraph;
	EventGraph.GraphName = TEXT("EventGraph");
	EventGraph.GraphType = TEXT("EventGraph");
	FBlueprintGraphInfo CalcGraph;
	CalcGraph.GraphName = TEXT("CalcDamage");
	CalcGraph.GraphType = TEXT("Function");
	Mock.GetBlueprintGraphsResult.bSuccess = true;
	Mock.GetBlueprintGraphsResult.Graphs.Add(EventGraph);
	Mock.GetBlueprintGraphsResult.Graphs.Add(CalcGraph);

	FGetBlueprintGraphsImplTool GetGraphsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Result = GetGraphsTool.Execute(Args);
	TestTrue(TEXT("GetBlueprintGraphs success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Graphs contain CalcDamage"), MCPTestUtils::GetResultText(Result).Contains(TEXT("CalcDamage")));

	// Step 7: CompileBlueprint
	Mock.CompileBlueprintResult.bSuccess = true;
	Mock.CompileBlueprintResult.NumErrors = 0;
	Mock.CompileBlueprintResult.NumWarnings = 0;

	FCompileBlueprintImplTool CompileTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	TestTrue(TEXT("CompileBlueprint success"), MCPTestUtils::IsSuccess(CompileTool.Execute(Args)));

	// Step 8: ReparentBlueprint
	Mock.ReparentBlueprintResult.bSuccess = true;
	Mock.ReparentBlueprintResult.OldParentClass = TEXT("/Script/Engine.Actor");
	Mock.ReparentBlueprintResult.NewParentClass = TEXT("/Script/Engine.Pawn");

	FReparentBlueprintImplTool ReparentTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	Args->SetStringField(TEXT("new_parent_class"), TEXT("/Script/Engine.Pawn"));
	TestTrue(TEXT("ReparentBlueprint success"), MCPTestUtils::IsSuccess(ReparentTool.Execute(Args)));

	// Step 9: OpenBlueprintEditor
	Mock.OpenBlueprintEditorResult.bSuccess = true;
	Mock.OpenBlueprintEditorResult.BlueprintName = TEXT("BP_TestActor");

	FOpenBlueprintEditorImplTool OpenEditorTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), BPPath);
	TestTrue(TEXT("OpenBlueprintEditor success"), MCPTestUtils::IsSuccess(OpenEditorTool.Execute(Args)));

	// Step 10: FindFunction
	FFindFunctionInfo FuncInfo;
	FuncInfo.FunctionName = TEXT("K2_SetActorLocation");
	FuncInfo.ClassName = TEXT("Actor");
	FuncInfo.DisplayName = TEXT("Set Actor Location");
	FFindFunctionParamInfo Param;
	Param.ParamName = TEXT("NewLocation");
	FuncInfo.Params.Add(Param);
	Mock.FindFunctionResult.bSuccess = true;
	Mock.FindFunctionResult.Functions.Add(FuncInfo);

	FFindFunctionImplTool FindFuncTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("search"), TEXT("SetActorLocation"));
	Result = FindFuncTool.Execute(Args);
	TestTrue(TEXT("FindFunction success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains K2_SetActorLocation"), MCPTestUtils::GetResultText(Result).Contains(TEXT("K2_SetActorLocation")));

	// Verify all calls were made
	TestEqual(TEXT("CreateBlueprint called"), Mock.Recorder.GetCallCount(TEXT("CreateBlueprint")), 1);
	TestEqual(TEXT("AddBlueprintVariable called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintVariable")), 2);
	TestEqual(TEXT("RemoveBlueprintVariable called"), Mock.Recorder.GetCallCount(TEXT("RemoveBlueprintVariable")), 1);
	TestEqual(TEXT("AddBlueprintFunction called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintFunction")), 1);
	TestEqual(TEXT("GetBlueprintGraphs called"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintGraphs")), 1);
	TestEqual(TEXT("CompileBlueprint called"), Mock.Recorder.GetCallCount(TEXT("CompileBlueprint")), 1);
	TestEqual(TEXT("ReparentBlueprint called"), Mock.Recorder.GetCallCount(TEXT("ReparentBlueprint")), 1);
	TestEqual(TEXT("OpenBlueprintEditor called"), Mock.Recorder.GetCallCount(TEXT("OpenBlueprintEditor")), 1);
	TestEqual(TEXT("FindFunction called"), Mock.Recorder.GetCallCount(TEXT("FindFunction")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 10);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
