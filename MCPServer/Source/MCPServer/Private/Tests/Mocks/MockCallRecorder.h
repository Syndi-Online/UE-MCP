// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Records mock method calls for verification in unit tests.
 */
struct FMockCall
{
	FString MethodName;
	TSharedPtr<FJsonObject> Args;
};

class FMockCallRecorder
{
public:
	void RecordCall(const FString& Method)
	{
		FMockCall Call;
		Call.MethodName = Method;
		Calls.Add(Call);
	}

	void RecordCall(const FString& Method, const TSharedPtr<FJsonObject>& Args)
	{
		FMockCall Call;
		Call.MethodName = Method;
		Call.Args = Args;
		Calls.Add(Call);
	}

	int32 GetCallCount(const FString& Method) const
	{
		int32 Count = 0;
		for (const FMockCall& Call : Calls)
		{
			if (Call.MethodName == Method)
			{
				Count++;
			}
		}
		return Count;
	}

	const FMockCall* GetLastCall(const FString& Method) const
	{
		for (int32 i = Calls.Num() - 1; i >= 0; --i)
		{
			if (Calls[i].MethodName == Method)
			{
				return &Calls[i];
			}
		}
		return nullptr;
	}

	int32 GetTotalCallCount() const { return Calls.Num(); }

	void Reset() { Calls.Empty(); }

	TArray<FMockCall> Calls;
};
