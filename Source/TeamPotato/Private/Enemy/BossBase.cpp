// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossBase.h"
#include "Subsystem/MVVMSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABossBase::ABossBase()
{
    BulletPatternComp = CreateDefaultSubobject<UBulletHellComponent>(TEXT("BulletPatternComp"));
    MaxHealth = 300;
    CurrentHealth = MaxHealth;
    Bossmultiple = 5.0;
}

void ABossBase::BeginPlay()
{
    Super::BeginPlay();

    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->RegisterBossActor(this);
    }
    
    if (OnBossSpawn.IsBound())
    {
        OnBossSpawn.Broadcast();
    }

    OnBossHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ABossBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->UnregisterBossActor(this);
    }

    Super::EndPlay(EndPlayReason);
}

float ABossBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage <= 0.0f || CurrentHealth <= 0.0f)
    {
        return 0.0f;
    }

    OnBossHealthChanged.Broadcast(CurrentHealth, MaxHealth);

    if (CurrentPhase == 1 && CurrentHealth <= MaxHealth * 0.5f)
    {
        CurrentPhase = 2;
    }
    return ActualDamage;

}

void ABossBase::OnDie()
{
    Super::OnDie();
    UE_LOG(LogTemp, Log, TEXT("BossIsDie"));

    if (OnBossDie.IsBound())
    {
        OnBossDie.Broadcast();
    }
}

void ABossBase::ExecutePattern(int32 PatternIndex)
{
    UE_LOG(LogTemp, Log, TEXT("BossPattern: %d"), PatternIndex);

    switch (PatternIndex)
    {
    case 0: PatternOne();
        break;
    case 1: PatternTwo();
        break;
    case 2: PatternThree(); 
        break;
    case 3: PatternFour(); 
        break;
    case 4: PatternFive();
        break;
    default: UE_LOG(LogTemp, Warning, TEXT("PatternInvalid")); break;
    }
}

float ABossBase::SetMovementSpeed_Implementation(EEnemySpeed State)
{
    float TargetSpeed = 0.0f;
    UE_LOG(LogTemp, Log, TEXT("State : %d"), (int)State);
    //EEnemySpeed에 따라서 속도 조절
    switch (State)
    {
    case EEnemySpeed::Idle:
        TargetSpeed = 0.0f;
        break;
    case EEnemySpeed::Walking:
        TargetSpeed = 200.0f;
        break;
    case EEnemySpeed::Jumping:
        TargetSpeed = 300.0f;
        break;
    case EEnemySpeed::Sprint:
        TargetSpeed = 500.0f;
        break;
    default:
        TargetSpeed = 100.0f; // 기본값
        break;
    }
    UE_LOG(LogTemp, Log, TEXT("State : %f"), TargetSpeed);

    // 실제 이동 속도 적용
    GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;

    // 변경된 속도 반환
    return TargetSpeed;
}

void ABossBase::PatternOne_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("Pattern1"));

}

void ABossBase::PatternTwo_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("Pattern2"));

}

void ABossBase::PatternThree_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("Pattern3"));

}

void ABossBase::PatternFour_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("Pattern4"));

}

void ABossBase::PatternFive_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("Pattern5"));

}

void ABossBase::FinishCurrentPattern()
{
    if (OnPatternFinished.IsBound())
    {
        OnPatternFinished.Broadcast();
    }
}
