// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

namespace MCPTestUtils
{
	/** Extract text from a tool result's content array */
	inline FString GetResultText(const TSharedPtr<FJsonObject>& Result)
	{
		if (!Result.IsValid()) return TEXT("");

		const TArray<TSharedPtr<FJsonValue>>* ContentArray;
		if (Result->TryGetArrayField(TEXT("content"), ContentArray) && ContentArray->Num() > 0)
		{
			const TSharedPtr<FJsonObject>* FirstContent;
			if ((*ContentArray)[0]->TryGetObject(FirstContent))
			{
				FString Text;
				if ((*FirstContent)->TryGetStringField(TEXT("text"), Text))
				{
					return Text;
				}
			}
		}
		return TEXT("");
	}

	/** Check if a tool result indicates success (isError == false) */
	inline bool IsSuccess(const TSharedPtr<FJsonObject>& Result)
	{
		if (!Result.IsValid()) return false;
		bool bIsError = false;
		if (Result->TryGetBoolField(TEXT("isError"), bIsError))
		{
			return !bIsError;
		}
		return false;
	}

	/** Check if a tool result indicates error (isError == true) */
	inline bool IsError(const TSharedPtr<FJsonObject>& Result)
	{
		if (!Result.IsValid()) return true;
		bool bIsError = false;
		if (Result->TryGetBoolField(TEXT("isError"), bIsError))
		{
			return bIsError;
		}
		return true;
	}

	/** Create a JSON object with a single string field */
	inline TSharedPtr<FJsonObject> MakeArgs(const FString& Key, const FString& Value)
	{
		TSharedPtr<FJsonObject> Args = MakeShared<FJsonObject>();
		Args->SetStringField(Key, Value);
		return Args;
	}

	/** Create an empty JSON object */
	inline TSharedPtr<FJsonObject> EmptyArgs()
	{
		return MakeShared<FJsonObject>();
	}
}
