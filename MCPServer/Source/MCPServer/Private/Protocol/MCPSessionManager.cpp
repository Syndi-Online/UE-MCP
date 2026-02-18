// Copyright Epic Games, Inc. All Rights Reserved.

#include "Protocol/MCPSessionManager.h"
#include "Misc/Guid.h"

FString FMCPSessionManager::CreateSession()
{
	FString SessionId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
	ActiveSessions.Add(SessionId);
	return SessionId;
}

bool FMCPSessionManager::IsValidSession(const FString& SessionId) const
{
	return ActiveSessions.Contains(SessionId);
}

void FMCPSessionManager::RemoveSession(const FString& SessionId)
{
	ActiveSessions.Remove(SessionId);
}
