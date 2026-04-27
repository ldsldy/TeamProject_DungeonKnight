// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWeaponWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;
class UWeaponViewModel;
class UPlayerResourceViewModel;
class UWeaponDataAsset;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPlayerWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();

    // --- 뷰모델 바인딩 함수 ---
    // 플레이어에게 있는 무기 공격 가능 자원 변경 함수
    UFUNCTION()
    void UpdatePlayerResourceBar(float CurrentResource, float MaxResource);

    UFUNCTION()
    void HandleResourceFieldChanged(FName FieldName);

    // 플레이어의 메인 무기 정보 변경 함수 (데이터 에셋 혹은 테이블도 생각)
     UFUNCTION()
    void UpdateMainWeaponInfo(UWeaponDataAsset* InDataAsset);

    UFUNCTION()
    void UpdateSubWeaponInfo(UWeaponDataAsset* InDataAsset);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> WeaponIconImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WeaponName = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SubWeaponIconImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> PlayerResourceBar = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UsageResourceText = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentEnergyText = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxEnergyText = nullptr;

private:
    UPROPERTY()
    TObjectPtr<UWeaponViewModel> WeaponViewModel;

    UPROPERTY()
    TObjectPtr<UPlayerResourceViewModel> PlayerResourceViewModel;

    bool bIsViewModelBound = false;
};
