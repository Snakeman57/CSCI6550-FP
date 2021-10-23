// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Runtime/Engine/Classes/Kismet/KismetTextLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "NumberBoxC.generated.h"

/**
 * 
 */
UCLASS()
class CSCI6550_FP_API UNumberBoxC : public UEditableTextBox
{
	GENERATED_BODY()
public:
	UNumberBoxC(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
		void HandleOnTextChanged(const FText& t);
	UFUNCTION(BlueprintCallable)
		void HandleOnTextCommitted(const FText& t, ETextCommit::Type cm);
	UPROPERTY(BlueprintReadOnly)
		int32 num;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 min;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 max = INT_MAX;
protected:
	FText last;
};