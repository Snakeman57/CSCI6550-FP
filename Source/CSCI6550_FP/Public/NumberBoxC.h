// Copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Runtime/Engine/Classes/Kismet/KismetTextLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "NumberBoxC.generated.h"

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
	UFUNCTION(BlueprintCallable)
		void reset();
	UPROPERTY(BlueprintReadOnly)
		int32 num;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 dflt; // default value for resetting
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 min;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 max;
protected:
	FText last;
};
