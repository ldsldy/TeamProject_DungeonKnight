// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerResource.h"
#include "Player/TestCharacter.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/CharacterSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerResource::UPlayerResource()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}


// Called when the game starts
void UPlayerResource::BeginPlay()
{
    Super::BeginPlay();

    if (UCharacterSubsystem* CharacterSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UCharacterSubsystem>())
    {
        const FPlayerSaveData& CurrentPlayerData = CharacterSubsystem->GetCurrentPlayerData();
        SetMaxHealth(CurrentPlayerData.MaxHealth);
        SetMaxEnergy(CurrentPlayerData.MaxEnergy);
        SetWalkSpeed(CurrentPlayerData.WalkSpeed);
    }
    else
    {
        SetMaxHealth(MaxHealth);
        SetMaxEnergy(MaxEnergy);
        SetWalkSpeed(WalkSpeed);
    }

    CurrentGold = 0;

    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->RegisterPlayerResourceComp(this);
    }

    BroadcastGoldChanged();
}

void UPlayerResource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->UnregisterPlayerResourceComp(this);
    }
    Super::EndPlay(EndPlayReason);
}

void UPlayerResource::SetMaxHealth(float InMaxHealth)
{
    MaxHealth = FMath::Max(MinHealth, InMaxHealth);
    Health = MaxHealth;
    BroadcastHealthChanged();
}

void UPlayerResource::SetMaxEnergy(float InMaxEnergy)
{
    MaxEnergy = FMath::Max(MinEnergy, InMaxEnergy);
    Energy = MaxEnergy;
    BroadcastEnergyChanged();
}

void UPlayerResource::SetWalkSpeed(float InWalkSpeed)
{
    WalkSpeed = FMath::Max(0.0f, InWalkSpeed);

    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
        {
            MovementComp->MaxWalkSpeed = WalkSpeed;
        }
    }

    BroadcastWalkSpeedChanged();
}

void UPlayerResource::PlayerTakeDamage(float InDamage)
{
    Health = FMath::Clamp(Health - InDamage, 0.0f, MaxHealth);

    // 체력 변경 브로드캐스트 시도
    BroadcastHealthChanged();

    UE_LOG(LogTemp, Log, TEXT("Left Health: %f"), Health);
    if (Health <= HealthEpsilon)
    {
        AActor* OwnerCharacter = GetOwner();
        Cast<ATestCharacter>(OwnerCharacter)->KillPlayer();
    }
}

void UPlayerResource::Heal(float InHeal)
{
    Health = FMath::Clamp(Health + InHeal, 0.0f, MaxHealth);
    // 체력 변경 브로드캐스트 시도
    BroadcastHealthChanged();
}

bool UPlayerResource::UseEnergy(float InUseStaminaAmount)
{
    
    if (IsEnergyRemain(InUseStaminaAmount))
    {
        Energy = FMath::Clamp(Energy - InUseStaminaAmount, 0.0f, MaxEnergy);
        UE_LOG(LogTemp, Log, TEXT("Stamina : %f"), Energy);
        // 에너지 변경 브로드캐스트 시도
        BroadcastEnergyChanged();
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No Stamina"));
        return false;
    }
}

void UPlayerResource::AddGold(int32 InGold)
{
    CurrentGold += InGold;
    BroadcastGoldChanged();
}

void UPlayerResource::FillEnergy(float InEnergy)
{
    Energy = FMath::Clamp(Energy + InEnergy, 0.0f, MaxEnergy);
    UE_LOG(LogTemp, Log, TEXT("Stamina : %f"), Energy);
    BroadcastEnergyChanged();
}

void UPlayerResource::AddPower(float InPower)
{
    AttackPower = FMath::Max(MinAttackPower, AttackPower + InPower);
    UE_LOG(LogTemp, Log, TEXT("Power : %f"), AttackPower);
    BroadcastAttackPowerChanged();
}

void UPlayerResource::AddMaxHealth(float InMaxHealth)
{
    SetMaxHealth(MaxHealth + InMaxHealth);
}

void UPlayerResource::AddMaxEnergy(float InMaxStamina)
{
    SetMaxEnergy(MaxEnergy + InMaxStamina);
}

void UPlayerResource::AddWalkSpeed(float InWalkSpeed)
{
    SetWalkSpeed(WalkSpeed + InWalkSpeed);
}

void UPlayerResource::BroadcastScalar(FName Key, float Value)
{
    OnScalarChanged.Broadcast(Key, Value);
}

// 최대 체력이나 현재 체력이 바뀌었을 때 뒤에 넣어서 브로드캐스트 해주는 함수
void UPlayerResource::BroadcastHealthChanged()
{
    BroadcastScalar(PlayerResourceVMFields::HealthCurrent, Health);
    BroadcastScalar(PlayerResourceVMFields::HealthMax, MaxHealth);
}

void UPlayerResource::BroadcastEnergyChanged()
{
    BroadcastScalar(PlayerResourceVMFields::EnergyCurrent, Energy);
    BroadcastScalar(PlayerResourceVMFields::EnergyMax, MaxEnergy);
}

void UPlayerResource::BroadcastGoldChanged()
{
    BroadcastScalar(PlayerResourceVMFields::Gold, static_cast<float>(CurrentGold));
}

void UPlayerResource::BroadcastWalkSpeedChanged()
{
    BroadcastScalar(PlayerResourceVMFields::WalkSpeed, WalkSpeed);
}

void UPlayerResource::BroadcastAttackPowerChanged()
{
    BroadcastScalar(PlayerResourceVMFields::AttackPower, AttackPower);
}
