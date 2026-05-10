// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LastBoss.h"
#include "Common/MyGameSettings.h"
#include "TeamPotato/Room/DungeonRoom9.h"
#include "Kismet/GameplayStatics.h"
ALastBoss::ALastBoss()
{
    MaxHealth = 1200;
    CurrentHealth = MaxHealth;
}

void ALastBoss::BeginPlay()
{
    Super::BeginPlay();
}

void ALastBoss::OnDie()
{
    Super::OnDie();

    OnGameClear.Broadcast();
    GetWorldTimerManager().SetTimer(VictoryWidgetTimerHandle, this, &ALastBoss::ShowVictoryWidget, 14.0f, false);
}

float ALastBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}

void ALastBoss::ShowVictoryWidget()
{
    TSubclassOf<UUserWidget> WidgetClass = nullptr;

    if (const UMyGameSettings* GameSettings = UMyGameSettings::Get())
    {
        WidgetClass = GameSettings->ChapterClearWidgetClass.LoadSynchronous();
    }

    if (!WidgetClass)
    {
        WidgetClass = VictoryWidgetClass;
    }

    if (WidgetClass)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            VictoryWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
            if (VictoryWidget)
            {
                VictoryWidget->AddToViewport();

                // 마우스 커서 및 입력 모드 설정
                PC->SetShowMouseCursor(true);
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(VictoryWidget->TakeWidget());
                PC->SetInputMode(InputMode);
            }
        }
    }
}
