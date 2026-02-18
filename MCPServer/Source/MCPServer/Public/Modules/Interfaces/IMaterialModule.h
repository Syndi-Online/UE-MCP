// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FCreateMaterialExpressionResult
{
	bool bSuccess = false;
	FString ExpressionName;
	FString ExpressionClass;
	int32 ExpressionIndex = -1;
	FString ErrorMessage;
};

struct FDeleteMaterialExpressionResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FConnectExpressionsResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FConnectPropertyResult
{
	bool bSuccess = false;
	FString PropertyName;
	FString ErrorMessage;
};

struct FRecompileMaterialResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FMCPMaterialParamInfo
{
	FString ParameterName;
	FString ParameterType;
	FString Value;
};

struct FGetMaterialParameterResult
{
	bool bSuccess = false;
	FMCPMaterialParamInfo Parameter;
	FString ErrorMessage;
};

struct FSetMaterialInstanceParamResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FSetMaterialInstanceParentResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FMaterialStatisticsResult
{
	bool bSuccess = false;
	int32 NumVertexShaderInstructions = 0;
	int32 NumPixelShaderInstructions = 0;
	int32 NumSamplers = 0;
	int32 NumVertexTextureSamples = 0;
	int32 NumPixelTextureSamples = 0;
	int32 NumVirtualTextureSamples = 0;
	int32 NumInterpolatorScalars = 0;
	FString ErrorMessage;
};

/**
 * Module interface for Material operations.
 */
class IMaterialModule
{
public:
	virtual ~IMaterialModule() = default;

	virtual FCreateMaterialExpressionResult CreateMaterialExpression(const FString& MaterialPath, const FString& ExpressionClass, int32 NodePosX, int32 NodePosY) = 0;
	virtual FDeleteMaterialExpressionResult DeleteMaterialExpression(const FString& MaterialPath, int32 ExpressionIndex) = 0;
	virtual FConnectExpressionsResult ConnectMaterialExpressions(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, int32 ToIndex, const FString& ToInput) = 0;
	virtual FConnectPropertyResult ConnectMaterialProperty(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, const FString& PropertyName) = 0;
	virtual FRecompileMaterialResult RecompileMaterial(const FString& MaterialPath) = 0;
	virtual FGetMaterialParameterResult GetMaterialParameter(const FString& MaterialPath, const FString& ParameterName) = 0;
	virtual FSetMaterialInstanceParamResult SetMaterialInstanceParameter(const FString& InstancePath, const FString& ParameterName, const FString& Value, const FString& ParameterType) = 0;
	virtual FSetMaterialInstanceParentResult SetMaterialInstanceParent(const FString& InstancePath, const FString& ParentPath) = 0;
	virtual FMaterialStatisticsResult GetMaterialStatistics(const FString& MaterialPath) = 0;
};
