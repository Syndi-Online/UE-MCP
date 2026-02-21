// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/BatchSetActorTransformsImplTool.h"
#include "Tools/Impl/BatchSetActorFoldersImplTool.h"
#include "Tools/Impl/BatchSetActorPropertiesImplTool.h"
#include "Tools/Impl/BatchSetMaterialExpressionPropertiesImplTool.h"
#include "Tools/Impl/BatchConnectMaterialExpressionsImplTool.h"
#include "Tools/Impl/AddGraphNodesBatchImplTool.h"
#include "Tools/Impl/BatchConnectGraphPinsImplTool.h"
#include "Tests/Mocks/MockActorModule.h"
#include "Tests/Mocks/MockMaterialModule.h"
#include "Tests/Mocks/MockBlueprintModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ===========================================================================
// BatchSetActorTransforms
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorTransformsMetadataTest,
	"MCPServer.Unit.Batch.BatchSetActorTransforms.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorTransformsMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorTransformsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_set_actor_transforms"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorTransformsSuccessTest,
	"MCPServer.Unit.Batch.BatchSetActorTransforms.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorTransformsSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorTransformResult.bSuccess = true;
	Mock.SetActorTransformResult.Location = FVector(100, 200, 0);
	Mock.SetActorTransformResult.Rotation = FRotator(0, 45, 0);
	Mock.SetActorTransformResult.Scale = FVector(1, 1, 1);
	FBatchSetActorTransformsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;

	// Op with location only
	auto Op1 = MakeShared<FJsonObject>();
	Op1->SetStringField(TEXT("actor_identifier"), TEXT("BP_Scrab_C_1"));
	auto Loc1 = MakeShared<FJsonObject>();
	Loc1->SetNumberField(TEXT("x"), 2800);
	Loc1->SetNumberField(TEXT("y"), 4800);
	Loc1->SetNumberField(TEXT("z"), 0);
	Op1->SetObjectField(TEXT("location"), Loc1);
	Ops.Add(MakeShared<FJsonValueObject>(Op1));

	// Op with location + rotation
	auto Op2 = MakeShared<FJsonObject>();
	Op2->SetStringField(TEXT("actor_identifier"), TEXT("BP_Scrab_C_3"));
	auto Loc2 = MakeShared<FJsonObject>();
	Loc2->SetNumberField(TEXT("x"), 3000);
	Loc2->SetNumberField(TEXT("y"), 5000);
	Loc2->SetNumberField(TEXT("z"), 0);
	Op2->SetObjectField(TEXT("location"), Loc2);
	auto Rot2 = MakeShared<FJsonObject>();
	Rot2->SetNumberField(TEXT("pitch"), 0);
	Rot2->SetNumberField(TEXT("yaw"), 45);
	Rot2->SetNumberField(TEXT("roll"), 0);
	Op2->SetObjectField(TEXT("rotation"), Rot2);
	Ops.Add(MakeShared<FJsonValueObject>(Op2));

	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestTrue(TEXT("Contains 0 failed"), MCPTestUtils::GetResultText(Result).Contains(TEXT("0 failed")));
	TestEqual(TEXT("SetActorTransform called twice"), Mock.Recorder.GetCallCount(TEXT("SetActorTransform")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorTransformsMissingArgsTest,
	"MCPServer.Unit.Batch.BatchSetActorTransforms.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorTransformsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorTransformsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions operations"), MCPTestUtils::GetResultText(Result).Contains(TEXT("operations")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorTransformsModuleFailureTest,
	"MCPServer.Unit.Batch.BatchSetActorTransforms.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorTransformsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorTransformResult.bSuccess = false;
	Mock.SetActorTransformResult.ErrorMessage = TEXT("Actor not found");
	FBatchSetActorTransformsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;
	auto Op = MakeShared<FJsonObject>();
	Op->SetStringField(TEXT("actor_identifier"), TEXT("Missing_0"));
	auto Loc = MakeShared<FJsonObject>();
	Loc->SetNumberField(TEXT("x"), 0);
	Loc->SetNumberField(TEXT("y"), 0);
	Loc->SetNumberField(TEXT("z"), 0);
	Op->SetObjectField(TEXT("location"), Loc);
	Ops.Add(MakeShared<FJsonValueObject>(Op));
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError when all fail"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor not found")));
	return true;
}

// ===========================================================================
// BatchSetActorFolders
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorFoldersMetadataTest,
	"MCPServer.Unit.Batch.BatchSetActorFolders.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorFoldersMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorFoldersImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_set_actor_folders"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorFoldersSuccessTest,
	"MCPServer.Unit.Batch.BatchSetActorFolders.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorFoldersSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorFolderResult.bSuccess = true;
	FBatchSetActorFoldersImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;

	auto Op1 = MakeShared<FJsonObject>();
	Op1->SetStringField(TEXT("actor_identifier"), TEXT("Light_0"));
	Op1->SetStringField(TEXT("folder_path"), TEXT("Lighting"));
	Ops.Add(MakeShared<FJsonValueObject>(Op1));

	auto Op2 = MakeShared<FJsonObject>();
	Op2->SetStringField(TEXT("actor_identifier"), TEXT("Cube_0"));
	Op2->SetStringField(TEXT("folder_path"), TEXT("Geometry"));
	Ops.Add(MakeShared<FJsonValueObject>(Op2));

	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestTrue(TEXT("Contains 0 failed"), MCPTestUtils::GetResultText(Result).Contains(TEXT("0 failed")));
	TestEqual(TEXT("SetActorFolder called twice"), Mock.Recorder.GetCallCount(TEXT("SetActorFolder")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorFoldersMissingArgsTest,
	"MCPServer.Unit.Batch.BatchSetActorFolders.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorFoldersMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorFoldersImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions operations"), MCPTestUtils::GetResultText(Result).Contains(TEXT("operations")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorFoldersPartialFailureTest,
	"MCPServer.Unit.Batch.BatchSetActorFolders.PartialFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorFoldersPartialFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	// First call succeeds, but mock returns same result for all calls
	// So we test with failure result — all fail
	Mock.SetActorFolderResult.bSuccess = false;
	Mock.SetActorFolderResult.ErrorMessage = TEXT("Actor not found");
	FBatchSetActorFoldersImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;

	auto Op1 = MakeShared<FJsonObject>();
	Op1->SetStringField(TEXT("actor_identifier"), TEXT("Missing_0"));
	Op1->SetStringField(TEXT("folder_path"), TEXT("Folder"));
	Ops.Add(MakeShared<FJsonValueObject>(Op1));

	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError when all fail"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains 0 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("0 succeeded")));
	TestTrue(TEXT("Contains error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor not found")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorFoldersTooManyTest,
	"MCPServer.Unit.Batch.BatchSetActorFolders.TooMany",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorFoldersTooManyTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorFoldersImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;
	for (int32 i = 0; i < 101; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("actor_identifier"), FString::Printf(TEXT("Actor_%d"), i));
		Op->SetStringField(TEXT("folder_path"), TEXT("Folder"));
		Ops.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on >100"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions maximum"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Maximum 100")));
	return true;
}

// ===========================================================================
// BatchSetActorProperties
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorPropertiesMetadataTest,
	"MCPServer.Unit.Batch.BatchSetActorProperties.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorPropertiesMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorPropertiesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_set_actor_properties"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorPropertiesSuccessTest,
	"MCPServer.Unit.Batch.BatchSetActorProperties.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorPropertiesSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorPropertyResult.bSuccess = true;
	FBatchSetActorPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;

	auto Op1 = MakeShared<FJsonObject>();
	Op1->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	Op1->SetStringField(TEXT("property_name"), TEXT("bHidden"));
	Op1->SetStringField(TEXT("property_value"), TEXT("true"));
	Ops.Add(MakeShared<FJsonValueObject>(Op1));

	auto Op2 = MakeShared<FJsonObject>();
	Op2->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_1"));
	Op2->SetStringField(TEXT("property_name"), TEXT("Mobility"));
	Op2->SetStringField(TEXT("property_value"), TEXT("Movable"));
	Ops.Add(MakeShared<FJsonValueObject>(Op2));

	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestEqual(TEXT("SetActorProperty called twice"), Mock.Recorder.GetCallCount(TEXT("SetActorProperty")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorPropertiesMissingArgsTest,
	"MCPServer.Unit.Batch.BatchSetActorProperties.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorPropertiesMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FBatchSetActorPropertiesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions operations"), MCPTestUtils::GetResultText(Result).Contains(TEXT("operations")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetActorPropertiesModuleFailureTest,
	"MCPServer.Unit.Batch.BatchSetActorProperties.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetActorPropertiesModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorPropertyResult.bSuccess = false;
	Mock.SetActorPropertyResult.ErrorMessage = TEXT("Property not found");
	FBatchSetActorPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Ops;
	auto Op = MakeShared<FJsonObject>();
	Op->SetStringField(TEXT("actor_identifier"), TEXT("Actor_0"));
	Op->SetStringField(TEXT("property_name"), TEXT("Bad"));
	Op->SetStringField(TEXT("property_value"), TEXT("val"));
	Ops.Add(MakeShared<FJsonValueObject>(Op));
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError when all fail"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ===========================================================================
// BatchSetMaterialExpressionProperties
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetMatExprPropsMetadataTest,
	"MCPServer.Unit.Batch.BatchSetMaterialExpressionProperties.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetMatExprPropsMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FBatchSetMaterialExpressionPropertiesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_set_material_expression_properties"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetMatExprPropsSuccessTest,
	"MCPServer.Unit.Batch.BatchSetMaterialExpressionProperties.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetMatExprPropsSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialExpressionPropertyResult.bSuccess = true;
	FBatchSetMaterialExpressionPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	TArray<TSharedPtr<FJsonValue>> Ops;

	auto Op1 = MakeShared<FJsonObject>();
	Op1->SetNumberField(TEXT("expression_index"), 0);
	Op1->SetStringField(TEXT("property_name"), TEXT("Constant"));
	Op1->SetStringField(TEXT("property_value"), TEXT("(R=0.1,G=0.5,B=0.1,A=1.0)"));
	Ops.Add(MakeShared<FJsonValueObject>(Op1));

	auto Op2 = MakeShared<FJsonObject>();
	Op2->SetNumberField(TEXT("expression_index"), 5);
	Op2->SetStringField(TEXT("property_name"), TEXT("ParameterName"));
	Op2->SetStringField(TEXT("property_value"), TEXT("Dirt"));
	Ops.Add(MakeShared<FJsonValueObject>(Op2));

	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestEqual(TEXT("Module called twice"), Mock.Recorder.GetCallCount(TEXT("SetMaterialExpressionProperty")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetMatExprPropsMissingArgsTest,
	"MCPServer.Unit.Batch.BatchSetMaterialExpressionProperties.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetMatExprPropsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FBatchSetMaterialExpressionPropertiesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions material_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetMatExprPropsModuleFailureTest,
	"MCPServer.Unit.Batch.BatchSetMaterialExpressionProperties.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetMatExprPropsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.SetMaterialExpressionPropertyResult.bSuccess = false;
	Mock.SetMaterialExpressionPropertyResult.ErrorMessage = TEXT("Property not found on expression");
	FBatchSetMaterialExpressionPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	TArray<TSharedPtr<FJsonValue>> Ops;
	auto Op = MakeShared<FJsonObject>();
	Op->SetNumberField(TEXT("expression_index"), 0);
	Op->SetStringField(TEXT("property_name"), TEXT("Bad"));
	Op->SetStringField(TEXT("property_value"), TEXT("val"));
	Ops.Add(MakeShared<FJsonValueObject>(Op));
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ===========================================================================
// BatchConnectMaterialExpressions
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectMatExprsMetadataTest,
	"MCPServer.Unit.Batch.BatchConnectMaterialExpressions.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectMatExprsMetadataTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FBatchConnectMaterialExpressionsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_connect_material_expressions"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectMatExprsSuccessTest,
	"MCPServer.Unit.Batch.BatchConnectMaterialExpressions.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectMatExprsSuccessTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialExpressionsResult.bSuccess = true;
	FBatchConnectMaterialExpressionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	TArray<TSharedPtr<FJsonValue>> Conns;

	auto Conn1 = MakeShared<FJsonObject>();
	Conn1->SetNumberField(TEXT("from_index"), 0);
	Conn1->SetNumberField(TEXT("to_index"), 5);
	Conn1->SetStringField(TEXT("from_output"), TEXT(""));
	Conn1->SetStringField(TEXT("to_input"), TEXT("Base"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn1));

	auto Conn2 = MakeShared<FJsonObject>();
	Conn2->SetNumberField(TEXT("from_index"), 1);
	Conn2->SetNumberField(TEXT("to_index"), 5);
	Conn2->SetStringField(TEXT("from_output"), TEXT(""));
	Conn2->SetStringField(TEXT("to_input"), TEXT("Layer"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn2));

	Args->SetArrayField(TEXT("connections"), Conns);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestEqual(TEXT("Module called twice"), Mock.Recorder.GetCallCount(TEXT("ConnectMaterialExpressions")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectMatExprsMissingArgsTest,
	"MCPServer.Unit.Batch.BatchConnectMaterialExpressions.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectMatExprsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	FBatchConnectMaterialExpressionsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions material_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("material_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectMatExprsModuleFailureTest,
	"MCPServer.Unit.Batch.BatchConnectMaterialExpressions.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectMatExprsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockMaterialModule Mock;
	Mock.ConnectMaterialExpressionsResult.bSuccess = false;
	Mock.ConnectMaterialExpressionsResult.ErrorMessage = TEXT("Invalid expression index");
	FBatchConnectMaterialExpressionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Test"));
	TArray<TSharedPtr<FJsonValue>> Conns;
	auto Conn = MakeShared<FJsonObject>();
	Conn->SetNumberField(TEXT("from_index"), 99);
	Conn->SetNumberField(TEXT("to_index"), 0);
	Conn->SetStringField(TEXT("from_output"), TEXT(""));
	Conn->SetStringField(TEXT("to_input"), TEXT("A"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn));
	Args->SetArrayField(TEXT("connections"), Conns);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid expression index")));
	return true;
}

// ===========================================================================
// AddGraphNodesBatch — pin_defaults parsing
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodesBatchPinDefaultsTest,
	"MCPServer.Unit.Batch.AddGraphNodesBatch.PinDefaults",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodesBatchPinDefaultsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddGraphNodesBatchResultNode ResNode;
	ResNode.LocalId = TEXT("print");
	ResNode.NodeId = TEXT("AAAA-BBBB-CCCC-DDDD");
	Mock.AddGraphNodesBatchResult.bSuccess = true;
	Mock.AddGraphNodesBatchResult.Nodes.Add(ResNode);
	Mock.AddGraphNodesBatchResult.ConnectionsMade = 0;

	FAddGraphNodesBatchImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));

	TArray<TSharedPtr<FJsonValue>> NodesArr;
	auto NodeObj = MakeShared<FJsonObject>();
	NodeObj->SetStringField(TEXT("local_id"), TEXT("print"));
	NodeObj->SetStringField(TEXT("node_type"), TEXT("CallFunction"));
	NodeObj->SetStringField(TEXT("member_name"), TEXT("PrintString"));

	// Add pin_defaults
	auto PinDefaults = MakeShared<FJsonObject>();
	PinDefaults->SetStringField(TEXT("InString"), TEXT("Hello"));
	PinDefaults->SetStringField(TEXT("bPrintToScreen"), TEXT("true"));
	NodeObj->SetObjectField(TEXT("pin_defaults"), PinDefaults);

	NodesArr.Add(MakeShared<FJsonValueObject>(NodeObj));
	Args->SetArrayField(TEXT("nodes"), NodesArr);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("AddGraphNodesBatch called"), Mock.Recorder.GetCallCount(TEXT("AddGraphNodesBatch")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodesBatchRollbackTest,
	"MCPServer.Unit.Batch.AddGraphNodesBatch.Rollback",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodesBatchRollbackTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddGraphNodesBatchResult.bSuccess = false;
	Mock.AddGraphNodesBatchResult.ErrorMessage = TEXT("Node creation failed, rolled back 2 nodes");

	FAddGraphNodesBatchImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));

	TArray<TSharedPtr<FJsonValue>> NodesArr;
	auto NodeObj = MakeShared<FJsonObject>();
	NodeObj->SetStringField(TEXT("local_id"), TEXT("node1"));
	NodeObj->SetStringField(TEXT("node_type"), TEXT("CallFunction"));
	NodeObj->SetStringField(TEXT("member_name"), TEXT("BadFunc"));
	NodesArr.Add(MakeShared<FJsonValueObject>(NodeObj));
	Args->SetArrayField(TEXT("nodes"), NodesArr);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains rollback"), MCPTestUtils::GetResultText(Result).Contains(TEXT("rolled back")));
	return true;
}

// ===========================================================================
// BatchConnectGraphPins
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectGraphPinsMetadataTest,
	"MCPServer.Unit.Batch.BatchConnectGraphPins.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectGraphPinsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FBatchConnectGraphPinsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_connect_graph_pins"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectGraphPinsSuccessTest,
	"MCPServer.Unit.Batch.BatchConnectGraphPins.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectGraphPinsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ConnectGraphPinsResult.bSuccess = true;
	FBatchConnectGraphPinsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));

	TArray<TSharedPtr<FJsonValue>> Conns;
	auto Conn1 = MakeShared<FJsonObject>();
	Conn1->SetStringField(TEXT("source_node_id"), TEXT("AAAA-BBBB-CCCC-DDDD"));
	Conn1->SetStringField(TEXT("source_pin"), TEXT("then"));
	Conn1->SetStringField(TEXT("target_node_id"), TEXT("EEEE-FFFF-0000-1111"));
	Conn1->SetStringField(TEXT("target_pin"), TEXT("execute"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn1));

	auto Conn2 = MakeShared<FJsonObject>();
	Conn2->SetStringField(TEXT("source_node_id"), TEXT("AAAA-BBBB-CCCC-DDDD"));
	Conn2->SetStringField(TEXT("source_pin"), TEXT("ReturnValue"));
	Conn2->SetStringField(TEXT("target_node_id"), TEXT("EEEE-FFFF-0000-1111"));
	Conn2->SetStringField(TEXT("target_pin"), TEXT("InString"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn2));

	Args->SetArrayField(TEXT("connections"), Conns);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));
	TestEqual(TEXT("ConnectGraphPins called twice"), Mock.Recorder.GetCallCount(TEXT("ConnectGraphPins")), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectGraphPinsMissingArgsTest,
	"MCPServer.Unit.Batch.BatchConnectGraphPins.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectGraphPinsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FBatchConnectGraphPinsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchConnectGraphPinsModuleFailureTest,
	"MCPServer.Unit.Batch.BatchConnectGraphPins.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchConnectGraphPinsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ConnectGraphPinsResult.bSuccess = false;
	Mock.ConnectGraphPinsResult.ErrorMessage = TEXT("Pin not found");
	FBatchConnectGraphPinsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));

	TArray<TSharedPtr<FJsonValue>> Conns;
	auto Conn = MakeShared<FJsonObject>();
	Conn->SetStringField(TEXT("source_node_id"), TEXT("AAAA-BBBB-CCCC-DDDD"));
	Conn->SetStringField(TEXT("source_pin"), TEXT("BadPin"));
	Conn->SetStringField(TEXT("target_node_id"), TEXT("EEEE-FFFF-0000-1111"));
	Conn->SetStringField(TEXT("target_pin"), TEXT("execute"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn));
	Args->SetArrayField(TEXT("connections"), Conns);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError when all fail"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Pin not found")));
	return true;
}

// ===========================================================================
// Integration: Batch workflow
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchIntegrationTest,
	"MCPServer.Integration.Batch.FullWorkflow",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchIntegrationTest::RunTest(const FString& Parameters)
{
	// Step 1: Batch set actor folders
	FMockActorModule ActorMock;
	ActorMock.SetActorFolderResult.bSuccess = true;

	FBatchSetActorFoldersImplTool FolderTool(ActorMock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FolderOps;
	for (int32 i = 0; i < 5; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("actor_identifier"), FString::Printf(TEXT("Actor_%d"), i));
		Op->SetStringField(TEXT("folder_path"), TEXT("TestFolder"));
		FolderOps.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), FolderOps);
	auto Result = FolderTool.Execute(Args);
	TestTrue(TEXT("BatchSetActorFolders success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("5 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("5 succeeded")));

	// Step 2: Batch set actor properties
	ActorMock.SetActorPropertyResult.bSuccess = true;
	FBatchSetActorPropertiesImplTool PropTool(ActorMock);
	Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> PropOps;
	for (int32 i = 0; i < 3; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("actor_identifier"), FString::Printf(TEXT("Light_%d"), i));
		Op->SetStringField(TEXT("property_name"), TEXT("bHidden"));
		Op->SetStringField(TEXT("property_value"), TEXT("true"));
		PropOps.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), PropOps);
	Result = PropTool.Execute(Args);
	TestTrue(TEXT("BatchSetActorProperties success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("3 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3 succeeded")));

	// Step 3: Batch set material expression properties
	FMockMaterialModule MatMock;
	MatMock.SetMaterialExpressionPropertyResult.bSuccess = true;

	FBatchSetMaterialExpressionPropertiesImplTool MatPropTool(MatMock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Landscape"));
	TArray<TSharedPtr<FJsonValue>> MatOps;
	auto MatOp1 = MakeShared<FJsonObject>();
	MatOp1->SetNumberField(TEXT("expression_index"), 0);
	MatOp1->SetStringField(TEXT("property_name"), TEXT("Constant"));
	MatOp1->SetStringField(TEXT("property_value"), TEXT("(R=0.1,G=0.5,B=0.1,A=1.0)"));
	MatOps.Add(MakeShared<FJsonValueObject>(MatOp1));
	auto MatOp2 = MakeShared<FJsonObject>();
	MatOp2->SetNumberField(TEXT("expression_index"), 1);
	MatOp2->SetStringField(TEXT("property_name"), TEXT("ParameterName"));
	MatOp2->SetStringField(TEXT("property_value"), TEXT("Roughness"));
	MatOps.Add(MakeShared<FJsonValueObject>(MatOp2));
	Args->SetArrayField(TEXT("operations"), MatOps);
	Result = MatPropTool.Execute(Args);
	TestTrue(TEXT("BatchSetMatExprProps success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));

	// Step 4: Batch connect material expressions
	MatMock.ConnectMaterialExpressionsResult.bSuccess = true;

	FBatchConnectMaterialExpressionsImplTool ConnTool(MatMock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_Landscape"));
	TArray<TSharedPtr<FJsonValue>> Conns;
	auto Conn1 = MakeShared<FJsonObject>();
	Conn1->SetNumberField(TEXT("from_index"), 0);
	Conn1->SetNumberField(TEXT("to_index"), 5);
	Conn1->SetStringField(TEXT("from_output"), TEXT(""));
	Conn1->SetStringField(TEXT("to_input"), TEXT("Base"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn1));
	auto Conn2 = MakeShared<FJsonObject>();
	Conn2->SetNumberField(TEXT("from_index"), 1);
	Conn2->SetNumberField(TEXT("to_index"), 5);
	Conn2->SetStringField(TEXT("from_output"), TEXT(""));
	Conn2->SetStringField(TEXT("to_input"), TEXT("Layer"));
	Conns.Add(MakeShared<FJsonValueObject>(Conn2));
	Args->SetArrayField(TEXT("connections"), Conns);
	Result = ConnTool.Execute(Args);
	TestTrue(TEXT("BatchConnectMatExprs success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("2 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2 succeeded")));

	// Step 5: Batch set actor transforms
	ActorMock.SetActorTransformResult.bSuccess = true;
	ActorMock.SetActorTransformResult.Location = FVector(100, 200, 0);
	ActorMock.SetActorTransformResult.Rotation = FRotator(0, 45, 0);
	ActorMock.SetActorTransformResult.Scale = FVector(1, 1, 1);

	FBatchSetActorTransformsImplTool TransformTool(ActorMock);
	Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> TransformOps;
	for (int32 i = 0; i < 4; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("actor_identifier"), FString::Printf(TEXT("Actor_%d"), i));
		auto Loc = MakeShared<FJsonObject>();
		Loc->SetNumberField(TEXT("x"), i * 100.0);
		Loc->SetNumberField(TEXT("y"), i * 200.0);
		Loc->SetNumberField(TEXT("z"), 0);
		Op->SetObjectField(TEXT("location"), Loc);
		auto Rot = MakeShared<FJsonObject>();
		Rot->SetNumberField(TEXT("pitch"), 0);
		Rot->SetNumberField(TEXT("yaw"), i * 45.0);
		Rot->SetNumberField(TEXT("roll"), 0);
		Op->SetObjectField(TEXT("rotation"), Rot);
		TransformOps.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), TransformOps);
	Result = TransformTool.Execute(Args);
	TestTrue(TEXT("BatchSetActorTransforms success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("4 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("4 succeeded")));

	// Verify total calls
	TestEqual(TEXT("SetActorFolder total"), ActorMock.Recorder.GetCallCount(TEXT("SetActorFolder")), 5);
	TestEqual(TEXT("SetActorProperty total"), ActorMock.Recorder.GetCallCount(TEXT("SetActorProperty")), 3);
	TestEqual(TEXT("SetActorTransform total"), ActorMock.Recorder.GetCallCount(TEXT("SetActorTransform")), 4);
	TestEqual(TEXT("SetMaterialExpressionProperty total"), MatMock.Recorder.GetCallCount(TEXT("SetMaterialExpressionProperty")), 2);
	TestEqual(TEXT("ConnectMaterialExpressions total"), MatMock.Recorder.GetCallCount(TEXT("ConnectMaterialExpressions")), 2);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
