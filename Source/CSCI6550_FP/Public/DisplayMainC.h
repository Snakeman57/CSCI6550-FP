// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheWorld.h"
#include "DisplayMainC.generated.h"

/**
 * 
 */
UCLASS()
class CSCI6550_FP_API UDisplayMainC : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void newSim(int lat1, int lat2, int size, int pop);
private:
	TheWorld world;
};
