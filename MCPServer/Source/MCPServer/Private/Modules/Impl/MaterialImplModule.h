// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IMaterialModule.h"

class FMaterialImplModule : public IMaterialModule
{
public:
	virtual FCreateMaterialExpressionResult CreateMaterialExpression(const FString& MaterialPath, const FString& ExpressionClass, int32 NodePosX, int32 NodePosY) override;
	virtual FDeleteMaterialExpressionResult DeleteMaterialExpression(const FString& MaterialPath, int32 ExpressionIndex) override;
	virtual FConnectExpressionsResult ConnectMaterialExpressions(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, int32 ToIndex, const FString& ToInput) override;
	virtual FConnectPropertyResult ConnectMaterialProperty(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, const FString& PropertyName) override;
	virtual FRecompileMaterialResult RecompileMaterial(const FString& MaterialPath) override;
	virtual FGetMaterialParameterResult GetMaterialParameter(const FString& MaterialPath, const FString& ParameterName) override;
	virtual FSetMaterialInstanceParamResult SetMaterialInstanceParameter(const FString& InstancePath, const FString& ParameterName, const FString& Value, const FString& ParameterType) override;
	virtual FSetMaterialInstanceParentResult SetMaterialInstanceParent(const FString& InstancePath, const FString& ParentPath) override;
	virtual FMaterialStatisticsResult GetMaterialStatistics(const FString& MaterialPath) override;
};
