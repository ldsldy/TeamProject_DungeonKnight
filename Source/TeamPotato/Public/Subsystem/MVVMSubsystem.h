// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MVVMSubsystem.generated.h"

class UPlayerResourceViewModel;
class UPerkViewModel;
class UWeaponViewModel;
class UMinimapViewModel;
class UEnemyViewModel;
class UPlayerResource;
class UPerkComponent;
class UWeaponComponent;
class ABossBase;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UMVVMSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// --- 뷰모델 생성 함수 ---
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
	// ==============================================================================
	// 컴포넌트 등록 및 해제 함수들
	// ==============================================================================
	
	// --- 캐릭터, 컴포넌트 등록 및 해제 함수 ---
	UFUNCTION()
	void RegisterPlayerResourceComp(class UPlayerResource* NewComp);
	UFUNCTION()
	void UnregisterPlayerResourceComp(class UPlayerResource* ExitingComp);

	// --- 퍽 컴포넌트 등록 및 해제 함수 ---
	UFUNCTION()
	void RegisterPerkComp(class UPerkComponent* NewComp);
	UFUNCTION()
	void UnregisterPerkComp(class UPerkComponent* ExitingComp);

    // --- 무기 컴포넌트 등록 및 해제 함수 ---
    UFUNCTION()
    void RegisterWeaponComp(class UWeaponComponent* NewComp);
    UFUNCTION()
    void UnregisterWeaponComp(class UWeaponComponent* ExitingComp);

    // --- DungeonGenerator 액터 등록 및 해제 함수 ---
    UFUNCTION()
    void RegisterDungeonGeneratorActor(class ADungeonGanarator* NewActor);
    UFUNCTION()
    void UnregisterDungeonGeneratorActor(class ADungeonGanarator* ExitingActor);

    // --- 보스 액터 등록 및 해제 함수 ---
    UFUNCTION()
    void RegisterBossActor(class ABossBase* NewActor);
    UFUNCTION()
    void UnregisterBossActor(class ABossBase* ExitingActor);

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
};
