// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UPlayerStatWidget;
class UInventoryPerkTileWidget;
class UPlayerWeaponWidget;
class UPlayerGoldWidget;
class UBossWidget;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// --- 플레이어 스탯 위젯 ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerStatWidget> PlayerStatPanel;

    // --- 인벤토리 퍽 위젯 ---
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInventoryPerkTileWidget> InventoryPerkPanel;

    // --- 플레이어 무기 위젯 ---
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPlayerWeaponWidget> PlayerWeaponPanel;

    // --- 플레이어 골드 위젯 ---
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPlayerGoldWidget> PlayerGoldPanel;

    // --- 보스 위젯 ---
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBossWidget> BossWidget;
};
