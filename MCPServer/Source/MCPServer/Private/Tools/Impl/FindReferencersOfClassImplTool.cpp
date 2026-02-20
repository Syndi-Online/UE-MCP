// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/FindReferencersOfClassImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FFindReferencersOfClassImplTool::FFindReferencersOfClassImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FFindReferencersOfClassImplTool::GetName() const
{
	return TEXT("find_referencers_of_class");
}

FString FFindReferencersOfClassImplTool::GetDescription() const
{
	return TEXT("Find all assets that reference a given C++ or Blueprint class");
}

TSharedPtr<FJsonObject> FFindReferencersOfClassImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ClassPathProp = MakeShared<FJsonObject>();
	ClassPathProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassPathProp->SetStringField(TEXT("description"), TEXT("Class path, e.g. /Script/HoldMyWheel.RepairActivity"));
	Properties->SetObjectField(TEXT("class_path"), ClassPathProp);

	TSharedPtr<FJsonObject> PackagePathProp = MakeShared<FJsonObject>();
	PackagePathProp->SetStringField(TEXT("type"), TEXT("string"));
	PackagePathProp->SetStringField(TEXT("description"), TEXT("Limit search to this package path, e.g. /Game/BluePrint"));
	Properties->SetObjectField(TEXT("package_path"), PackagePathProp);

	TSharedPtr<FJsonObject> RecursiveProp = MakeShared<FJsonObject>();
	RecursiveProp->SetStringField(TEXT("type"), TEXT("boolean"));
	RecursiveProp->SetStringField(TEXT("description"), TEXT("Search recursively (default: true)"));
	Properties->SetObjectField(TEXT("recursive"), RecursiveProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("class_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FFindReferencersOfClassImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ClassPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("class_path"), ClassPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: class_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString PackagePath;
	bool bRecursive = true;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("package_path"), PackagePath);
		Arguments->TryGetBoolField(TEXT("recursive"), bRecursive);
	}

	FAssetFindReferencersOfClassResult FindResult = AssetModule.FindReferencersOfClass(ClassPath, PackagePath, bRecursive);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (FindResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Found %d assets referencing '%s':\n"), FindResult.Assets.Num(), *ClassPath);
		for (const FAssetInfo& Info : FindResult.Assets)
		{
			ResponseText += FString::Printf(TEXT("- %s (%s)\n"), *Info.AssetPath, *Info.AssetClass);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed: %s"), *FindResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
