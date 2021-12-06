// Copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheWorld.h"
#include <Runtime\Core\Public\Misc\Paths.h>
#include "DisplayMainC.generated.h"

USTRUCT(BlueprintType)
struct FSimStats { // struct for simulation stats
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		int locations; // number of locations
	UPROPERTY(BlueprintReadOnly)
		int lat1; // lower lattitude bound
	UPROPERTY(BlueprintReadOnly)
		int lat2; // upper lattitude bound
	UPROPERTY(BlueprintReadOnly)
		TArray<int> biomes; // number of each biome
	UPROPERTY(BlueprintReadOnly)
		TArray<float> pops; // percent Location population per biome
	UPROPERTY(BlueprintReadOnly)
		TArray<float> supplies; // average Location supply per biome
	UPROPERTY(BlueprintReadOnly)
		float avgSupplyW; // average supply value
	UPROPERTY(BlueprintReadOnly)
		TArray<float> traits; // average trait value
	UPROPERTY(BlueprintReadOnly)
		int extant; // number of living Peoples
	UPROPERTY(BlueprintReadOnly)
		int extinct; // number of dead Peoples
	UPROPERTY(BlueprintReadOnly)
		float avgSupplyP; // average supply value
	UPROPERTY(BlueprintReadOnly)
		int64 avgPop; // average population value
	UPROPERTY(BlueprintReadOnly)
		TArray<int64> avgPopB; // average People population value per biome
	UPROPERTY(BlueprintReadOnly)
		TArray<float> avgSupplyB; // average People supply value per biome
	UPROPERTY(BlueprintReadOnly)
		int wars; // total wars
	UPROPERTY(BlueprintReadOnly)
		int trades; // total trades
	UPROPERTY(BlueprintReadOnly)
		int coastPpls; // people living in coastal areas
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
		void newSim(const int& lat1, const int& lat2, const int& size, const int& pop);
	UFUNCTION(BlueprintCallable)
		FSimStats getStats();
	UFUNCTION(BlueprintCallable)
		void runSim();
protected:

private:
	TheWorld* world;
};
