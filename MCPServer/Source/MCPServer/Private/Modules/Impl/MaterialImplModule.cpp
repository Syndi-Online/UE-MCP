// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/MaterialImplModule.h"
#include "MaterialEditingLibrary.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialExpression.h"
#include "Materials/MaterialInterface.h"

FCreateMaterialExpressionResult FMaterialImplModule::CreateMaterialExpression(const FString& MaterialPath, const FString& ExpressionClassName, int32 NodePosX, int32 NodePosY)
{
	FCreateMaterialExpressionResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	// Find the expression class
	FString FullClassName = ExpressionClassName;
	if (!FullClassName.StartsWith(TEXT("MaterialExpression")))
	{
		FullClassName = TEXT("MaterialExpression") + FullClassName;
	}

	UClass* ExprClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *FullClassName));
	if (!ExprClass || !ExprClass->IsChildOf(UMaterialExpression::StaticClass()))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Expression class not found: %s"), *FullClassName);
		return Result;
	}

	UMaterialExpression* Expression = UMaterialEditingLibrary::CreateMaterialExpression(Material, ExprClass, NodePosX, NodePosY);
	if (!Expression)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create material expression");
		return Result;
	}

	Result.bSuccess = true;
	Result.ExpressionName = Expression->GetName();
	Result.ExpressionClass = ExprClass->GetName();
	TArrayView<const TObjectPtr<UMaterialExpression>> AllExpressions = Material->GetExpressions();
	Result.ExpressionIndex = INDEX_NONE;
	for (int32 i = 0; i < AllExpressions.Num(); ++i)
	{
		if (AllExpressions[i] == Expression)
		{
			Result.ExpressionIndex = i;
			break;
		}
	}
	return Result;
}

FDeleteMaterialExpressionResult FMaterialImplModule::DeleteMaterialExpression(const FString& MaterialPath, int32 ExpressionIndex)
{
	FDeleteMaterialExpressionResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();
	if (!Expressions.IsValidIndex(ExpressionIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid expression index: %d (material has %d expressions)"), ExpressionIndex, Expressions.Num());
		return Result;
	}

	UMaterialExpression* Expression = Expressions[ExpressionIndex];
	UMaterialEditingLibrary::DeleteMaterialExpression(Material, Expression);

	Result.bSuccess = true;
	return Result;
}

FConnectExpressionsResult FMaterialImplModule::ConnectMaterialExpressions(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, int32 ToIndex, const FString& ToInput)
{
	FConnectExpressionsResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();

	if (!Expressions.IsValidIndex(FromIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid source expression index: %d"), FromIndex);
		return Result;
	}
	if (!Expressions.IsValidIndex(ToIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid target expression index: %d"), ToIndex);
		return Result;
	}

	bool bConnected = UMaterialEditingLibrary::ConnectMaterialExpressions(
		Expressions[FromIndex], FromOutput,
		Expressions[ToIndex], ToInput);

	if (!bConnected)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to connect material expressions");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FConnectPropertyResult FMaterialImplModule::ConnectMaterialProperty(const FString& MaterialPath, int32 FromIndex, const FString& FromOutput, const FString& PropertyName)
{
	FConnectPropertyResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();
	if (!Expressions.IsValidIndex(FromIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid expression index: %d"), FromIndex);
		return Result;
	}

	// Map string to EMaterialProperty
	EMaterialProperty Property;
	if (PropertyName.Equals(TEXT("BaseColor"), ESearchCase::IgnoreCase))
		Property = MP_BaseColor;
	else if (PropertyName.Equals(TEXT("Metallic"), ESearchCase::IgnoreCase))
		Property = MP_Metallic;
	else if (PropertyName.Equals(TEXT("Specular"), ESearchCase::IgnoreCase))
		Property = MP_Specular;
	else if (PropertyName.Equals(TEXT("Roughness"), ESearchCase::IgnoreCase))
		Property = MP_Roughness;
	else if (PropertyName.Equals(TEXT("Normal"), ESearchCase::IgnoreCase))
		Property = MP_Normal;
	else if (PropertyName.Equals(TEXT("EmissiveColor"), ESearchCase::IgnoreCase))
		Property = MP_EmissiveColor;
	else if (PropertyName.Equals(TEXT("Opacity"), ESearchCase::IgnoreCase))
		Property = MP_Opacity;
	else if (PropertyName.Equals(TEXT("OpacityMask"), ESearchCase::IgnoreCase))
		Property = MP_OpacityMask;
	else if (PropertyName.Equals(TEXT("AmbientOcclusion"), ESearchCase::IgnoreCase))
		Property = MP_AmbientOcclusion;
	else if (PropertyName.Equals(TEXT("WorldPositionOffset"), ESearchCase::IgnoreCase))
		Property = MP_WorldPositionOffset;
	else if (PropertyName.Equals(TEXT("SubsurfaceColor"), ESearchCase::IgnoreCase))
		Property = MP_SubsurfaceColor;
	else if (PropertyName.Equals(TEXT("Refraction"), ESearchCase::IgnoreCase))
		Property = MP_Refraction;
	else if (PropertyName.Equals(TEXT("Anisotropy"), ESearchCase::IgnoreCase))
		Property = MP_Anisotropy;
	else if (PropertyName.Equals(TEXT("Tangent"), ESearchCase::IgnoreCase))
		Property = MP_Tangent;
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown material property: %s. Supported: BaseColor, Metallic, Specular, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, AmbientOcclusion, WorldPositionOffset, SubsurfaceColor, Refraction, Anisotropy, Tangent"), *PropertyName);
		return Result;
	}

	bool bConnected = UMaterialEditingLibrary::ConnectMaterialProperty(
		Expressions[FromIndex], FromOutput, Property);

	if (!bConnected)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to connect expression to property: %s"), *PropertyName);
		return Result;
	}

	Result.bSuccess = true;
	Result.PropertyName = PropertyName;
	return Result;
}

FRecompileMaterialResult FMaterialImplModule::RecompileMaterial(const FString& MaterialPath)
{
	FRecompileMaterialResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	UMaterialEditingLibrary::RecompileMaterial(Material);

	Result.bSuccess = true;
	return Result;
}

FGetMaterialParameterResult FMaterialImplModule::GetMaterialParameter(const FString& MaterialPath, const FString& ParameterName)
{
	FGetMaterialParameterResult Result;

	UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
	if (!MaterialInterface)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	FName ParamName(*ParameterName);

	// Try as UMaterial first
	UMaterial* Material = Cast<UMaterial>(MaterialInterface);
	if (Material)
	{
		// Try scalar
		TArray<FName> ScalarNames;
		UMaterialEditingLibrary::GetScalarParameterNames(Material, ScalarNames);
		if (ScalarNames.Contains(ParamName))
		{
			float Value = UMaterialEditingLibrary::GetMaterialDefaultScalarParameterValue(Material, ParamName);
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Scalar");
			Result.Parameter.Value = FString::Printf(TEXT("%.4f"), Value);
			return Result;
		}

		// Try vector
		TArray<FName> VectorNames;
		UMaterialEditingLibrary::GetVectorParameterNames(Material, VectorNames);
		if (VectorNames.Contains(ParamName))
		{
			FLinearColor Value = UMaterialEditingLibrary::GetMaterialDefaultVectorParameterValue(Material, ParamName);
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Vector");
			Result.Parameter.Value = FString::Printf(TEXT("(R=%.4f, G=%.4f, B=%.4f, A=%.4f)"), Value.R, Value.G, Value.B, Value.A);
			return Result;
		}

		// Try texture
		TArray<FName> TextureNames;
		UMaterialEditingLibrary::GetTextureParameterNames(Material, TextureNames);
		if (TextureNames.Contains(ParamName))
		{
			UTexture* Value = UMaterialEditingLibrary::GetMaterialDefaultTextureParameterValue(Material, ParamName);
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Texture");
			Result.Parameter.Value = Value ? Value->GetPathName() : TEXT("None");
			return Result;
		}

		// Try static switch
		TArray<FName> StaticSwitchNames;
		UMaterialEditingLibrary::GetStaticSwitchParameterNames(Material, StaticSwitchNames);
		if (StaticSwitchNames.Contains(ParamName))
		{
			bool Value = UMaterialEditingLibrary::GetMaterialDefaultStaticSwitchParameterValue(Material, ParamName);
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("StaticSwitch");
			Result.Parameter.Value = Value ? TEXT("true") : TEXT("false");
			return Result;
		}
	}

	// Try as Material Instance
	UMaterialInstanceConstant* Instance = Cast<UMaterialInstanceConstant>(MaterialInterface);
	if (Instance)
	{
		float ScalarValue;
		if (Instance->GetScalarParameterValue(ParamName, ScalarValue))
		{
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Scalar");
			Result.Parameter.Value = FString::Printf(TEXT("%.4f"), ScalarValue);
			return Result;
		}

		FLinearColor VectorValue;
		if (Instance->GetVectorParameterValue(ParamName, VectorValue))
		{
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Vector");
			Result.Parameter.Value = FString::Printf(TEXT("(R=%.4f, G=%.4f, B=%.4f, A=%.4f)"), VectorValue.R, VectorValue.G, VectorValue.B, VectorValue.A);
			return Result;
		}

		UTexture* TextureValue;
		if (Instance->GetTextureParameterValue(ParamName, TextureValue))
		{
			Result.bSuccess = true;
			Result.Parameter.ParameterName = ParameterName;
			Result.Parameter.ParameterType = TEXT("Texture");
			Result.Parameter.Value = TextureValue ? TextureValue->GetPathName() : TEXT("None");
			return Result;
		}
	}

	Result.bSuccess = false;
	Result.ErrorMessage = FString::Printf(TEXT("Parameter not found: %s"), *ParameterName);
	return Result;
}

FSetMaterialInstanceParamResult FMaterialImplModule::SetMaterialInstanceParameter(const FString& InstancePath, const FString& ParameterName, const FString& Value, const FString& ParameterType)
{
	FSetMaterialInstanceParamResult Result;

	UMaterialInstanceConstant* Instance = LoadObject<UMaterialInstanceConstant>(nullptr, *InstancePath);
	if (!Instance)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material Instance not found: %s"), *InstancePath);
		return Result;
	}

	FName ParamName(*ParameterName);

	if (ParameterType.Equals(TEXT("Scalar"), ESearchCase::IgnoreCase))
	{
		float FloatValue = FCString::Atof(*Value);
		bool bSet = UMaterialEditingLibrary::SetMaterialInstanceScalarParameterValue(Instance, ParamName, FloatValue);
		if (!bSet)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Failed to set scalar parameter: %s"), *ParameterName);
			return Result;
		}
	}
	else if (ParameterType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase))
	{
		FLinearColor ColorValue;
		if (!ColorValue.InitFromString(Value))
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Invalid vector value: %s. Expected format: (R=1.0,G=0.0,B=0.0,A=1.0)"), *Value);
			return Result;
		}
		bool bSet = UMaterialEditingLibrary::SetMaterialInstanceVectorParameterValue(Instance, ParamName, ColorValue);
		if (!bSet)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Failed to set vector parameter: %s"), *ParameterName);
			return Result;
		}
	}
	else if (ParameterType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
	{
		UTexture* Texture = LoadObject<UTexture>(nullptr, *Value);
		if (!Texture)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Texture not found: %s"), *Value);
			return Result;
		}
		bool bSet = UMaterialEditingLibrary::SetMaterialInstanceTextureParameterValue(Instance, ParamName, Texture);
		if (!bSet)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Failed to set texture parameter: %s"), *ParameterName);
			return Result;
		}
	}
	else if (ParameterType.Equals(TEXT("StaticSwitch"), ESearchCase::IgnoreCase))
	{
		bool bValue = Value.Equals(TEXT("true"), ESearchCase::IgnoreCase) || Value.Equals(TEXT("1"));
		bool bSet = UMaterialEditingLibrary::SetMaterialInstanceStaticSwitchParameterValue(Instance, ParamName, bValue);
		if (!bSet)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Failed to set static switch parameter: %s"), *ParameterName);
			return Result;
		}
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown parameter type: %s. Supported: Scalar, Vector, Texture, StaticSwitch"), *ParameterType);
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FSetMaterialInstanceParentResult FMaterialImplModule::SetMaterialInstanceParent(const FString& InstancePath, const FString& ParentPath)
{
	FSetMaterialInstanceParentResult Result;

	UMaterialInstanceConstant* Instance = LoadObject<UMaterialInstanceConstant>(nullptr, *InstancePath);
	if (!Instance)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material Instance not found: %s"), *InstancePath);
		return Result;
	}

	UMaterialInterface* NewParent = LoadObject<UMaterialInterface>(nullptr, *ParentPath);
	if (!NewParent)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Parent material not found: %s"), *ParentPath);
		return Result;
	}

	UMaterialEditingLibrary::SetMaterialInstanceParent(Instance, NewParent);

	Result.bSuccess = true;
	return Result;
}

FMaterialStatisticsResult FMaterialImplModule::GetMaterialStatistics(const FString& MaterialPath)
{
	FMaterialStatisticsResult Result;

	UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
	if (!MaterialInterface)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	FMaterialStatistics Stats = UMaterialEditingLibrary::GetStatistics(MaterialInterface);

	Result.bSuccess = true;
	Result.NumVertexShaderInstructions = Stats.NumVertexShaderInstructions;
	Result.NumPixelShaderInstructions = Stats.NumPixelShaderInstructions;
	Result.NumSamplers = Stats.NumSamplers;
	Result.NumVertexTextureSamples = Stats.NumVertexTextureSamples;
	Result.NumPixelTextureSamples = Stats.NumPixelTextureSamples;
	Result.NumVirtualTextureSamples = Stats.NumVirtualTextureSamples;
	Result.NumInterpolatorScalars = Stats.NumInterpolatorScalars;
	return Result;
}

FGetMaterialExpressionsResult FMaterialImplModule::GetMaterialExpressions(const FString& MaterialPath)
{
	FGetMaterialExpressionsResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();
	for (int32 i = 0; i < Expressions.Num(); ++i)
	{
		FMCPMaterialExpressionInfo Info;
		Info.Index = i;
		Info.Name = Expressions[i]->GetName();
		Info.Class = Expressions[i]->GetClass()->GetName();
		Info.PosX = Expressions[i]->MaterialExpressionEditorX;
		Info.PosY = Expressions[i]->MaterialExpressionEditorY;
		Result.Expressions.Add(Info);
	}
	Result.bSuccess = true;
	return Result;
}

FGetMaterialExpressionPropertyResult FMaterialImplModule::GetMaterialExpressionProperty(const FString& MaterialPath, int32 ExpressionIndex, const FString& PropertyName)
{
	FGetMaterialExpressionPropertyResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();
	if (!Expressions.IsValidIndex(ExpressionIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid expression index: %d (material has %d expressions)"), ExpressionIndex, Expressions.Num());
		return Result;
	}

	UMaterialExpression* Expression = Expressions[ExpressionIndex];
	FProperty* Prop = Expression->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Prop)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Property '%s' not found on expression %s"), *PropertyName, *Expression->GetClass()->GetName());
		return Result;
	}

	FString ValueStr;
	const void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(Expression);
	Prop->ExportTextItem_Direct(ValueStr, ValuePtr, nullptr, Expression, PPF_None);

	Result.bSuccess = true;
	Result.PropertyValue = ValueStr;
	return Result;
}

FSetMaterialExpressionPropertyResult FMaterialImplModule::SetMaterialExpressionProperty(const FString& MaterialPath, int32 ExpressionIndex, const FString& PropertyName, const FString& PropertyValue)
{
	FSetMaterialExpressionPropertyResult Result;

	UMaterial* Material = LoadObject<UMaterial>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	TArrayView<const TObjectPtr<UMaterialExpression>> Expressions = Material->GetExpressions();
	if (!Expressions.IsValidIndex(ExpressionIndex))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid expression index: %d (material has %d expressions)"), ExpressionIndex, Expressions.Num());
		return Result;
	}

	UMaterialExpression* Expression = Expressions[ExpressionIndex];
	FProperty* Prop = Expression->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Prop)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Property '%s' not found on expression %s"), *PropertyName, *Expression->GetClass()->GetName());
		return Result;
	}

	void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(Expression);
	const TCHAR* Buffer = *PropertyValue;
	Prop->ImportText_Direct(Buffer, ValuePtr, Expression, PPF_None);

	Expression->Modify();
	Material->PreEditChange(nullptr);
	Material->PostEditChange();

	Result.bSuccess = true;
	return Result;
}
