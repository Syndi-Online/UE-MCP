// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IActorModule;

/**
 * MCP Tool: spawn_actor
 * Spawns an actor in the editor world from a class path.
 */
class FSpawnActorImplTool : public IMCPTool
{
public:
	explicit FSpawnActorImplTool(IActorModule& InActorModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IActorModule& ActorModule;
};
