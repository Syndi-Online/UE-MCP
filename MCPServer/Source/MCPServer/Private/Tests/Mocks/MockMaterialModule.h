// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IMaterialModule.h"
#include "MockCallRecorder.h"

class FMockMaterialModule : public IMaterialModule
{
public:
	FMockCallRecorder Recorder;

	FCreateMaterialExpressionResult CreateMaterialExpressionResult;
	FDeleteMaterialExpressionResult DeleteMaterialExpressionResult;
	FConnectExpressionsResult ConnectMaterialExpressionsResult;
	FConnectPropertyResult ConnectMaterialPropertyResult;
	FRecompileMaterialResult RecompileMaterialResult;
	FGetMaterialParameterResult GetMaterialParameterResult;
	FSetMaterialInstanceParamResult SetMaterialInstanceParameterResult;
	FSetMaterialInstanceParentResult SetMaterialInstanceParentResult;
	FMaterialStatisticsResult GetMaterialStatisticsResult;

	virtual FCreateMaterialExpressionResult CreateMaterialExpression(const FString& MaterialPath, const FString& ExpressionClass, int32 NodePosX, int32 NodePosY) override { Recorder.RecordCall(TEXT("CreateMaterialExpression")); return CreateMaterialExpressionResult; }
	virtual FDeleteMaterialExpressionResult DeleteMaterialExpression(const FString& MaterialPath, int32 ExpressionIndex) override { Recorder.RecordCall(TEXT("DeleteMaterialExpression")); return DeleteMaterialExpressionResult; }
	virtual FConnectExpressionsResult ConnectMaterialExpressions(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, int32 ToIndex, const FString& ToInput) override { Recorder.RecordCall(TEXT("ConnectMaterialExpressions")); return ConnectMaterialExpressionsResult; }
	virtual FConnectPropertyResult ConnectMaterialProperty(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, const FString& PropertyName) override { Recorder.RecordCall(TEXT("ConnectMaterialProperty")); return ConnectMaterialPropertyResult; }
	virtual FRecompileMaterialResult RecompileMaterial(const FString& MaterialPath) override { Recorder.RecordCall(TEXT("RecompileMaterial")); return RecompileMaterialResult; }
	virtual FGetMaterialParameterResult GetMaterialParameter(const FString& MaterialPath, const FString& ParameterName) override { Recorder.RecordCall(TEXT("GetMaterialParameter")); return GetMaterialParameterResult; }
	virtual FSetMaterialInstanceParamResult SetMaterialInstanceParameter(const FString& InstancePath, const FString& ParameterName, const FString& Value, const FString& ParameterType) override { Recorder.RecordCall(TEXT("SetMaterialInstanceParameter")); return SetMaterialInstanceParameterResult; }
	virtual FSetMaterialInstanceParentResult SetMaterialInstanceParent(const FString& InstancePath, const FString& ParentPath) override { Recorder.RecordCall(TEXT("SetMaterialInstanceParent")); return SetMaterialInstanceParentResult; }
	virtual FMaterialStatisticsResult GetMaterialStatistics(const FString& MaterialPath) override { Recorder.RecordCall(TEXT("GetMaterialStatistics")); return GetMaterialStatisticsResult; }
};
