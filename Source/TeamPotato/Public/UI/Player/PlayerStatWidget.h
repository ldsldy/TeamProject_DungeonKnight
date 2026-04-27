// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;
class UPlayerResourceViewModel;

UCLASS()
class TEAMPOTATO_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();

	/// --- 뷰모델 바인딩 함수 ---
	UFUNCTION()
	void HandleViewModelFieldChanged(FName FieldName);

    void RefreshHealthUI();

    UFUNCTION()
    void UpdatePlayerDelayBar();

    UFUNCTION()
    void StartDelayBarTimer();

///==================== 변수 ======================== 
protected:
    // ==========================================
    // --- 딜레이 바 관련 변수 ---
    // ==========================================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float InterpDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float DelayBarWaitTime = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float InterpAlpha = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    float DelayBarInterval = 0.02f;

    /// --- 딜레이 바 커브 ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DelayBar")
    TObjectPtr<UCurveFloat> DelayBarHealthCurve = nullptr;

    // ==========================================
    // --- 위젯 바인딩 함수 ---
    // ==========================================
	// --- 플레이어의 체력 프로그레스바---
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> DelayProgressBar;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> HealthText;

private:
    // --- 플레이어 상태 뷰모델 ---
	UPROPERTY()
	TObjectPtr<UPlayerResourceViewModel> PlayerStatusViewModel;

    // 중복 바인딩 방지 플래그
    bool bIsViewModelBound = false;

    float CurrentHealthPercent = 1.0f;
    float DelayBarPercent = 1.0f;

    FTimerHandle DelayBarTimerHandle;
    FTimerHandle DelayBarWaitTimerHandle;
};
