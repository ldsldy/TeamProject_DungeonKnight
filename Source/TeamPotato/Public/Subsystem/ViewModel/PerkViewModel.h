// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ViewModel/PGViewModelBase.h"
#include "PerkViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerkEquipped, UPerkDataAsset*, NewPerkDataAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTryAllPerkClear);

class UPerkDataAsset;
class UPerkComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UPerkViewModel : public UPGViewModelBase
{
    GENERATED_BODY()

public:
    virtual void Initialize(UObject* InModel) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable)
    void SetPerkDataAsset(UPerkDataAsset* NewData);

    UFUNCTION(BlueprintCallable)
    bool RequestEquipPerk(UPerkDataAsset* NewData, int32 SlotIndex = -1);

    UFUNCTION(BlueprintCallable)
    void RequestClearAllPerks();

    UFUNCTION(BlueprintPure)
    UPerkDataAsset* GetEquippedPerkAt(int32 Index) const;

    const TArray<TObjectPtr<UPerkDataAsset>>& GetEquippedPerks() const { return CachedEquippedPerks; }

public:
    UPROPERTY(BlueprintAssignable)
    FOnPerkEquipped OnPerkEquipped;

    UPROPERTY(BlueprintAssignable)
    FTryAllPerkClear OnTryAllPerkClear;

private:
    UFUNCTION()
    void HandleModelPerkCleared();

    void RefreshCachedPerksFromModel();

private:
    UPROPERTY()
    TObjectPtr<UPerkComponent> Model = nullptr;

    UPROPERTY()
    TArray<TObjectPtr<UPerkDataAsset>> CachedEquippedPerks;
};
