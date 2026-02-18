// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IActorModule;

/**
 * MCP Tool: delete_actor
 * Deletes an actor from the editor world by name, label, or path.
 */
class FDeleteActorImplTool : public IMCPTool
{
public:
	explicit FDeleteActorImplTool(IActorModule& InActorModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IActorModule& ActorModule;
};
