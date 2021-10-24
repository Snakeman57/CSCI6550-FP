// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheWorld.h"
#include <Runtime\Core\Public\Misc\Paths.h>
#include "DisplayMainC.generated.h"

USTRUCT(BlueprintType)
struct FSimStats {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		int locations; // number of locations
	UPROPERTY(BlueprintReadOnly)
		int lat1;
	UPROPERTY(BlueprintReadOnly)
		int lat2; // lattitude range
	UPROPERTY(BlueprintReadOnly)
		TArray<int> biomes; // number of each biomme
	UPROPERTY(BlueprintReadOnly)
		float avgSupplyW; // average supply value
	UPROPERTY(BlueprintReadOnly)
		int extant; // number of living Peoples
	UPROPERTY(BlueprintReadOnly)
		int extinct; // number of dead Peoples
	UPROPERTY(BlueprintReadOnly)
		float avgSupplyP; // average supply value
	FSimStats operator =(WorldInfo& other);
	FSimStats operator =(PopInfo& other);
};
UCLASS()
class CSCI6550_FP_API UDisplayMainC : public UUserWidget
{
	GENERATED_BODY()
public:
	UDisplayMainC(const FObjectInitializer& ObjectInitializer);
	~UDisplayMainC();
	UFUNCTION(BlueprintCallable)
		void newSim(int lat1, int lat2, int size, int pop);
	UFUNCTION(BlueprintCallable)
		FSimStats getStats();
protected:

private:
	TheWorld* world;
};
