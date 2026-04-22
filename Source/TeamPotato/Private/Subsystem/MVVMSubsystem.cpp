// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MVVMSubsystem.h"
#include "Component/PerkComponent.h"
#include "Component/PlayerResource.h"
#include "Component/WeaponComponent.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/PerkViewModel.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "Subsystem/ViewModel/EnemyViewModel.h"
#include "Data/WeaponDataAsset.h"
#include "TeamPotato/Logic/DungeonGanarator.h"
#include "Enemy/BossBase.h"
#include "Player/TestPlayerController.h"

void UMVVMSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UMVVMSubsystem::Deinitialize()
{
    if (PlayerStatusViewModel)
    {
        PlayerStatusViewModel->Deinitialize();
    }

    RegisteredPlayerResourceComp = nullptr;

    Super::Deinitialize();
}


UPlayerResourceViewModel* UMVVMSubsystem::GetPlayerStatusViewModel()
{
	if (!PlayerStatusViewModel)
	{
		PlayerStatusViewModel = NewObject<UPlayerResourceViewModel>(this);
	}	
	return PlayerStatusViewModel;
}

UPerkViewModel* UMVVMSubsystem::GetPerkViewModel()
{
	if (!PerkViewModel)
	{
        PerkViewModel = NewObject<UPerkViewModel>(this);
	}
	return PerkViewModel;
}

UWeaponViewModel* UMVVMSubsystem::GetWeaponViewModel()
{
    if (!WeaponViewModel)
    {
        WeaponViewModel = NewObject<UWeaponViewModel>(this);
    }
    return WeaponViewModel;
}

UMinimapViewModel* UMVVMSubsystem::GetMinimapViewModel()
{
    if (!MinimapViewModel)
    {
        MinimapViewModel = NewObject<UMinimapViewModel>(this);
    }
    return MinimapViewModel;
}

UEnemyViewModel* UMVVMSubsystem::GetEnemyViewModel()
{
    if (!EnemyViewModel)
    {
        EnemyViewModel = NewObject<UEnemyViewModel>(this);
    }
    return EnemyViewModel;
}


// ==============================================================================
// 컴포넌트 등록 및 해제 함수들
// ==============================================================================

void UMVVMSubsystem::RegisterPlayerResourceComp(UPlayerResource* NewComp)
{
	if (!NewComp) return;

    if (RegisteredPlayerResourceComp && RegisteredPlayerResourceComp != NewComp)
    {
        UnregisterPlayerResourceComp(RegisteredPlayerResourceComp);
    }

    RegisteredPlayerResourceComp = NewComp;

	// 뷰모델 가져오기
	UPlayerResourceViewModel* VM = GetPlayerStatusViewModel();
    VM->Initialize(NewComp);
}

void UMVVMSubsystem::UnregisterPlayerResourceComp(UPlayerResource* ExitingComp)
{
    if (PlayerStatusViewModel)
    {
        PlayerStatusViewModel->Deinitialize();
    }

    if (RegisteredPlayerResourceComp == ExitingComp)
    {
        RegisteredPlayerResourceComp = nullptr;
	}
}


void UMVVMSubsystem::RegisterPerkComp(UPerkComponent* NewComp)
{
	if (!NewComp) return;

	UnregisterPerkComp(NewComp);

	// 뷰모델 가져오기
    UPerkViewModel* VM = GetPerkViewModel();

	// Model->ViewModel (장착된 퍽이 바뀌면 뷰모델에 반영)
	NewComp->OnEquipmentUpdated.AddDynamic(VM, &UPerkViewModel::SetPerkDataAsset);

    NewComp->OnPerkEquipmentCleared.AddDynamic(VM, &UPerkViewModel::RequestClearAllPerks);

    // ViewModel->Model (장착 요청시에 컴포넌트에서 장착)
    VM->OnEquipPerkRequest.BindDynamic(NewComp, &UPerkComponent::EquipPerk);
}

void UMVVMSubsystem::UnregisterPerkComp(UPerkComponent* ExitingComp)
{
	if (ExitingComp && PerkViewModel)
	{
		// 델리게이트 언바인딩
		ExitingComp->OnEquipmentUpdated.RemoveDynamic(PerkViewModel, &UPerkViewModel::SetPerkDataAsset);
        ExitingComp->OnPerkEquipmentCleared.RemoveDynamic(PerkViewModel, &UPerkViewModel::RequestClearAllPerks);
        PerkViewModel->OnEquipPerkRequest.Unbind();
	}
}

void UMVVMSubsystem::RegisterWeaponComp(UWeaponComponent* NewComp)
{
    if (!NewComp) return;

    UnregisterWeaponComp(NewComp);

    // 뷰모델 가져오기
    UWeaponViewModel* VM = GetWeaponViewModel();


    // Model->ViewModel (무기가 바뀌면 뷰모델에 반영)
    // 1. 메인 무기 변경
    // 2. 서브 무기 변경
    
    // Model->ViewModel (장착된 무기가 바뀌면 뷰모델에 반영)
    NewComp->OnMainWeaponChanged.AddDynamic(VM, &UWeaponViewModel::SetMainWeapon);
    NewComp->OnSubWeaponChanged.AddDynamic(VM, &UWeaponViewModel::SetSubWeapon);
}

void UMVVMSubsystem::UnregisterWeaponComp(UWeaponComponent* ExitingComp)
{
    if (ExitingComp && WeaponViewModel)
    {
        // 델리게이트 언바인딩
        ExitingComp->OnMainWeaponChanged.RemoveDynamic(WeaponViewModel, &UWeaponViewModel::SetMainWeapon);
        ExitingComp->OnSubWeaponChanged.RemoveDynamic(WeaponViewModel, &UWeaponViewModel::SetSubWeapon);
    }
}

void UMVVMSubsystem::RegisterDungeonGeneratorActor(ADungeonGanarator* NewActor)
{
    if (!NewActor) return;

    UnregisterDungeonGeneratorActor(NewActor);

    // 뷰모델 가져오기
    UMinimapViewModel* VM = GetMinimapViewModel();

    // DungeonGenerator -> ViewModel -> 던전 생성 완료 알림 뿌림
    NewActor->OnDungeonGenerationCompleted.AddDynamic(VM, &UMinimapViewModel::RequestMinimapCapture);

    // 플레이어 컨트롤러 가져오기 
    //APlayerController* PC = GetWorld()->GetFirstPlayerController();
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());

    NewActor->OnStageAndChapterChanged.AddDynamic(PC, &ATestPlayerController::TryPerkSelectionScreen);
}

void UMVVMSubsystem::UnregisterDungeonGeneratorActor(ADungeonGanarator* ExitingActor)
{
    if (ExitingActor && MinimapViewModel)
    {
        // 델리게이트 언바인딩
        ExitingActor->OnDungeonGenerationCompleted.RemoveDynamic(MinimapViewModel, &UMinimapViewModel::RequestMinimapCapture);
    }
}

void UMVVMSubsystem::RegisterBossActor(ABossBase* NewActor)
{
    if (!NewActor) return;

    UnregisterBossActor(NewActor);

    // 뷰모델 가져오기
    UEnemyViewModel* VM = GetEnemyViewModel();

    // BossActor -> ViewModel -> 보스 스폰 알림 뿌림
    NewActor->OnBossHealthChanged.AddDynamic(VM, &UEnemyViewModel::SetBossHealth);
}

void UMVVMSubsystem::UnregisterBossActor(ABossBase* ExitingActor)
{
    if (ExitingActor && EnemyViewModel)
    {
        // 델리게이트 언바인딩
        ExitingActor->OnBossHealthChanged.RemoveDynamic(EnemyViewModel, &UEnemyViewModel::SetBossHealth);
    }
}

