// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGoldWidget.generated.h"


class UTextBlock;
class UPlayerResourceViewModel;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPlayerGoldWidget : public UUserWidget
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

    void UpdatePlayerGold();
    
    ///=================================================
    ///==================== 변수 ======================= 
    ///=================================================
protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerGoldText;

private:
    // --- 골드 변화를 알려줄 플레이어 리소스 뷰모델 ---
    UPROPERTY()
    TObjectPtr<UPlayerResourceViewModel> PlayerResourceViewModel;

    bool bIsViewModelBound = false;
};
