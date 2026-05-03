// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WeaponComponent.h"
#include "Player/TestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Weapon/GunWeaponActor.h"
#include "Data/WeaponDataAsset.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->RegisterWeaponComp(this);
    }

	Owner = Cast<ATestCharacter>(GetOwner());
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetActivateWeapon()->Destroy();

    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->UnregisterWeaponComp(this);
    }

    Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::InitializeBaseWeapon(UWeaponDataAsset* InWeaponData)
{
    if (InWeaponData && InWeaponData->WeaponClass)
    {
        BaseWeapon = GetWorld()->SpawnActor<AWeaponBase>(InWeaponData->WeaponClass);
        if (BaseWeapon)
        {
            BaseWeapon->InitializeFromData(InWeaponData);

            BaseWeapon->SetOwner(Owner);
            BaseWeapon->SetOwnerComponent(this);

            // 만약 게임 시작 시 무기가 아예 없다면 기본 무기를 장착
            if (!CurrentWeapon)
            {
                if (BaseWeapon->GetWeaponType() == EWeaponType::Sword)
                {
                    BaseWeapon->AttachToComponent(
                        Owner->GetMesh(),
                        FAttachmentTransformRules::SnapToTargetIncludingScale,
                        TEXT("Weapon_Sword")
                    );
                }
                else if (BaseWeapon->GetWeaponType() == EWeaponType::Gun)
                {
                    BaseWeapon->AttachToComponent(
                        Owner->GetMesh(),
                        FAttachmentTransformRules::SnapToTargetIncludingScale,
                        TEXT("Weapon_Gun")
                    );
                }
                ActivatedWeapon = BaseWeapon;
                Owner->SetPlayerActivatedWeapon(BaseWeapon->GetWeaponType());
                BroadcastMainWeaponChanged();
            }
        }
    }
}

void UWeaponComponent::WeaponAttack()
{
	if (!CurrentWeapon && !BaseWeapon) return;

	if (CurrentWeapon && !bIsUsingBaseWeapon)
	{
		CurrentWeapon->Attack(Owner);
	}
    else
    {
        BaseWeapon->Attack(Owner);
    }
}

void UWeaponComponent::EquipCurrentWeapon(AWeaponBase* InWeapon)
{
    UE_LOG(LogTemp, Log, TEXT("EquipCurrentWeapon called"));

    BaseWeapon->SetActorHiddenInGame(true);
    BaseWeapon->SetActorEnableCollision(false);

    if (!InWeapon || !Owner) return;

    CurrentWeapon = InWeapon;

    Owner->SetPlayerActivatedWeapon(CurrentWeapon->GetWeaponType());
    CurrentWeapon->SetActorHiddenInGame(false);
    CurrentWeapon->SetActorEnableCollision(true);
    CurrentWeapon->SetOwner(Owner);
    CurrentWeapon->SetOwnerComponent(this);

    if (CurrentWeapon->GetWeaponType() == EWeaponType::Sword)
    {
        CurrentWeapon->AttachToComponent(
            Owner->GetMesh(),
            FAttachmentTransformRules::SnapToTargetIncludingScale,
            TEXT("Weapon_Sword")
        );
    }
    else if (CurrentWeapon->GetWeaponType() == EWeaponType::Gun)
    {
        CurrentWeapon->AttachToComponent(
            Owner->GetMesh(),
            FAttachmentTransformRules::SnapToTargetIncludingScale,
            TEXT("Weapon_Gun")
        );
    }

    ActivatedWeapon = CurrentWeapon;

    // 메인 무기 변경 브로드캐스트
    BroadcastMainWeaponChanged();
}

void UWeaponComponent::EquipSubWeapon(AWeaponBase* InWeapon)
{
    if (!InWeapon) return;

    SubWeapon = InWeapon;
    SubWeapon->SetActorHiddenInGame(true);
    SubWeapon->SetActorEnableCollision(false);

    // 서브 무기 변경 브로드캐스트
    BroadcastSubWeaponChanged();
}

void UWeaponComponent::PickupWeapon(UWeaponDataAsset* WeaponData)
{
    if (!Owner || !WeaponData || !WeaponData->WeaponClass) return;

    AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponData->WeaponClass);
    if (!NewWeapon) return;

    NewWeapon->InitializeFromData(WeaponData);

    if (!CurrentWeapon) // 현재 무기 없음
    {
        EquipCurrentWeapon(NewWeapon);
    }
    else if (!SubWeapon) // 서브 무기 없음
    {
        EquipSubWeapon(NewWeapon);
    }
    else if (!bIsUsingBaseWeapon)
    {
        // 둘 다 있을 때만 현재 무기를 버리고 교체
        AWeaponBase* OldWeapon = CurrentWeapon;
        EquipCurrentWeapon(NewWeapon);
        SpawnPickupWeapon(OldWeapon->GetWeaponData());
        OldWeapon->Destroy();
    }
    // 기본 무기 상태
    else
    {
        // 기본 무기는 숨기고 새 무기로 교체 (기본 무기는 파괴하거나 드롭하지 않음)
        BaseWeapon->SetActorHiddenInGame(true);
        BaseWeapon->SetActorEnableCollision(false);

        AWeaponBase* OldWeapon = CurrentWeapon;
        EquipCurrentWeapon(NewWeapon);
        SpawnPickupWeapon(OldWeapon->GetWeaponData());
        OldWeapon->Destroy();

        bIsUsingBaseWeapon = false;
    }
}

void UWeaponComponent::SwapWeapon()
{
    if (!CurrentWeapon || !SubWeapon) return;
    bIsUsingBaseWeapon = false;

    AWeaponBase* Temp = CurrentWeapon;

    EquipCurrentWeapon(SubWeapon);

    EquipSubWeapon(Temp);
}

void UWeaponComponent::SpawnPickupWeapon(UWeaponDataAsset* WeaponData)
{
    if (!WeaponData || !WeaponData->PickupWeaponClass || !Owner) return;

    FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AWeaponPickupActor* Pickup = GetWorld()->SpawnActor<AWeaponPickupActor>(
        WeaponData->PickupWeaponClass,
        SpawnLocation,
        SpawnRotation
    );

    if (Pickup)
    {
        Pickup->SetWeaponData(WeaponData);
    }
}

void UWeaponComponent::SwitchToBaseWeapon()
{
    if (!BaseWeapon || bIsUsingBaseWeapon) return;

    // 기존 무기(CurrentWeapon)가 있다면 숨기기만 함
    if (CurrentWeapon && CurrentWeapon != BaseWeapon)
    {
        CurrentWeapon->SetActorHiddenInGame(true);
        CurrentWeapon->SetActorEnableCollision(false);
    }

    bIsUsingBaseWeapon = true;
    ActivatedWeapon = BaseWeapon;

    BaseWeapon->SetActorHiddenInGame(false);
    BaseWeapon->SetActorEnableCollision(true);

    Owner->SetPlayerActivatedWeapon(BaseWeapon->GetWeaponType());
    BroadcastMainWeaponChanged();
}

EWeaponType UWeaponComponent::GetCurrentWeaponType() const
{
	return ActivatedWeapon ? ActivatedWeapon->GetWeaponType() : EWeaponType::None;
}

UWeaponDataAsset* UWeaponComponent::GetMainWeaponData() const
{
    return ActivatedWeapon ? ActivatedWeapon->GetWeaponData() : nullptr;
}

UWeaponDataAsset* UWeaponComponent::GetSubWeaponData() const
{
    return SubWeapon ? SubWeapon->GetWeaponData() : nullptr;
}


void UWeaponComponent::BroadcastMainWeaponChanged()
{
    if (OnMainWeaponChanged.IsBound())
    {
        OnMainWeaponChanged.Broadcast(ActivatedWeapon->GetWeaponData());
    }
}

void UWeaponComponent::BroadcastSubWeaponChanged()
{
    if (OnSubWeaponChanged.IsBound())
    {
        OnSubWeaponChanged.Broadcast(SubWeapon->GetWeaponData());
    }
}
