// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ViewModel/PGViewModelBase.h"
#include "WeaponViewModel.generated.h"

class UWeaponComponent;
class UWeaponDataAsset;

/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UWeaponViewModel : public UPGViewModelBase
{
    GENERATED_BODY()

public:
    virtual void Initialize(UObject* InModel) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure)
    UWeaponDataAsset* GetMainWeaponData() const { return MainWeaponData; }

    UFUNCTION(BlueprintPure)
    UWeaponDataAsset* GetSubWeaponData() const { return SubWeaponData; }

private:
    UFUNCTION()
    void HandleMainWeaponChanged(UWeaponDataAsset* InWeaponData);

    UFUNCTION()
    void HandleSubWeaponChanged(UWeaponDataAsset* InWeaponData);

private:
    UPROPERTY()
    TObjectPtr<UWeaponComponent> Model = nullptr;

    UPROPERTY()
    UWeaponDataAsset* MainWeaponData = nullptr;

    UPROPERTY()
    UWeaponDataAsset* SubWeaponData = nullptr;
};
