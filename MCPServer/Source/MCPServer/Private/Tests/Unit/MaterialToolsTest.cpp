// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/CreateMaterialExpressionImplTool.h"
#include "Tools/Impl/DeleteMaterialExpressionImplTool.h"
#include "Tools/Impl/ConnectMaterialExpressionsImplTool.h"
#include "Tools/Impl/ConnectMaterialPropertyImplTool.h"
#include "Tools/Impl/RecompileMaterialImplTool.h"
#include "Tools/Impl/GetMaterialParameterImplTool.h"
#include "Tools/Impl/SetMaterialInstanceParameterImplTool.h"
#include "Tools/Impl/SetMaterialInstanceParentImplTool.h"
#include "Tools/Impl/GetMaterialStatisticsImplTool.h"
#include "Tests/Mocks/MockMaterialModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// CreateMaterialExpression
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateMaterialExpressionMetadataTest,
	"MCPServer.Unit.Material.CreateMaterialExpression.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateMaterialExpressionMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FCreateMaterialExpressionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_material_expression"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateMaterialExpressionSuccessTest,
	"MCPServer.Unit.Material.CreateMaterialExpression.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateMaterialExpressionSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.CreateMaterialExpressionResult.bSuccess = true;
	Mock.CreateMaterialExpressionResult.ExpressionName = TEXT("Constant");
	Mock.CreateMaterialExpressionResult.ExpressionIndex = 0;
	FCreateMaterialExpressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetStringField(TEXT("expression_class"), TEXT("Constant"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Constant"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Constant")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateMaterialExpressionMissingArgsTest,
	"MCPServer.Unit.Material.CreateMaterialExpression.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateMaterialExpressionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FCreateMaterialExpressionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateMaterialExpressionModuleFailureTest,
	"MCPServer.Unit.Material.CreateMaterialExpression.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateMaterialExpressionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.CreateMaterialExpressionResult.bSuccess = false;
	Mock.CreateMaterialExpressionResult.ErrorMessage = TEXT("Material not found");
	FCreateMaterialExpressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Missing"));
	Args->SetStringField(TEXT("expression_class"), TEXT("Constant"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Material not found")));
	return true;
}

// ---------------------------------------------------------------------------
// DeleteMaterialExpression
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteMaterialExpressionMetadataTest,
	"MCPServer.Unit.Material.DeleteMaterialExpression.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteMaterialExpressionMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FDeleteMaterialExpressionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_material_expression"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteMaterialExpressionSuccessTest,
	"MCPServer.Unit.Material.DeleteMaterialExpression.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteMaterialExpressionSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.DeleteMaterialExpressionResult.bSuccess = true;
	FDeleteMaterialExpressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("expression_index"), 0);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains success text"), MCPTestUtils::GetResultText(Result).Contains(TEXT("deleted")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteMaterialExpressionMissingArgsTest,
	"MCPServer.Unit.Material.DeleteMaterialExpression.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteMaterialExpressionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FDeleteMaterialExpressionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteMaterialExpressionModuleFailureTest,
	"MCPServer.Unit.Material.DeleteMaterialExpression.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteMaterialExpressionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.DeleteMaterialExpressionResult.bSuccess = false;
	Mock.DeleteMaterialExpressionResult.ErrorMessage = TEXT("Index out of range");
	FDeleteMaterialExpressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("expression_index"), 99);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Index out of range")));
	return true;
}

// ---------------------------------------------------------------------------
// ConnectMaterialExpressions
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialExpressionsMetadataTest,
	"MCPServer.Unit.Material.ConnectMaterialExpressions.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialExpressionsMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FConnectMaterialExpressionsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("connect_material_expressions"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialExpressionsSuccessTest,
	"MCPServer.Unit.Material.ConnectMaterialExpressions.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialExpressionsSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialExpressionsResult.bSuccess = true;
	FConnectMaterialExpressionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("from_index"), 0);
	Args->SetNumberField(TEXT("to_index"), 1);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains connected"), MCPTestUtils::GetResultText(Result).Contains(TEXT("connected")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialExpressionsMissingArgsTest,
	"MCPServer.Unit.Material.ConnectMaterialExpressions.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialExpressionsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FConnectMaterialExpressionsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialExpressionsModuleFailureTest,
	"MCPServer.Unit.Material.ConnectMaterialExpressions.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialExpressionsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialExpressionsResult.bSuccess = false;
	Mock.ConnectMaterialExpressionsResult.ErrorMessage = TEXT("Incompatible pin types");
	FConnectMaterialExpressionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("from_index"), 0);
	Args->SetNumberField(TEXT("to_index"), 1);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Incompatible pin types")));
	return true;
}

// ---------------------------------------------------------------------------
// ConnectMaterialProperty
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialPropertyMetadataTest,
	"MCPServer.Unit.Material.ConnectMaterialProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FConnectMaterialPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("connect_material_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialPropertySuccessTest,
	"MCPServer.Unit.Material.ConnectMaterialProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialPropertyResult.bSuccess = true;
	Mock.ConnectMaterialPropertyResult.PropertyName = TEXT("BaseColor");
	FConnectMaterialPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("from_index"), 0);
	Args->SetStringField(TEXT("property"), TEXT("BaseColor"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains BaseColor"), MCPTestUtils::GetResultText(Result).Contains(TEXT("BaseColor")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialPropertyMissingArgsTest,
	"MCPServer.Unit.Material.ConnectMaterialProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FConnectMaterialPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectMaterialPropertyModuleFailureTest,
	"MCPServer.Unit.Material.ConnectMaterialProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectMaterialPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialPropertyResult.bSuccess = false;
	Mock.ConnectMaterialPropertyResult.ErrorMessage = TEXT("Invalid property");
	FConnectMaterialPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetNumberField(TEXT("from_index"), 0);
	Args->SetStringField(TEXT("property"), TEXT("Invalid"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid property")));
	return true;
}

// ---------------------------------------------------------------------------
// RecompileMaterial
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecompileMaterialMetadataTest,
	"MCPServer.Unit.Material.RecompileMaterial.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecompileMaterialMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FRecompileMaterialImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("recompile_material"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecompileMaterialSuccessTest,
	"MCPServer.Unit.Material.RecompileMaterial.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecompileMaterialSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.RecompileMaterialResult.bSuccess = true;
	FRecompileMaterialImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains recompiled"), MCPTestUtils::GetResultText(Result).Contains(TEXT("recompiled")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecompileMaterialMissingArgsTest,
	"MCPServer.Unit.Material.RecompileMaterial.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecompileMaterialMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FRecompileMaterialImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecompileMaterialModuleFailureTest,
	"MCPServer.Unit.Material.RecompileMaterial.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecompileMaterialModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.RecompileMaterialResult.bSuccess = false;
	Mock.RecompileMaterialResult.ErrorMessage = TEXT("Compile error");
	FRecompileMaterialImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Compile error")));
	return true;
}

// ---------------------------------------------------------------------------
// GetMaterialParameter
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialParameterMetadataTest,
	"MCPServer.Unit.Material.GetMaterialParameter.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialParameterMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FGetMaterialParameterImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_material_parameter"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialParameterSuccessTest,
	"MCPServer.Unit.Material.GetMaterialParameter.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialParameterSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.GetMaterialParameterResult.bSuccess = true;
	Mock.GetMaterialParameterResult.Parameter.ParameterName = TEXT("Color");
	Mock.GetMaterialParameterResult.Parameter.Value = TEXT("(1,0,0)");
	FGetMaterialParameterImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetStringField(TEXT("parameter_name"), TEXT("Color"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Color"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Color")));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("(1,0,0)")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialParameterMissingArgsTest,
	"MCPServer.Unit.Material.GetMaterialParameter.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialParameterMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FGetMaterialParameterImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialParameterModuleFailureTest,
	"MCPServer.Unit.Material.GetMaterialParameter.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialParameterModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.GetMaterialParameterResult.bSuccess = false;
	Mock.GetMaterialParameterResult.ErrorMessage = TEXT("Parameter not found");
	FGetMaterialParameterImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	Args->SetStringField(TEXT("parameter_name"), TEXT("Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Parameter not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SetMaterialInstanceParameter
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParameterMetadataTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParameter.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParameterMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FSetMaterialInstanceParameterImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_material_instance_parameter"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParameterSuccessTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParameter.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParameterSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialInstanceParameterResult.bSuccess = true;
	FSetMaterialInstanceParameterImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("instance_path"), TEXT("/Game/MI_Test"));
	Args->SetStringField(TEXT("parameter_name"), TEXT("Color"));
	Args->SetStringField(TEXT("value"), TEXT("(1,0,0)"));
	Args->SetStringField(TEXT("parameter_type"), TEXT("Vector"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains success text"), MCPTestUtils::GetResultText(Result).Contains(TEXT("set successfully")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParameterMissingArgsTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParameter.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParameterMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FSetMaterialInstanceParameterImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions instance_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("instance_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParameterModuleFailureTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParameter.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParameterModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialInstanceParameterResult.bSuccess = false;
	Mock.SetMaterialInstanceParameterResult.ErrorMessage = TEXT("Instance not found");
	FSetMaterialInstanceParameterImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("instance_path"), TEXT("/Game/MI_Missing"));
	Args->SetStringField(TEXT("parameter_name"), TEXT("Color"));
	Args->SetStringField(TEXT("value"), TEXT("1.0"));
	Args->SetStringField(TEXT("parameter_type"), TEXT("Scalar"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Instance not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SetMaterialInstanceParent
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParentMetadataTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParent.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParentMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FSetMaterialInstanceParentImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_material_instance_parent"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParentSuccessTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParent.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParentSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialInstanceParentResult.bSuccess = true;
	FSetMaterialInstanceParentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("instance_path"), TEXT("/Game/MI_Test"));
	Args->SetStringField(TEXT("parent_path"), TEXT("/Game/M_Parent"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains success text"), MCPTestUtils::GetResultText(Result).Contains(TEXT("parent set successfully")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParentMissingArgsTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParent.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParentMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FSetMaterialInstanceParentImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions instance_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("instance_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMaterialInstanceParentModuleFailureTest,
	"MCPServer.Unit.Material.SetMaterialInstanceParent.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMaterialInstanceParentModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialInstanceParentResult.bSuccess = false;
	Mock.SetMaterialInstanceParentResult.ErrorMessage = TEXT("Parent material not found");
	FSetMaterialInstanceParentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("instance_path"), TEXT("/Game/MI_Test"));
	Args->SetStringField(TEXT("parent_path"), TEXT("/Game/M_Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Parent material not found")));
	return true;
}

// ---------------------------------------------------------------------------
// GetMaterialStatistics
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialStatisticsMetadataTest,
	"MCPServer.Unit.Material.GetMaterialStatistics.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialStatisticsMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FGetMaterialStatisticsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_material_statistics"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialStatisticsSuccessTest,
	"MCPServer.Unit.Material.GetMaterialStatistics.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialStatisticsSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.GetMaterialStatisticsResult.bSuccess = true;
	Mock.GetMaterialStatisticsResult.NumPixelShaderInstructions = 42;
	FGetMaterialStatisticsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 42"), MCPTestUtils::GetResultText(Result).Contains(TEXT("42")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialStatisticsMissingArgsTest,
	"MCPServer.Unit.Material.GetMaterialStatistics.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialStatisticsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FGetMaterialStatisticsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions material_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMaterialStatisticsModuleFailureTest,
	"MCPServer.Unit.Material.GetMaterialStatistics.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMaterialStatisticsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.GetMaterialStatisticsResult.bSuccess = false;
	Mock.GetMaterialStatisticsResult.ErrorMessage = TEXT("Material not compiled");
	FGetMaterialStatisticsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Material not compiled")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
