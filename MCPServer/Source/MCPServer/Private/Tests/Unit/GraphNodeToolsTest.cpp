// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/GetGraphNodesImplTool.h"
#include "Tools/Impl/GetGraphNodesSummaryImplTool.h"
#include "Tools/Impl/SetNodePositionImplTool.h"
#include "Tools/Impl/BatchSetNodePositionsImplTool.h"
#include "Tools/Impl/AddCommentBoxImplTool.h"
#include "Tools/Impl/DeleteCommentBoxImplTool.h"
#include "Tools/Impl/SetCommentBoxPropertiesImplTool.h"
#include "Tests/Mocks/MockBlueprintModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ===========================================================================
// GetGraphNodes
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesMetadataTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetGraphNodesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_graph_nodes"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSuccessTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = true;

	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	Node1.PosX = 100;
	Node1.PosY = 200;

	FGraphNodePinInfo Pin1;
	Pin1.PinId = TEXT("PIN-001");
	Pin1.PinName = TEXT("then");
	Pin1.PinType = TEXT("exec");
	Pin1.Direction = TEXT("Output");
	Node1.Pins.Add(Pin1);

	Mock.GetGraphNodesResult.Nodes.Add(Node1);

	FGetGraphNodesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains 1 nodes"), Text.Contains(TEXT("1 nodes")));
	TestTrue(TEXT("Contains node_id"), Text.Contains(TEXT("GUID-001")));
	TestTrue(TEXT("Contains node_class"), Text.Contains(TEXT("K2Node_Event")));
	TestEqual(TEXT("GetGraphNodes called"), Mock.Recorder.GetCallCount(TEXT("GetGraphNodes")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesMissingArgsTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetGraphNodesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesModuleFailureTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = false;
	Mock.GetGraphNodesResult.ErrorMessage = TEXT("Blueprint not found");
	FGetGraphNodesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/Missing"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Blueprint not found")));
	return true;
}

// ===========================================================================
// GetGraphNodes with node_ids filter
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesWithNodeIdsFilterTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.NodeIdsFilter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesWithNodeIdsFilterTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = true;

	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	FGraphNodeInfo Node2;
	Node2.NodeId = TEXT("GUID-002");
	Node2.NodeClass = TEXT("K2Node_CallFunction");
	Node2.NodeTitle = TEXT("Print String");
	FGraphNodeInfo Node3;
	Node3.NodeId = TEXT("GUID-003");
	Node3.NodeClass = TEXT("K2Node_VariableGet");
	Node3.NodeTitle = TEXT("Get Health");
	Mock.GetGraphNodesResult.Nodes.Add(Node1);
	Mock.GetGraphNodesResult.Nodes.Add(Node2);
	Mock.GetGraphNodesResult.Nodes.Add(Node3);

	FGetGraphNodesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	TArray<TSharedPtr<FJsonValue>> NodeIds;
	NodeIds.Add(MakeShared<FJsonValueString>(TEXT("GUID-001")));
	NodeIds.Add(MakeShared<FJsonValueString>(TEXT("GUID-003")));
	Args->SetArrayField(TEXT("node_ids"), NodeIds);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains Returning 2 of 3"), Text.Contains(TEXT("Returning 2 of 3")));
	TestTrue(TEXT("Contains GUID-001"), Text.Contains(TEXT("GUID-001")));
	TestTrue(TEXT("Contains GUID-003"), Text.Contains(TEXT("GUID-003")));
	TestFalse(TEXT("Does not contain GUID-002"), Text.Contains(TEXT("GUID-002")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesEmptyNodeIdsTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodes.EmptyNodeIds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesEmptyNodeIdsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = true;

	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	Mock.GetGraphNodesResult.Nodes.Add(Node1);

	FGetGraphNodesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	// No node_ids — should return all nodes with original text
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains Graph has 1 nodes"), Text.Contains(TEXT("Graph has 1 nodes")));
	TestTrue(TEXT("Contains GUID-001"), Text.Contains(TEXT("GUID-001")));
	return true;
}

// ===========================================================================
// GetGraphNodesSummary
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSummaryMetadataTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodesSummary.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSummaryMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetGraphNodesSummaryImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_graph_nodes_summary"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSummarySuccessTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodesSummary.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSummarySuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = true;

	// Node1 has a pin connected to Node2's pin
	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	FGraphNodePinInfo Pin1;
	Pin1.PinId = TEXT("PIN-001");
	Pin1.PinName = TEXT("then");
	Pin1.PinType = TEXT("exec");
	Pin1.Direction = TEXT("Output");
	Pin1.ConnectedPinIds.Add(TEXT("PIN-002"));
	Node1.Pins.Add(Pin1);

	FGraphNodeInfo Node2;
	Node2.NodeId = TEXT("GUID-002");
	Node2.NodeClass = TEXT("K2Node_CallFunction");
	Node2.NodeTitle = TEXT("Print String");
	FGraphNodePinInfo Pin2;
	Pin2.PinId = TEXT("PIN-002");
	Pin2.PinName = TEXT("execute");
	Pin2.PinType = TEXT("exec");
	Pin2.Direction = TEXT("Input");
	Pin2.ConnectedPinIds.Add(TEXT("PIN-001"));
	Node2.Pins.Add(Pin2);

	Mock.GetGraphNodesResult.Nodes.Add(Node1);
	Mock.GetGraphNodesResult.Nodes.Add(Node2);

	FGetGraphNodesSummaryImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains 2 nodes"), Text.Contains(TEXT("2 nodes")));
	TestTrue(TEXT("Contains node_id GUID-001"), Text.Contains(TEXT("GUID-001")));
	TestTrue(TEXT("Contains node_id GUID-002"), Text.Contains(TEXT("GUID-002")));
	TestTrue(TEXT("Contains connected_node_ids"), Text.Contains(TEXT("connected_node_ids")));
	// Should NOT contain pin details
	TestFalse(TEXT("No pin_id"), Text.Contains(TEXT("pin_id")));
	TestFalse(TEXT("No position"), Text.Contains(TEXT("position")));
	TestFalse(TEXT("No size"), Text.Contains(TEXT("\"size\"")));
	TestEqual(TEXT("GetGraphNodes called"), Mock.Recorder.GetCallCount(TEXT("GetGraphNodes")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSummaryClassFilterTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodesSummary.ClassFilter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSummaryClassFilterTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = true;

	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	FGraphNodeInfo Node2;
	Node2.NodeId = TEXT("GUID-002");
	Node2.NodeClass = TEXT("K2Node_CallFunction");
	Node2.NodeTitle = TEXT("Print String");
	FGraphNodeInfo Node3;
	Node3.NodeId = TEXT("GUID-003");
	Node3.NodeClass = TEXT("K2Node_Event");
	Node3.NodeTitle = TEXT("Event Tick");
	Mock.GetGraphNodesResult.Nodes.Add(Node1);
	Mock.GetGraphNodesResult.Nodes.Add(Node2);
	Mock.GetGraphNodesResult.Nodes.Add(Node3);

	FGetGraphNodesSummaryImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("class_filter"), TEXT("K2Node_Event"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains 2 nodes filtered from 3"), Text.Contains(TEXT("2 nodes (filtered from 3 total)")));
	TestTrue(TEXT("Contains GUID-001"), Text.Contains(TEXT("GUID-001")));
	TestTrue(TEXT("Contains GUID-003"), Text.Contains(TEXT("GUID-003")));
	TestFalse(TEXT("Does not contain GUID-002"), Text.Contains(TEXT("GUID-002")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSummaryMissingArgsTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodesSummary.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSummaryMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetGraphNodesSummaryImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetGraphNodesSummaryModuleFailureTest,
	"MCPServer.Unit.GraphNodes.GetGraphNodesSummary.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetGraphNodesSummaryModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetGraphNodesResult.bSuccess = false;
	Mock.GetGraphNodesResult.ErrorMessage = TEXT("Blueprint not found");
	FGetGraphNodesSummaryImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/Missing"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Blueprint not found")));
	return true;
}

// ===========================================================================
// SetNodePosition
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetNodePositionMetadataTest,
	"MCPServer.Unit.GraphNodes.SetNodePosition.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetNodePositionMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetNodePositionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_node_position"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetNodePositionSuccessTest,
	"MCPServer.Unit.GraphNodes.SetNodePosition.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetNodePositionSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetNodePositionResult.bSuccess = true;
	FSetNodePositionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-001"));
	auto Pos = MakeShared<FJsonObject>();
	Pos->SetNumberField(TEXT("x"), 300);
	Pos->SetNumberField(TEXT("y"), 400);
	Args->SetObjectField(TEXT("position"), Pos);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains moved"), MCPTestUtils::GetResultText(Result).Contains(TEXT("moved")));
	TestEqual(TEXT("SetNodePosition called"), Mock.Recorder.GetCallCount(TEXT("SetNodePosition")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetNodePositionMissingArgsTest,
	"MCPServer.Unit.GraphNodes.SetNodePosition.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetNodePositionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetNodePositionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetNodePositionModuleFailureTest,
	"MCPServer.Unit.GraphNodes.SetNodePosition.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetNodePositionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetNodePositionResult.bSuccess = false;
	Mock.SetNodePositionResult.ErrorMessage = TEXT("Node not found");
	FSetNodePositionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("BAD"));
	auto Pos = MakeShared<FJsonObject>();
	Pos->SetNumberField(TEXT("x"), 0);
	Pos->SetNumberField(TEXT("y"), 0);
	Args->SetObjectField(TEXT("position"), Pos);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Node not found")));
	return true;
}

// ===========================================================================
// BatchSetNodePositions
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetNodePositionsMetadataTest,
	"MCPServer.Unit.GraphNodes.BatchSetNodePositions.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetNodePositionsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FBatchSetNodePositionsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("batch_set_node_positions"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetNodePositionsSuccessTest,
	"MCPServer.Unit.GraphNodes.BatchSetNodePositions.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetNodePositionsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetNodePositionResult.bSuccess = true;
	FBatchSetNodePositionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	TArray<TSharedPtr<FJsonValue>> Ops;
	for (int32 i = 0; i < 3; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("node_id"), FString::Printf(TEXT("GUID-%03d"), i));
		auto Pos = MakeShared<FJsonObject>();
		Pos->SetNumberField(TEXT("x"), i * 100);
		Pos->SetNumberField(TEXT("y"), i * 50);
		Op->SetObjectField(TEXT("position"), Pos);
		Ops.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 3 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3 succeeded")));
	TestEqual(TEXT("SetNodePosition called 3x"), Mock.Recorder.GetCallCount(TEXT("SetNodePosition")), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetNodePositionsMissingArgsTest,
	"MCPServer.Unit.GraphNodes.BatchSetNodePositions.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetNodePositionsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FBatchSetNodePositionsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatchSetNodePositionsModuleFailureTest,
	"MCPServer.Unit.GraphNodes.BatchSetNodePositions.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBatchSetNodePositionsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetNodePositionResult.bSuccess = false;
	Mock.SetNodePositionResult.ErrorMessage = TEXT("Node not found");
	FBatchSetNodePositionsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	TArray<TSharedPtr<FJsonValue>> Ops;
	auto Op = MakeShared<FJsonObject>();
	Op->SetStringField(TEXT("node_id"), TEXT("BAD"));
	auto Pos = MakeShared<FJsonObject>();
	Pos->SetNumberField(TEXT("x"), 0);
	Pos->SetNumberField(TEXT("y"), 0);
	Op->SetObjectField(TEXT("position"), Pos);
	Ops.Add(MakeShared<FJsonValueObject>(Op));
	Args->SetArrayField(TEXT("operations"), Ops);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Node not found")));
	return true;
}

// ===========================================================================
// AddCommentBox
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddCommentBoxMetadataTest,
	"MCPServer.Unit.GraphNodes.AddCommentBox.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddCommentBoxMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddCommentBoxImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_comment_box"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddCommentBoxSuccessTest,
	"MCPServer.Unit.GraphNodes.AddCommentBox.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddCommentBoxSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddCommentBoxResult.bSuccess = true;
	Mock.AddCommentBoxResult.NodeId = TEXT("COMMENT-001");
	FAddCommentBoxImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("comment_text"), TEXT("Input handling"));
	auto Pos = MakeShared<FJsonObject>();
	Pos->SetNumberField(TEXT("x"), 100);
	Pos->SetNumberField(TEXT("y"), 200);
	Args->SetObjectField(TEXT("position"), Pos);
	auto Size = MakeShared<FJsonObject>();
	Size->SetNumberField(TEXT("width"), 500);
	Size->SetNumberField(TEXT("height"), 300);
	Args->SetObjectField(TEXT("size"), Size);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains NodeId"), MCPTestUtils::GetResultText(Result).Contains(TEXT("COMMENT-001")));
	TestEqual(TEXT("AddCommentBox called"), Mock.Recorder.GetCallCount(TEXT("AddCommentBox")), 1);
	TestEqual(TEXT("CommentText passed to module"), Mock.LastAddCommentBoxText, TEXT("Input handling"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddCommentBoxMissingArgsTest,
	"MCPServer.Unit.GraphNodes.AddCommentBox.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddCommentBoxMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddCommentBoxImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddCommentBoxModuleFailureTest,
	"MCPServer.Unit.GraphNodes.AddCommentBox.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddCommentBoxModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddCommentBoxResult.bSuccess = false;
	Mock.AddCommentBoxResult.ErrorMessage = TEXT("Graph not found");
	FAddCommentBoxImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("BadGraph"));
	Args->SetStringField(TEXT("comment_text"), TEXT("test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Graph not found")));
	return true;
}

// ===========================================================================
// DeleteCommentBox
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteCommentBoxMetadataTest,
	"MCPServer.Unit.GraphNodes.DeleteCommentBox.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteCommentBoxMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FDeleteCommentBoxImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_comment_box"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteCommentBoxSuccessTest,
	"MCPServer.Unit.GraphNodes.DeleteCommentBox.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteCommentBoxSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.DeleteCommentBoxResult.bSuccess = true;
	FDeleteCommentBoxImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("COMMENT-001"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains deleted"), MCPTestUtils::GetResultText(Result).Contains(TEXT("deleted")));
	TestEqual(TEXT("DeleteCommentBox called"), Mock.Recorder.GetCallCount(TEXT("DeleteCommentBox")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteCommentBoxMissingArgsTest,
	"MCPServer.Unit.GraphNodes.DeleteCommentBox.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteCommentBoxMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FDeleteCommentBoxImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteCommentBoxModuleFailureTest,
	"MCPServer.Unit.GraphNodes.DeleteCommentBox.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteCommentBoxModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.DeleteCommentBoxResult.bSuccess = false;
	Mock.DeleteCommentBoxResult.ErrorMessage = TEXT("Not a Comment Box");
	FDeleteCommentBoxImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("NOT-COMMENT"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Not a Comment Box")));
	return true;
}

// ===========================================================================
// SetCommentBoxProperties
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCommentBoxPropsMetadataTest,
	"MCPServer.Unit.GraphNodes.SetCommentBoxProperties.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCommentBoxPropsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetCommentBoxPropertiesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_comment_box_properties"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCommentBoxPropsSuccessTest,
	"MCPServer.Unit.GraphNodes.SetCommentBoxProperties.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCommentBoxPropsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetCommentBoxPropertiesResult.bSuccess = true;
	FSetCommentBoxPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("COMMENT-001"));
	Args->SetStringField(TEXT("comment_text"), TEXT("Updated comment"));
	auto Color = MakeShared<FJsonObject>();
	Color->SetNumberField(TEXT("r"), 1.0);
	Color->SetNumberField(TEXT("g"), 0.0);
	Color->SetNumberField(TEXT("b"), 0.0);
	Color->SetNumberField(TEXT("a"), 1.0);
	Args->SetObjectField(TEXT("color"), Color);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains updated"), MCPTestUtils::GetResultText(Result).Contains(TEXT("updated")));
	TestEqual(TEXT("SetCommentBoxProperties called"), Mock.Recorder.GetCallCount(TEXT("SetCommentBoxProperties")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCommentBoxPropsMissingArgsTest,
	"MCPServer.Unit.GraphNodes.SetCommentBoxProperties.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCommentBoxPropsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetCommentBoxPropertiesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCommentBoxPropsModuleFailureTest,
	"MCPServer.Unit.GraphNodes.SetCommentBoxProperties.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCommentBoxPropsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetCommentBoxPropertiesResult.bSuccess = false;
	Mock.SetCommentBoxPropertiesResult.ErrorMessage = TEXT("Not a Comment Box");
	FSetCommentBoxPropertiesImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("NOT-COMMENT"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Not a Comment Box")));
	return true;
}

// ===========================================================================
// Integration: Graph Nodes workflow
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGraphNodesIntegrationTest,
	"MCPServer.Integration.GraphNodes.FullWorkflow",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGraphNodesIntegrationTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;

	// Setup: Two connected nodes
	FGraphNodeInfo Node1;
	Node1.NodeId = TEXT("GUID-001");
	Node1.NodeClass = TEXT("K2Node_Event");
	Node1.NodeTitle = TEXT("Event BeginPlay");
	Node1.PosX = 0;
	Node1.PosY = 0;
	FGraphNodePinInfo Pin1Out;
	Pin1Out.PinId = TEXT("PIN-001");
	Pin1Out.PinName = TEXT("then");
	Pin1Out.PinType = TEXT("exec");
	Pin1Out.Direction = TEXT("Output");
	Pin1Out.ConnectedPinIds.Add(TEXT("PIN-002"));
	Node1.Pins.Add(Pin1Out);

	FGraphNodeInfo Node2;
	Node2.NodeId = TEXT("GUID-002");
	Node2.NodeClass = TEXT("K2Node_CallFunction");
	Node2.NodeTitle = TEXT("Print String");
	Node2.PosX = 300;
	Node2.PosY = 0;
	FGraphNodePinInfo Pin2In;
	Pin2In.PinId = TEXT("PIN-002");
	Pin2In.PinName = TEXT("execute");
	Pin2In.PinType = TEXT("exec");
	Pin2In.Direction = TEXT("Input");
	Pin2In.ConnectedPinIds.Add(TEXT("PIN-001"));
	Node2.Pins.Add(Pin2In);

	FGraphNodeInfo Node3;
	Node3.NodeId = TEXT("GUID-003");
	Node3.NodeClass = TEXT("K2Node_VariableGet");
	Node3.NodeTitle = TEXT("Get Health");
	Node3.PosX = 100;
	Node3.PosY = 200;

	Mock.GetGraphNodesResult.bSuccess = true;
	Mock.GetGraphNodesResult.Nodes.Add(Node1);
	Mock.GetGraphNodesResult.Nodes.Add(Node2);
	Mock.GetGraphNodesResult.Nodes.Add(Node3);

	// Step 1: get_graph_nodes_summary — lightweight overview
	FGetGraphNodesSummaryImplTool SummaryTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	auto Result = SummaryTool.Execute(Args);
	TestTrue(TEXT("Step1: Summary success"), MCPTestUtils::IsSuccess(Result));
	FString SummaryText = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Step1: 3 nodes"), SummaryText.Contains(TEXT("3 nodes")));
	TestTrue(TEXT("Step1: has connected_node_ids"), SummaryText.Contains(TEXT("connected_node_ids")));
	TestFalse(TEXT("Step1: no pin_id"), SummaryText.Contains(TEXT("pin_id")));

	// Step 2: get_graph_nodes with node_ids filter — detailed view of specific nodes
	FGetGraphNodesImplTool GetNodesTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	TArray<TSharedPtr<FJsonValue>> FilterIds;
	FilterIds.Add(MakeShared<FJsonValueString>(TEXT("GUID-001")));
	FilterIds.Add(MakeShared<FJsonValueString>(TEXT("GUID-002")));
	Args->SetArrayField(TEXT("node_ids"), FilterIds);
	Result = GetNodesTool.Execute(Args);
	TestTrue(TEXT("Step2: Filtered success"), MCPTestUtils::IsSuccess(Result));
	FString FilteredText = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Step2: Returning 2 of 3"), FilteredText.Contains(TEXT("Returning 2 of 3")));
	TestTrue(TEXT("Step2: Has GUID-001"), FilteredText.Contains(TEXT("GUID-001")));
	TestTrue(TEXT("Step2: Has GUID-002"), FilteredText.Contains(TEXT("GUID-002")));
	TestFalse(TEXT("Step2: No GUID-003"), FilteredText.Contains(TEXT("GUID-003")));

	// Step 3: get_graph_nodes without filter — all nodes
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Result = GetNodesTool.Execute(Args);
	TestTrue(TEXT("Step3: GetGraphNodes success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step3: 3 nodes"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3 nodes")));

	// Step 4: Set node position
	Mock.SetNodePositionResult.bSuccess = true;
	FSetNodePositionImplTool SetPosTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-001"));
	auto Pos = MakeShared<FJsonObject>();
	Pos->SetNumberField(TEXT("x"), 100);
	Pos->SetNumberField(TEXT("y"), 200);
	Args->SetObjectField(TEXT("position"), Pos);
	Result = SetPosTool.Execute(Args);
	TestTrue(TEXT("Step4: SetNodePosition success"), MCPTestUtils::IsSuccess(Result));

	// Step 5: Batch set node positions
	FBatchSetNodePositionsImplTool BatchPosTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	TArray<TSharedPtr<FJsonValue>> Ops;
	for (int32 i = 0; i < 5; ++i)
	{
		auto Op = MakeShared<FJsonObject>();
		Op->SetStringField(TEXT("node_id"), FString::Printf(TEXT("GUID-%03d"), i));
		auto P = MakeShared<FJsonObject>();
		P->SetNumberField(TEXT("x"), i * 200);
		P->SetNumberField(TEXT("y"), 0);
		Op->SetObjectField(TEXT("position"), P);
		Ops.Add(MakeShared<FJsonValueObject>(Op));
	}
	Args->SetArrayField(TEXT("operations"), Ops);
	Result = BatchPosTool.Execute(Args);
	TestTrue(TEXT("Step5: Batch success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step5: 5 succeeded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("5 succeeded")));

	// Step 6: Add comment box
	Mock.AddCommentBoxResult.bSuccess = true;
	Mock.AddCommentBoxResult.NodeId = TEXT("COMMENT-001");
	FAddCommentBoxImplTool AddBoxTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("comment_text"), TEXT("Input handling section"));
	auto BoxPos = MakeShared<FJsonObject>();
	BoxPos->SetNumberField(TEXT("x"), -50);
	BoxPos->SetNumberField(TEXT("y"), -50);
	Args->SetObjectField(TEXT("position"), BoxPos);
	auto BoxSize = MakeShared<FJsonObject>();
	BoxSize->SetNumberField(TEXT("width"), 600);
	BoxSize->SetNumberField(TEXT("height"), 300);
	Args->SetObjectField(TEXT("size"), BoxSize);
	Result = AddBoxTool.Execute(Args);
	TestTrue(TEXT("Step6: AddCommentBox success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step6: Got NodeId"), MCPTestUtils::GetResultText(Result).Contains(TEXT("COMMENT-001")));

	// Step 7: Set comment box properties
	Mock.SetCommentBoxPropertiesResult.bSuccess = true;
	FSetCommentBoxPropertiesImplTool SetPropsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("COMMENT-001"));
	Args->SetStringField(TEXT("comment_text"), TEXT("Updated: Input handling"));
	auto Color = MakeShared<FJsonObject>();
	Color->SetNumberField(TEXT("r"), 0.2);
	Color->SetNumberField(TEXT("g"), 0.8);
	Color->SetNumberField(TEXT("b"), 0.2);
	Color->SetNumberField(TEXT("a"), 1.0);
	Args->SetObjectField(TEXT("color"), Color);
	Result = SetPropsTool.Execute(Args);
	TestTrue(TEXT("Step7: SetCommentBoxProperties success"), MCPTestUtils::IsSuccess(Result));

	// Step 8: Delete comment box
	Mock.DeleteCommentBoxResult.bSuccess = true;
	FDeleteCommentBoxImplTool DeleteBoxTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("COMMENT-001"));
	Result = DeleteBoxTool.Execute(Args);
	TestTrue(TEXT("Step8: DeleteCommentBox success"), MCPTestUtils::IsSuccess(Result));

	// Verify call counts
	TestEqual(TEXT("GetGraphNodes total"), Mock.Recorder.GetCallCount(TEXT("GetGraphNodes")), 4); // 1 summary + 1 filtered + 1 all + 1 in summary call
	TestEqual(TEXT("SetNodePosition total"), Mock.Recorder.GetCallCount(TEXT("SetNodePosition")), 6); // 1 + 5 batch
	TestEqual(TEXT("AddCommentBox total"), Mock.Recorder.GetCallCount(TEXT("AddCommentBox")), 1);
	TestEqual(TEXT("SetCommentBoxProperties total"), Mock.Recorder.GetCallCount(TEXT("SetCommentBoxProperties")), 1);
	TestEqual(TEXT("DeleteCommentBox total"), Mock.Recorder.GetCallCount(TEXT("DeleteCommentBox")), 1);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
