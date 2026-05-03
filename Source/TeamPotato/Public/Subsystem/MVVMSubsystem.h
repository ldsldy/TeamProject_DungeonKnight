// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MVVMSubsystem.generated.h"

class ABossBase;
class UEnemyViewModel;
class UMinimapViewModel;
class UPerkComponent;
class UPerkViewModel;
class UPlayerResource;
class UPlayerResourceViewModel;
class UWeaponComponent;
class UWeaponViewModel;

/**
 * Owns shared ViewModel instances and binds only gameplay models that belong to the MVVM layer.
 */
UCLASS()
class TEAMPOTATO_API UMVVMSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    UPlayerResourceViewModel* GetPlayerStatusViewModel();

    UFUNCTION(BlueprintPure)
    UPerkViewModel* GetPerkViewModel();

    UFUNCTION(BlueprintPure)
    UWeaponViewModel* GetWeaponViewModel();

    UFUNCTION(BlueprintPure)
    UMinimapViewModel* GetMinimapViewModel();

    UFUNCTION(BlueprintPure)
    UEnemyViewModel* GetEnemyViewModel();

    UFUNCTION()
    void RegisterPlayerResourceComp(UPlayerResource* NewComp);

    UFUNCTION()
    void UnregisterPlayerResourceComp(UPlayerResource* ExitingComp);

    UFUNCTION()
    void RegisterPerkComp(UPerkComponent* NewComp);

    UFUNCTION()
    void UnregisterPerkComp(UPerkComponent* ExitingComp);

    UFUNCTION()
    void RegisterWeaponComp(UWeaponComponent* NewComp);

    UFUNCTION()
    void UnregisterWeaponComp(UWeaponComponent* ExitingComp);

    UFUNCTION()
    void RegisterBossActor(ABossBase* NewActor);

    UFUNCTION()
    void UnregisterBossActor(ABossBase* ExitingActor);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    UPROPERTY()
    TObjectPtr<UPlayerResourceViewModel> PlayerStatusViewModel;

    UPROPERTY()
    TObjectPtr<UPerkViewModel> PerkViewModel;

    UPROPERTY()
    TObjectPtr<UWeaponViewModel> WeaponViewModel;

    UPROPERTY()
    TObjectPtr<UMinimapViewModel> MinimapViewModel;

    UPROPERTY()
    TObjectPtr<UEnemyViewModel> EnemyViewModel;

    UPROPERTY()
    TObjectPtr<UPlayerResource> RegisteredPlayerResourceComp;

    UPROPERTY()
    TObjectPtr<UPerkComponent> RegisteredPerkComp;

    UPROPERTY()
    TObjectPtr<UWeaponComponent> RegisteredWeaponComp;

    UPROPERTY()
    TObjectPtr<ABossBase> RegisteredBossActor;
};
