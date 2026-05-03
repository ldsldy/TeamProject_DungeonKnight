// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EnumBase.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainWeaponChanged, UWeaponDataAsset*, InNewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSubWeaponChanged, UWeaponDataAsset*, InNewWeapon);

class ATestCharacter;
class AWeaponBase;
class UWeaponDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPOTATO_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void WeaponAttack();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void PickupWeapon(UWeaponDataAsset* WeaponData);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwapWeapon();

    // 플레이어 스폰 시 기본 무기 장착
    void InitializeBaseWeapon(UWeaponDataAsset* InWeaponData);

    // 에너지 부족 시 기본 무기로 전환
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToBaseWeapon();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetCurrentWeaponType() const;
    
    // 현재 활성화된 무기를 가져옴
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    inline AWeaponBase* GetActivateWeapon() { return ActivatedWeapon; }

    UFUNCTION(BlueprintPure, Category = "Weapon")
    UWeaponDataAsset* GetMainWeaponData() const;

    UFUNCTION(BlueprintPure, Category = "Weapon")
    UWeaponDataAsset* GetSubWeaponData() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // 현재 무기 장착
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipCurrentWeapon(AWeaponBase* InWeapon);

    // 서브 무기 장착
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipSubWeapon(AWeaponBase* InWeapon);

    // 무기 교체시 픽업 무기 스폰
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SpawnPickupWeapon(UWeaponDataAsset* WeaponData);

private:
    void BroadcastMainWeaponChanged();
    void BroadcastSubWeaponChanged();

public:
    // --- 델리게이트 ---
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMainWeaponChanged OnMainWeaponChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSubWeaponChanged OnSubWeaponChanged;

protected:
    // 플레이어
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TObjectPtr<ATestCharacter> Owner = nullptr;

    // 현재 무기
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TObjectPtr<AWeaponBase> ActivatedWeapon = nullptr;

    // 기본 무기
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TObjectPtr<AWeaponBase> BaseWeapon = nullptr;

    // 주 무기
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<AWeaponBase> CurrentWeapon = nullptr;

    // 보조 무기
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TObjectPtr<AWeaponBase> SubWeapon = nullptr;

    // 현재 기본 무기를 사용 중인지 여부
    bool bIsUsingBaseWeapon = false;
};
