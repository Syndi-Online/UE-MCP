// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/AddBlueprintComponentImplTool.h"
#include "Tools/Impl/RemoveBlueprintComponentImplTool.h"
#include "Tools/Impl/GetBlueprintComponentsImplTool.h"
#include "Tools/Impl/SetBlueprintComponentPropertyImplTool.h"
#include "Tools/Impl/GetBlueprintComponentPropertyImplTool.h"
#include "Tools/Impl/AddGraphNodeImplTool.h"
#include "Tools/Impl/ConnectGraphPinsImplTool.h"
#include "Tools/Impl/SetPinDefaultValueImplTool.h"
#include "Tools/Impl/DeleteGraphNodeImplTool.h"
#include "Tests/Mocks/MockBlueprintModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ===========================================================================
// AddBlueprintComponent
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintComponentMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddBlueprintComponent.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintComponentMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintComponentImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_blueprint_component"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintComponentSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddBlueprintComponent.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintComponentSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintComponentResult.bSuccess = true;
	Mock.AddBlueprintComponentResult.ComponentName = TEXT("MyMesh");
	Mock.AddBlueprintComponentResult.ComponentClass = TEXT("StaticMeshComponent");
	Mock.AddBlueprintComponentResult.ParentComponent = TEXT("(root)");
	FAddBlueprintComponentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_class"), TEXT("StaticMeshComponent"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains component name"), Text.Contains(TEXT("MyMesh")));
	TestTrue(TEXT("Contains class"), Text.Contains(TEXT("StaticMeshComponent")));
	TestEqual(TEXT("AddBlueprintComponent called"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintComponent")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintComponentMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddBlueprintComponent.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintComponentMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintComponentImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintComponentMissingClassTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddBlueprintComponent.MissingClass",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintComponentMissingClassTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddBlueprintComponentImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions component_class"), MCPTestUtils::GetResultText(Result).Contains(TEXT("component_class")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddBlueprintComponentModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddBlueprintComponent.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddBlueprintComponentModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddBlueprintComponentResult.bSuccess = false;
	Mock.AddBlueprintComponentResult.ErrorMessage = TEXT("Blueprint not found");
	FAddBlueprintComponentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/Missing"));
	Args->SetStringField(TEXT("component_class"), TEXT("StaticMeshComponent"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Blueprint not found")));
	return true;
}

// ===========================================================================
// RemoveBlueprintComponent
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintComponentMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.RemoveBlueprintComponent.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintComponentMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FRemoveBlueprintComponentImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_blueprint_component"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintComponentSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.RemoveBlueprintComponent.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintComponentSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.RemoveBlueprintComponentResult.bSuccess = true;
	FRemoveBlueprintComponentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains removed"), MCPTestUtils::GetResultText(Result).Contains(TEXT("removed")));
	TestEqual(TEXT("RemoveBlueprintComponent called"), Mock.Recorder.GetCallCount(TEXT("RemoveBlueprintComponent")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintComponentMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.RemoveBlueprintComponent.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintComponentMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FRemoveBlueprintComponentImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveBlueprintComponentModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.RemoveBlueprintComponent.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveBlueprintComponentModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.RemoveBlueprintComponentResult.bSuccess = false;
	Mock.RemoveBlueprintComponentResult.ErrorMessage = TEXT("Component not found");
	FRemoveBlueprintComponentImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("BadComp"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Component not found")));
	return true;
}

// ===========================================================================
// GetBlueprintComponents
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentsMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponents.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintComponentsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_blueprint_components"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentsSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponents.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintComponentsResult.bSuccess = true;

	FBlueprintComponentInfo Comp1;
	Comp1.ComponentName = TEXT("DefaultSceneRoot");
	Comp1.ComponentClass = TEXT("SceneComponent");
	Comp1.bIsRoot = true;

	FBlueprintComponentInfo Comp2;
	Comp2.ComponentName = TEXT("StaticMesh");
	Comp2.ComponentClass = TEXT("StaticMeshComponent");
	Comp2.ParentComponent = TEXT("DefaultSceneRoot");
	Comp2.bIsRoot = false;

	Mock.GetBlueprintComponentsResult.Components.Add(Comp1);
	Mock.GetBlueprintComponentsResult.Components.Add(Comp2);

	FGetBlueprintComponentsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains 2 components"), Text.Contains(TEXT("2 components")));
	TestTrue(TEXT("Contains DefaultSceneRoot"), Text.Contains(TEXT("DefaultSceneRoot")));
	TestTrue(TEXT("Contains StaticMesh"), Text.Contains(TEXT("StaticMesh")));
	TestEqual(TEXT("GetBlueprintComponents called"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintComponents")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentsMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponents.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintComponentsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentsModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponents.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintComponentsResult.bSuccess = false;
	Mock.GetBlueprintComponentsResult.ErrorMessage = TEXT("Blueprint not found");
	FGetBlueprintComponentsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Blueprint not found")));
	return true;
}

// ===========================================================================
// SetBlueprintComponentProperty
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBlueprintComponentPropertyMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetBlueprintComponentProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBlueprintComponentPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetBlueprintComponentPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_blueprint_component_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBlueprintComponentPropertySuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetBlueprintComponentProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBlueprintComponentPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetBlueprintComponentPropertyResult.bSuccess = true;
	FSetBlueprintComponentPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	Args->SetStringField(TEXT("property_name"), TEXT("bVisible"));
	Args->SetStringField(TEXT("property_value"), TEXT("true"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains property name"), Text.Contains(TEXT("bVisible")));
	TestTrue(TEXT("Contains component name"), Text.Contains(TEXT("MyMesh")));
	TestEqual(TEXT("SetBlueprintComponentProperty called"), Mock.Recorder.GetCallCount(TEXT("SetBlueprintComponentProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBlueprintComponentPropertyMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetBlueprintComponentProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBlueprintComponentPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetBlueprintComponentPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBlueprintComponentPropertyModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetBlueprintComponentProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBlueprintComponentPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetBlueprintComponentPropertyResult.bSuccess = false;
	Mock.SetBlueprintComponentPropertyResult.ErrorMessage = TEXT("Property not found");
	FSetBlueprintComponentPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	Args->SetStringField(TEXT("property_name"), TEXT("BadProp"));
	Args->SetStringField(TEXT("property_value"), TEXT("true"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ===========================================================================
// GetBlueprintComponentProperty
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentPropertyMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponentProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintComponentPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_blueprint_component_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentPropertySuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponentProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintComponentPropertyResult.bSuccess = true;
	Mock.GetBlueprintComponentPropertyResult.PropertyValue = TEXT("true");
	FGetBlueprintComponentPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	Args->SetStringField(TEXT("property_name"), TEXT("bVisible"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains property name"), Text.Contains(TEXT("bVisible")));
	TestTrue(TEXT("Contains value"), Text.Contains(TEXT("true")));
	TestEqual(TEXT("GetBlueprintComponentProperty called"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintComponentProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentPropertyMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponentProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FGetBlueprintComponentPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBlueprintComponentPropertyModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.GetBlueprintComponentProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBlueprintComponentPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.GetBlueprintComponentPropertyResult.bSuccess = false;
	Mock.GetBlueprintComponentPropertyResult.ErrorMessage = TEXT("Component not found");
	FGetBlueprintComponentPropertyImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("BadComp"));
	Args->SetStringField(TEXT("property_name"), TEXT("bVisible"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Component not found")));
	return true;
}

// ===========================================================================
// AddGraphNode
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodeMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddGraphNode.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodeMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddGraphNodeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_graph_node"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodeSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddGraphNode.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodeSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddGraphNodeResult.bSuccess = true;
	Mock.AddGraphNodeResult.NodeId = TEXT("GUID-NEW-001");

	FGraphNodePinInfo Pin1;
	Pin1.PinId = TEXT("PIN-001");
	Pin1.PinName = TEXT("execute");
	Pin1.PinType = TEXT("exec");
	Pin1.Direction = TEXT("Input");
	Mock.AddGraphNodeResult.Pins.Add(Pin1);

	FGraphNodePinInfo Pin2;
	Pin2.PinId = TEXT("PIN-002");
	Pin2.PinName = TEXT("then");
	Pin2.PinType = TEXT("exec");
	Pin2.Direction = TEXT("Output");
	Mock.AddGraphNodeResult.Pins.Add(Pin2);

	FAddGraphNodeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_type"), TEXT("CallFunction"));
	Args->SetStringField(TEXT("member_name"), TEXT("PrintString"));
	Args->SetStringField(TEXT("target"), TEXT("KismetSystemLibrary"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains node_id"), Text.Contains(TEXT("GUID-NEW-001")));
	TestTrue(TEXT("Contains 2 pins"), Text.Contains(TEXT("2 pins")));
	TestEqual(TEXT("AddGraphNode called"), Mock.Recorder.GetCallCount(TEXT("AddGraphNode")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodeMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddGraphNode.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FAddGraphNodeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddGraphNodeModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.AddGraphNode.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddGraphNodeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.AddGraphNodeResult.bSuccess = false;
	Mock.AddGraphNodeResult.ErrorMessage = TEXT("Function not found");
	FAddGraphNodeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_type"), TEXT("CallFunction"));
	Args->SetStringField(TEXT("member_name"), TEXT("BadFunction"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Function not found")));
	return true;
}

// ===========================================================================
// ConnectGraphPins
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectGraphPinsMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.ConnectGraphPins.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectGraphPinsMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FConnectGraphPinsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("connect_graph_pins"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectGraphPinsSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.ConnectGraphPins.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectGraphPinsSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ConnectGraphPinsResult.bSuccess = true;
	FConnectGraphPinsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("source_node_id"), TEXT("GUID-001"));
	Args->SetStringField(TEXT("source_pin_name"), TEXT("then"));
	Args->SetStringField(TEXT("target_node_id"), TEXT("GUID-002"));
	Args->SetStringField(TEXT("target_pin_name"), TEXT("execute"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains Connected"), Text.Contains(TEXT("Connected")));
	TestEqual(TEXT("ConnectGraphPins called"), Mock.Recorder.GetCallCount(TEXT("ConnectGraphPins")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectGraphPinsMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.ConnectGraphPins.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectGraphPinsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FConnectGraphPinsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConnectGraphPinsModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.ConnectGraphPins.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConnectGraphPinsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.ConnectGraphPinsResult.bSuccess = false;
	Mock.ConnectGraphPinsResult.ErrorMessage = TEXT("Source node not found");
	FConnectGraphPinsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("source_node_id"), TEXT("BAD"));
	Args->SetStringField(TEXT("source_pin_name"), TEXT("then"));
	Args->SetStringField(TEXT("target_node_id"), TEXT("GUID-002"));
	Args->SetStringField(TEXT("target_pin_name"), TEXT("execute"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Source node not found")));
	return true;
}

// ===========================================================================
// SetPinDefaultValue
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPinDefaultValueMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetPinDefaultValue.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPinDefaultValueMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetPinDefaultValueImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_pin_default_value"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPinDefaultValueSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetPinDefaultValue.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPinDefaultValueSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetPinDefaultValueResult.bSuccess = true;
	FSetPinDefaultValueImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-001"));
	Args->SetStringField(TEXT("pin_name"), TEXT("InString"));
	Args->SetStringField(TEXT("default_value"), TEXT("Hello World"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	FString Text = MCPTestUtils::GetResultText(Result);
	TestTrue(TEXT("Contains pin name"), Text.Contains(TEXT("InString")));
	TestTrue(TEXT("Contains value"), Text.Contains(TEXT("Hello World")));
	TestEqual(TEXT("SetPinDefaultValue called"), Mock.Recorder.GetCallCount(TEXT("SetPinDefaultValue")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPinDefaultValueMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetPinDefaultValue.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPinDefaultValueMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FSetPinDefaultValueImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPinDefaultValueModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.SetPinDefaultValue.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPinDefaultValueModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.SetPinDefaultValueResult.bSuccess = false;
	Mock.SetPinDefaultValueResult.ErrorMessage = TEXT("Pin not found");
	FSetPinDefaultValueImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-001"));
	Args->SetStringField(TEXT("pin_name"), TEXT("BadPin"));
	Args->SetStringField(TEXT("default_value"), TEXT("value"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Pin not found")));
	return true;
}

// ===========================================================================
// DeleteGraphNode
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteGraphNodeMetadataTest,
	"MCPServer.Unit.BlueprintComponentGraph.DeleteGraphNode.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteGraphNodeMetadataTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FDeleteGraphNodeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_graph_node"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteGraphNodeSuccessTest,
	"MCPServer.Unit.BlueprintComponentGraph.DeleteGraphNode.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteGraphNodeSuccessTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.DeleteGraphNodeResult.bSuccess = true;
	FDeleteGraphNodeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-001"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains deleted"), MCPTestUtils::GetResultText(Result).Contains(TEXT("deleted")));
	TestEqual(TEXT("DeleteGraphNode called"), Mock.Recorder.GetCallCount(TEXT("DeleteGraphNode")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteGraphNodeMissingArgsTest,
	"MCPServer.Unit.BlueprintComponentGraph.DeleteGraphNode.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteGraphNodeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	FDeleteGraphNodeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Mentions blueprint_path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("blueprint_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteGraphNodeModuleFailureTest,
	"MCPServer.Unit.BlueprintComponentGraph.DeleteGraphNode.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteGraphNodeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;
	Mock.DeleteGraphNodeResult.bSuccess = false;
	Mock.DeleteGraphNodeResult.ErrorMessage = TEXT("Node not found");
	FDeleteGraphNodeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("BAD-NODE"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("isError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Node not found")));
	return true;
}

// ===========================================================================
// Integration: Blueprint Components + Graph Editing full workflow
// ===========================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintComponentGraphIntegrationTest,
	"MCPServer.Integration.BlueprintComponentGraph.FullWorkflow",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBlueprintComponentGraphIntegrationTest::RunTest(const FString& Parameters)
{
	FMockBlueprintModule Mock;

	// === Section 23: Blueprint Components ===

	// Step 1: Get components (empty initially)
	Mock.GetBlueprintComponentsResult.bSuccess = true;
	FGetBlueprintComponentsImplTool GetCompsTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	auto Result = GetCompsTool.Execute(Args);
	TestTrue(TEXT("Step1: GetComponents success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step1: 0 components"), MCPTestUtils::GetResultText(Result).Contains(TEXT("0 components")));

	// Step 2: Add root component (StaticMeshComponent)
	Mock.AddBlueprintComponentResult.bSuccess = true;
	Mock.AddBlueprintComponentResult.ComponentName = TEXT("MyMesh");
	Mock.AddBlueprintComponentResult.ComponentClass = TEXT("StaticMeshComponent");
	Mock.AddBlueprintComponentResult.ParentComponent = TEXT("(root)");
	FAddBlueprintComponentImplTool AddCompTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_class"), TEXT("StaticMeshComponent"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyMesh"));
	Result = AddCompTool.Execute(Args);
	TestTrue(TEXT("Step2: AddComponent success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step2: Contains MyMesh"), MCPTestUtils::GetResultText(Result).Contains(TEXT("MyMesh")));

	// Step 3: Add child component (PointLightComponent)
	Mock.AddBlueprintComponentResult.ComponentName = TEXT("MyLight");
	Mock.AddBlueprintComponentResult.ComponentClass = TEXT("PointLightComponent");
	Mock.AddBlueprintComponentResult.ParentComponent = TEXT("MyMesh");
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_class"), TEXT("PointLightComponent"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyLight"));
	Args->SetStringField(TEXT("parent_component"), TEXT("MyMesh"));
	Result = AddCompTool.Execute(Args);
	TestTrue(TEXT("Step3: AddChild success"), MCPTestUtils::IsSuccess(Result));

	// Step 4: Set component property
	Mock.SetBlueprintComponentPropertyResult.bSuccess = true;
	FSetBlueprintComponentPropertyImplTool SetPropTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyLight"));
	Args->SetStringField(TEXT("property_name"), TEXT("Intensity"));
	Args->SetStringField(TEXT("property_value"), TEXT("5000.0"));
	Result = SetPropTool.Execute(Args);
	TestTrue(TEXT("Step4: SetProperty success"), MCPTestUtils::IsSuccess(Result));

	// Step 5: Get component property
	Mock.GetBlueprintComponentPropertyResult.bSuccess = true;
	Mock.GetBlueprintComponentPropertyResult.PropertyValue = TEXT("5000.0");
	FGetBlueprintComponentPropertyImplTool GetPropTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyLight"));
	Args->SetStringField(TEXT("property_name"), TEXT("Intensity"));
	Result = GetPropTool.Execute(Args);
	TestTrue(TEXT("Step5: GetProperty success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step5: Contains 5000"), MCPTestUtils::GetResultText(Result).Contains(TEXT("5000")));

	// Step 6: Remove component
	Mock.RemoveBlueprintComponentResult.bSuccess = true;
	FRemoveBlueprintComponentImplTool RemoveCompTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("component_name"), TEXT("MyLight"));
	Result = RemoveCompTool.Execute(Args);
	TestTrue(TEXT("Step6: RemoveComponent success"), MCPTestUtils::IsSuccess(Result));

	// === Section 24: Graph Editing ===

	// Step 7: Add a CallFunction node
	Mock.AddGraphNodeResult.bSuccess = true;
	Mock.AddGraphNodeResult.NodeId = TEXT("GUID-PRINT");
	FGraphNodePinInfo ExecIn;
	ExecIn.PinId = TEXT("PIN-EXEC-IN");
	ExecIn.PinName = TEXT("execute");
	ExecIn.PinType = TEXT("exec");
	ExecIn.Direction = TEXT("Input");
	FGraphNodePinInfo ExecOut;
	ExecOut.PinId = TEXT("PIN-EXEC-OUT");
	ExecOut.PinName = TEXT("then");
	ExecOut.PinType = TEXT("exec");
	ExecOut.Direction = TEXT("Output");
	FGraphNodePinInfo StringPin;
	StringPin.PinId = TEXT("PIN-STRING");
	StringPin.PinName = TEXT("InString");
	StringPin.PinType = TEXT("string");
	StringPin.Direction = TEXT("Input");
	Mock.AddGraphNodeResult.Pins.Add(ExecIn);
	Mock.AddGraphNodeResult.Pins.Add(ExecOut);
	Mock.AddGraphNodeResult.Pins.Add(StringPin);

	FAddGraphNodeImplTool AddNodeTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_type"), TEXT("CallFunction"));
	Args->SetStringField(TEXT("member_name"), TEXT("PrintString"));
	Args->SetStringField(TEXT("target"), TEXT("KismetSystemLibrary"));
	auto PosObj = MakeShared<FJsonObject>();
	PosObj->SetNumberField(TEXT("x"), 400);
	PosObj->SetNumberField(TEXT("y"), 0);
	Args->SetObjectField(TEXT("position"), PosObj);
	Result = AddNodeTool.Execute(Args);
	TestTrue(TEXT("Step7: AddNode success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step7: Contains node_id"), MCPTestUtils::GetResultText(Result).Contains(TEXT("GUID-PRINT")));
	TestTrue(TEXT("Step7: Contains 3 pins"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3 pins")));

	// Step 8: Connect graph pins (BeginPlay.then -> PrintString.execute)
	Mock.ConnectGraphPinsResult.bSuccess = true;
	FConnectGraphPinsImplTool ConnectTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("source_node_id"), TEXT("GUID-BEGINPLAY"));
	Args->SetStringField(TEXT("source_pin_name"), TEXT("then"));
	Args->SetStringField(TEXT("target_node_id"), TEXT("GUID-PRINT"));
	Args->SetStringField(TEXT("target_pin_name"), TEXT("execute"));
	Result = ConnectTool.Execute(Args);
	TestTrue(TEXT("Step8: ConnectPins success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step8: Contains Connected"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Connected")));

	// Step 9: Set pin default value
	Mock.SetPinDefaultValueResult.bSuccess = true;
	FSetPinDefaultValueImplTool SetPinTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-PRINT"));
	Args->SetStringField(TEXT("pin_name"), TEXT("InString"));
	Args->SetStringField(TEXT("default_value"), TEXT("Hello from MCP!"));
	Result = SetPinTool.Execute(Args);
	TestTrue(TEXT("Step9: SetPinDefault success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step9: Contains Hello from MCP"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Hello from MCP")));

	// Step 10: Delete graph node
	Mock.DeleteGraphNodeResult.bSuccess = true;
	FDeleteGraphNodeImplTool DeleteNodeTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("blueprint_path"), TEXT("/Game/BP_Test"));
	Args->SetStringField(TEXT("graph_name"), TEXT("EventGraph"));
	Args->SetStringField(TEXT("node_id"), TEXT("GUID-PRINT"));
	Result = DeleteNodeTool.Execute(Args);
	TestTrue(TEXT("Step10: DeleteNode success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Step10: Contains deleted"), MCPTestUtils::GetResultText(Result).Contains(TEXT("deleted")));

	// Verify call counts
	TestEqual(TEXT("GetBlueprintComponents total"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintComponents")), 1);
	TestEqual(TEXT("AddBlueprintComponent total"), Mock.Recorder.GetCallCount(TEXT("AddBlueprintComponent")), 2);
	TestEqual(TEXT("SetBlueprintComponentProperty total"), Mock.Recorder.GetCallCount(TEXT("SetBlueprintComponentProperty")), 1);
	TestEqual(TEXT("GetBlueprintComponentProperty total"), Mock.Recorder.GetCallCount(TEXT("GetBlueprintComponentProperty")), 1);
	TestEqual(TEXT("RemoveBlueprintComponent total"), Mock.Recorder.GetCallCount(TEXT("RemoveBlueprintComponent")), 1);
	TestEqual(TEXT("AddGraphNode total"), Mock.Recorder.GetCallCount(TEXT("AddGraphNode")), 1);
	TestEqual(TEXT("ConnectGraphPins total"), Mock.Recorder.GetCallCount(TEXT("ConnectGraphPins")), 1);
	TestEqual(TEXT("SetPinDefaultValue total"), Mock.Recorder.GetCallCount(TEXT("SetPinDefaultValue")), 1);
	TestEqual(TEXT("DeleteGraphNode total"), Mock.Recorder.GetCallCount(TEXT("DeleteGraphNode")), 1);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
