// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PoolingSubsystem.h"
#include "Common/MyGameSettings.h"
#include "Enemy/DamagePopup/EnemyDamagePopupActor.h"

void UPoolingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 데미지 팝업 위젯 풀 초기화
    InitializeDamagePopupActorPool();
}

void UPoolingSubsystem::Deinitialize()
{
    // 데미지 팝업 액터 풀링 배열 정리
    for (AEnemyDamagePopupActor* Actor : AvailableDamagePopupActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }

    Super::Deinitialize();
}

void UPoolingSubsystem::InitializeDamagePopupActorPool()
{
    // 데미지 팝업 액터 풀링 배열 초기화
    AvailableDamagePopupActors.Reserve(DamagePopupActorPoolSize);

    // 데미지 팝업 클래스 가져오기
    UMyGameSettings* GameSettings = UMyGameSettings::Get();
    TSubclassOf<AEnemyDamagePopupActor> DamagePopupWidgetClass = GameSettings->EnemyDamagePopupActorClass.LoadSynchronous();

    // 데미지 팝업 액터 풀링 초기화
    while(AvailableDamagePopupActors.Num() < DamagePopupActorPoolSize)
    {
        AEnemyDamagePopupActor* DamagePopupActor 
            = GetWorld()->SpawnActor<AEnemyDamagePopupActor>(DamagePopupWidgetClass);
        if (DamagePopupActor)
        {
            // 보이지 않게 설정하고 사용 가능 배열에 추가
            AvailableDamagePopupActors.Add(DamagePopupActor);
        }
    }
}

void UPoolingSubsystem::GetPooledDamagePopupActor(float DamageAmount, FVector Location)
{
    if(AvailableDamagePopupActors.Num() <= 0)
    {
        return;
    }
    if (AvailableDamagePopupActors.Num() > 0)
    {
        // 사용 가능한 액터가 있으면 가져오기
        AEnemyDamagePopupActor* PooledActor = AvailableDamagePopupActors.Pop();
        UsedDamagePopupActor.Add(PooledActor);
        PooledActor->ShowDamagePopup(DamageAmount, Location);
    }
}

void UPoolingSubsystem::ReturnDamagePopupActorToPool(AEnemyDamagePopupActor* DamagePopupActor)
{
    if (DamagePopupActor)
    {
        // 사용 중인 배열에서 제거하고 사용 가능 배열에 다시 추가했을때, 풀 사이즈보다 작은 경우에만 반환
        UsedDamagePopupActor.Remove(DamagePopupActor);

        if (AvailableDamagePopupActors.Num() < DamagePopupActorPoolSize)
        {
            AvailableDamagePopupActors.Add(DamagePopupActor);
        }
        else
        {
            // 풀 사이즈보다 많은 경우 액터를 파괴
            DamagePopupActor->Destroy();
        }
    }
}
