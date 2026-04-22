// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/GameStateTypes.h"
#include "LevelDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TEAMPOTATO_API ULevelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TMap<EGameState, FLevelData> LevelsByState;

	// ===================================================
	// 레벨 데이터 검색 함수들
	// ===================================================
	UFUNCTION(BlueprintCallable, Category = "Level")
	bool GetLevelDataByState(EGameState State, FLevelData& OutLevelData) const;
};
