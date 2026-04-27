// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UEnemyViewModel;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()

private:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    void BindViewModel();
    void UnbindViewModel();

    UFUNCTION()
    void SetBossHealthUI(float CurrentHealth, float MaxHealth);

    UFUNCTION()
    void UpdateDelayBar();

    UFUNCTION()
    void StartDelayBarTimer();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float InterpDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float DelayBarWaitTime = 0.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float InterpAlpha = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float DelayBarInterval = 0.02f;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BossMaxHealthText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BossCurrentHealthText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> BossHealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> BossHealthDelayBar;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossWidget")
    TObjectPtr<UCurveFloat> DelayBarCurve;


private:
    // --- 플레이어 상태 뷰모델 ---
    UPROPERTY()
    TObjectPtr<UEnemyViewModel> EnemyViewModel;

    // 중복 바인딩 방지 플래그
    bool bIsViewModelBound = false;

    float CurrentHealthPercent = 1.0f;
    float DelayBarPercent = 1.0f;

    FTimerHandle DelayBarTimerHandle;
    FTimerHandle DelayBarWaitTimerHandle;
};
