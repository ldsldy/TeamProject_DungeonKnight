// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkSelectionScreenWidget.generated.h"

// --- 퍽이 선택되었음을 알리기 위한 델리게이트 ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPerkSelected);

class UPerkCardWidget;
class UPerkViewModel;
class UDataTable;
class UPerkDataAsset;
struct FPerkSelectDataTableRow;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPerkSelectionScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
	void SetupPerkCards();

	TArray<FPerkSelectDataTableRow> GetRandomPerkDataRows(UDataTable* InDataTable, int32 NumSelect);
    
    UFUNCTION()
    void HandlePerkSelected(UPerkDataAsset* SelectedPerkData);

    //void BindViewModel();
    //void UnbindViewModel();

public:
    UPROPERTY(BlueprintAssignable, Category = "Perk")
    FOnPerkSelected OnPerkSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPerkCardWidget> PerkCard1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPerkCardWidget> PerkCard2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPerkCardWidget> PerkCard3;

private:
    UPROPERTY()
    TObjectPtr<UPerkViewModel> PerkViewModel;

	UPROPERTY(EditAnywhere, Category = "Perk", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> PerkDataTable;

	UPROPERTY(EditAnywhere, Category = "Perk", meta = (AllowPrivateAccess = "true"))
	int32 PerkNum=3;

    bool bIsViewModelBound = false;
};
