// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/MCPToolRegistry.h"
#include "Tools/Interfaces/IMCPTool.h"
#include "Dom/JsonValue.h"

void FMCPToolRegistry::RegisterTool(TSharedPtr<IMCPTool> Tool)
{
	if (Tool.IsValid())
	{
		Tools.Add(Tool->GetName(), Tool);
	}
}

TArray<TSharedPtr<FJsonValue>> FMCPToolRegistry::GetToolsList() const
{
	TArray<TSharedPtr<FJsonValue>> ToolsArray;

	for (const auto& Pair : Tools)
	{
		const TSharedPtr<IMCPTool>& Tool = Pair.Value;

		TSharedPtr<FJsonObject> ToolObj = MakeShared<FJsonObject>();
		ToolObj->SetStringField(TEXT("name"), Tool->GetName());
		ToolObj->SetStringField(TEXT("description"), Tool->GetDescription());

		TSharedPtr<FJsonObject> InputSchema = Tool->GetInputSchema();
		if (InputSchema.IsValid())
		{
			ToolObj->SetObjectField(TEXT("inputSchema"), InputSchema);
		}

		ToolsArray.Add(MakeShared<FJsonValueObject>(ToolObj));
	}

	return ToolsArray;
}

TSharedPtr<FJsonObject> FMCPToolRegistry::CallTool(const FString& ToolName, const TSharedPtr<FJsonObject>& Arguments) const
{
	const TSharedPtr<IMCPTool>* FoundTool = Tools.Find(ToolName);
	if (!FoundTool || !FoundTool->IsValid())
	{
		return nullptr;
	}

	return (*FoundTool)->Execute(Arguments);
}

bool FMCPToolRegistry::HasTool(const FString& ToolName) const
{
	return Tools.Contains(ToolName);
}
