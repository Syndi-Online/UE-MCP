// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockMaterialModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/CreateMaterialExpressionImplTool.h"
#include "Tools/Impl/ConnectMaterialExpressionsImplTool.h"
#include "Tools/Impl/ConnectMaterialPropertyImplTool.h"
#include "Tools/Impl/RecompileMaterialImplTool.h"
#include "Tools/Impl/GetMaterialStatisticsImplTool.h"
#include "Tools/Impl/DeleteMaterialExpressionImplTool.h"
#include "Tools/Impl/GetMaterialExpressionsImplTool.h"
#include "Tools/Impl/GetMaterialExpressionPropertyImplTool.h"
#include "Tools/Impl/SetMaterialExpressionPropertyImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMaterialIntegrationTest,
	"MCPServer.Integration.Material.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMaterialIntegrationTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;

	const FString MatPath = TEXT("/Game/Materials/M_TestMaterial");

	// Step 1: CreateMaterialExpression (Constant3Vector)
	Mock.CreateMaterialExpressionResult.bSuccess = true;
	Mock.CreateMaterialExpressionResult.ExpressionName = TEXT("Constant3Vector_0");
	Mock.CreateMaterialExpressionResult.ExpressionClass = TEXT("MaterialExpressionConstant3Vector");
	Mock.CreateMaterialExpressionResult.ExpressionIndex = 0;

	FCreateMaterialExpressionImplTool CreateExpr1Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetStringField(TEXT("expression_class"), TEXT("MaterialExpressionConstant3Vector"));
	Args->SetNumberField(TEXT("node_pos_x"), 0);
	Args->SetNumberField(TEXT("node_pos_y"), 0);
	auto Result = CreateExpr1Tool.Execute(Args);
	TestTrue(TEXT("CreateExpression(Constant3Vector) success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Expression index in result"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Constant3Vector")));

	// Step 2: CreateMaterialExpression (Multiply)
	Mock.CreateMaterialExpressionResult.bSuccess = true;
	Mock.CreateMaterialExpressionResult.ExpressionName = TEXT("Multiply_0");
	Mock.CreateMaterialExpressionResult.ExpressionClass = TEXT("MaterialExpressionMultiply");
	Mock.CreateMaterialExpressionResult.ExpressionIndex = 1;

	FCreateMaterialExpressionImplTool CreateExpr2Tool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetStringField(TEXT("expression_class"), TEXT("MaterialExpressionMultiply"));
	Args->SetNumberField(TEXT("node_pos_x"), 200);
	Args->SetNumberField(TEXT("node_pos_y"), 0);
	Result = CreateExpr2Tool.Execute(Args);
	TestTrue(TEXT("CreateExpression(Multiply) success"), MCPTestUtils::IsSuccess(Result));

	// Step 3: ConnectMaterialExpressions (Constant3Vector -> Multiply)
	Mock.ConnectMaterialExpressionsResult.bSuccess = true;

	FConnectMaterialExpressionsImplTool ConnectExprTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("from_index"), 0);
	Args->SetStringField(TEXT("from_output"), TEXT(""));
	Args->SetNumberField(TEXT("to_index"), 1);
	Args->SetStringField(TEXT("to_input"), TEXT("A"));
	TestTrue(TEXT("ConnectExpressions success"), MCPTestUtils::IsSuccess(ConnectExprTool.Execute(Args)));

	// Step 4: ConnectMaterialProperty (Multiply -> BaseColor)
	Mock.ConnectMaterialPropertyResult.bSuccess = true;
	Mock.ConnectMaterialPropertyResult.PropertyName = TEXT("BaseColor");

	FConnectMaterialPropertyImplTool ConnectPropTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("from_index"), 1);
	Args->SetStringField(TEXT("from_output"), TEXT(""));
	Args->SetStringField(TEXT("property"), TEXT("BaseColor"));
	TestTrue(TEXT("ConnectMaterialProperty(BaseColor) success"), MCPTestUtils::IsSuccess(ConnectPropTool.Execute(Args)));

	// Step 5: RecompileMaterial
	Mock.RecompileMaterialResult.bSuccess = true;

	FRecompileMaterialImplTool RecompileTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	TestTrue(TEXT("RecompileMaterial success"), MCPTestUtils::IsSuccess(RecompileTool.Execute(Args)));

	// Step 6: GetMaterialStatistics
	Mock.GetMaterialStatisticsResult.bSuccess = true;
	Mock.GetMaterialStatisticsResult.NumVertexShaderInstructions = 10;
	Mock.GetMaterialStatisticsResult.NumPixelShaderInstructions = 25;
	Mock.GetMaterialStatisticsResult.NumSamplers = 2;

	FGetMaterialStatisticsImplTool StatsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Result = StatsTool.Execute(Args);
	TestTrue(TEXT("GetMaterialStatistics success"), MCPTestUtils::IsSuccess(Result));

	// Step 7: DeleteMaterialExpression (remove the Multiply node)
	Mock.DeleteMaterialExpressionResult.bSuccess = true;

	FDeleteMaterialExpressionImplTool DeleteExprTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("expression_index"), 1);
	TestTrue(TEXT("DeleteMaterialExpression success"), MCPTestUtils::IsSuccess(DeleteExprTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("CreateMaterialExpression called"), Mock.Recorder.GetCallCount(TEXT("CreateMaterialExpression")), 2);
	TestEqual(TEXT("ConnectMaterialExpressions called"), Mock.Recorder.GetCallCount(TEXT("ConnectMaterialExpressions")), 1);
	TestEqual(TEXT("ConnectMaterialProperty called"), Mock.Recorder.GetCallCount(TEXT("ConnectMaterialProperty")), 1);
	TestEqual(TEXT("RecompileMaterial called"), Mock.Recorder.GetCallCount(TEXT("RecompileMaterial")), 1);
	TestEqual(TEXT("GetMaterialStatistics called"), Mock.Recorder.GetCallCount(TEXT("GetMaterialStatistics")), 1);
	TestEqual(TEXT("DeleteMaterialExpression called"), Mock.Recorder.GetCallCount(TEXT("DeleteMaterialExpression")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 7);

	return true;
}

// ---------------------------------------------------------------------------
// Integration: Expression Properties Workflow
// Create expressions -> list them -> get property -> set property -> verify
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMaterialExpressionPropertiesIntegrationTest,
	"MCPServer.Integration.Material.ExpressionProperties",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMaterialExpressionPropertiesIntegrationTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	const FString MatPath = TEXT("/Game/Materials/M_Landscape");

	// Step 1: Create Constant3Vector expression
	Mock.CreateMaterialExpressionResult.bSuccess = true;
	Mock.CreateMaterialExpressionResult.ExpressionName = TEXT("MaterialExpressionConstant3Vector_0");
	Mock.CreateMaterialExpressionResult.ExpressionClass = TEXT("MaterialExpressionConstant3Vector");
	Mock.CreateMaterialExpressionResult.ExpressionIndex = 0;

	FCreateMaterialExpressionImplTool CreateTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetStringField(TEXT("expression_class"), TEXT("Constant3Vector"));
	Args->SetNumberField(TEXT("node_pos_x"), -600);
	Args->SetNumberField(TEXT("node_pos_y"), -300);
	auto Result = CreateTool.Execute(Args);
	TestTrue(TEXT("Create Constant3Vector success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: Create LandscapeLayerBlend expression
	Mock.CreateMaterialExpressionResult.ExpressionName = TEXT("MaterialExpressionLandscapeLayerBlend_0");
	Mock.CreateMaterialExpressionResult.ExpressionClass = TEXT("MaterialExpressionLandscapeLayerBlend");
	Mock.CreateMaterialExpressionResult.ExpressionIndex = 1;

	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetStringField(TEXT("expression_class"), TEXT("LandscapeLayerBlend"));
	Args->SetNumberField(TEXT("node_pos_x"), -200);
	Args->SetNumberField(TEXT("node_pos_y"), 0);
	Result = CreateTool.Execute(Args);
	TestTrue(TEXT("Create LandscapeLayerBlend success"), MCPTestUtils::IsSuccess(Result));

	// Step 3: Get all expressions — should list both
	FMCPMaterialExpressionInfo Info0;
	Info0.Index = 0;
	Info0.Name = TEXT("MaterialExpressionConstant3Vector_0");
	Info0.Class = TEXT("MaterialExpressionConstant3Vector");
	Info0.PosX = -600;
	Info0.PosY = -300;

	FMCPMaterialExpressionInfo Info1;
	Info1.Index = 1;
	Info1.Name = TEXT("MaterialExpressionLandscapeLayerBlend_0");
	Info1.Class = TEXT("MaterialExpressionLandscapeLayerBlend");
	Info1.PosX = -200;
	Info1.PosY = 0;

	Mock.GetMaterialExpressionsResult.bSuccess = true;
	Mock.GetMaterialExpressionsResult.Expressions.Empty();
	Mock.GetMaterialExpressionsResult.Expressions.Add(Info0);
	Mock.GetMaterialExpressionsResult.Expressions.Add(Info1);

	FGetMaterialExpressionsImplTool ListTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Result = ListTool.Execute(Args);
	TestTrue(TEXT("GetMaterialExpressions success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 expressions"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 expressions")));
	TestTrue(TEXT("Contains Constant3Vector"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Constant3Vector")));
	TestTrue(TEXT("Contains LandscapeLayerBlend"), MCPTestUtils::GetResultText(Result).Contains(TEXT("LandscapeLayerBlend")));

	// Step 4: Get property on Constant3Vector — read Constant color
	Mock.GetMaterialExpressionPropertyResult.bSuccess = true;
	Mock.GetMaterialExpressionPropertyResult.PropertyValue = TEXT("(R=0.000000,G=0.000000,B=0.000000,A=0.000000)");

	FGetMaterialExpressionPropertyImplTool GetPropTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("expression_index"), 0);
	Args->SetStringField(TEXT("property_name"), TEXT("Constant"));
	Result = GetPropTool.Execute(Args);
	TestTrue(TEXT("GetExpressionProperty success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains R=0"), MCPTestUtils::GetResultText(Result).Contains(TEXT("R=0")));

	// Step 5: Set property on Constant3Vector — set green color
	Mock.SetMaterialExpressionPropertyResult.bSuccess = true;

	FSetMaterialExpressionPropertyImplTool SetPropTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("expression_index"), 0);
	Args->SetStringField(TEXT("property_name"), TEXT("Constant"));
	Args->SetStringField(TEXT("property_value"), TEXT("(R=0.1,G=0.5,B=0.1,A=1.0)"));
	Result = SetPropTool.Execute(Args);
	TestTrue(TEXT("SetExpressionProperty success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains set successfully"), MCPTestUtils::GetResultText(Result).Contains(TEXT("set successfully")));

	// Step 6: Verify — get property again to check value changed
	Mock.GetMaterialExpressionPropertyResult.PropertyValue = TEXT("(R=0.100000,G=0.500000,B=0.100000,A=1.000000)");
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("expression_index"), 0);
	Args->SetStringField(TEXT("property_name"), TEXT("Constant"));
	Result = GetPropTool.Execute(Args);
	TestTrue(TEXT("GetExpressionProperty after set success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains new R value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("R=0.1")));

	// Step 7: Set Layers on LandscapeLayerBlend
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	Args->SetNumberField(TEXT("expression_index"), 1);
	Args->SetStringField(TEXT("property_name"), TEXT("Layers"));
	Args->SetStringField(TEXT("property_value"), TEXT("((LayerName=\"Grass\",BlendType=LB_WeightBlend),(LayerName=\"Dirt\",BlendType=LB_WeightBlend))"));
	Result = SetPropTool.Execute(Args);
	TestTrue(TEXT("SetExpressionProperty(Layers) success"), MCPTestUtils::IsSuccess(Result));

	// Step 8: Recompile after all changes
	Mock.RecompileMaterialResult.bSuccess = true;
	FRecompileMaterialImplTool RecompileTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), MatPath);
	TestTrue(TEXT("Recompile success"), MCPTestUtils::IsSuccess(RecompileTool.Execute(Args)));

	// Verify call counts
	TestEqual(TEXT("CreateMaterialExpression calls"), Mock.Recorder.GetCallCount(TEXT("CreateMaterialExpression")), 2);
	TestEqual(TEXT("GetMaterialExpressions calls"), Mock.Recorder.GetCallCount(TEXT("GetMaterialExpressions")), 1);
	TestEqual(TEXT("GetMaterialExpressionProperty calls"), Mock.Recorder.GetCallCount(TEXT("GetMaterialExpressionProperty")), 2);
	TestEqual(TEXT("SetMaterialExpressionProperty calls"), Mock.Recorder.GetCallCount(TEXT("SetMaterialExpressionProperty")), 2);
	TestEqual(TEXT("RecompileMaterial calls"), Mock.Recorder.GetCallCount(TEXT("RecompileMaterial")), 1);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
