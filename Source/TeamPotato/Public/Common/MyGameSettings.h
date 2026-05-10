// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "Common/GameStateTypes.h"
#include "MyGameSettings.generated.h"

class ULevelDataAsset;
class USoundMix;
class USoundClass;
class UUserWidget;
class AEnemyDamagePopupActor;
class UMinimapWidget;
class UMaterialInterface;
class UTexture2D;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Game Settings"))
class TEAMPOTATO_API UMyGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMyGameSettings();

	UFUNCTION(BlueprintPure, Category = "Game Settings")
	static UMyGameSettings* Get();
	
	// 서브시스템에서 접근하기 위한 레벨 데이터 에셋
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Levels")
	TSoftObjectPtr<ULevelDataAsset> LevelDataAsset;

	// 시작 게임 상태
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game State")
	EGameState DefaultStartState = EGameState::MainMenu;

    // 캐릭터 데이터 테이블
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<UDataTable> CharacterDataTable;

    // 사운드 설정
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Sound")
    TSoftObjectPtr<USoundMix> MasterSoundMix;

    // 음악 사운드 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Sound")
    TSoftObjectPtr<USoundClass> MusicSoundClass;

    // 효과음 사운드 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Sound")
    TSoftObjectPtr<USoundClass> SFXSoundClass;

    // 로딩 화면 위젯 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSoftClassPtr<UUserWidget> LoadingWidgetClass;

    // 챕터/게임 클리어 위젯 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSoftClassPtr<UUserWidget> ChapterClearWidgetClass;

    // 데미지 팝업 위젯 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI|Actor")
    TSoftClassPtr<AEnemyDamagePopupActor> EnemyDamagePopupActorClass;

    // 미니맵 머티리얼
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI|Minimap")
    TSoftObjectPtr<UMaterialInterface> MinimapBaseMaterial;

    // 미니맵 플레이어 아이콘
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI|Minimap")
    TSoftObjectPtr<UTexture2D> MinimapPlayerIcon;

    // 미니맵 위젯 클래스
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI|Minimap")
    TSoftClassPtr<UMinimapWidget> MinimapWidget;
};
