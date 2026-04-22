// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStateTypes.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EGameState : uint8
{
	None			UMETA(DisplayName = "None"),
	MainMenu		UMETA(DisplayName = "Main Menu"),
	Lobby			UMETA(DisplayName = "Lobby"),
	Chapter1		UMETA(DisplayName = "Chapter 1"),
	Chapter2		UMETA(DisplayName = "Chapter 2"),
	Chapter3		UMETA(DisplayName = "Chapter 3"),
	GameOver		UMETA(DisplayName = "Game Over"),
	Victory			UMETA(DisplayName = "Victory")
};

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()
	
	// 로드할 레벨 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LevelAsset;

	// 뷰포트에 표시될 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
};
