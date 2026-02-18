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

#endif // WITH_DEV_AUTOMATION_TESTS
