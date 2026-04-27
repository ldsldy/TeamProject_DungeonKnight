// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UImage;
class UMinimapViewModel;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    //===========================================
    // 미니맵 업데이트 함수
    //===========================================

    // 미니맵 머티리얼 초기화
    UFUNCTION()
    void HandleMinimapInitialized();

    void BindViewModel();
    void UnbindViewModel();

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> MinimapImage;

private:
    // --- 미니맵 뷰모델 ---
    UPROPERTY()
    TObjectPtr<UMinimapViewModel> MinimapViewModel;

    // --- 중복 바인딩 방지 플래그 ---
    bool bIsViewModelBound = false;
};
