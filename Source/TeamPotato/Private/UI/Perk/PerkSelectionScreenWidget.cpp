// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Perk/PerkSelectionScreenWidget.h"
#include "Data/PerkDataTableRow.h"
#include "UI/Perk/PerkCardWidget.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/PerkViewModel.h"

void UPerkSelectionScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (!PerkViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                PerkViewModel = Subsystem->GetPerkViewModel();
            }
        }
    }

	// Add Viewport마다 랜덤한 3개의 퍽 데이터를 가져와서 카드에 세팅
    SetupPerkCards();
}

void UPerkSelectionScreenWidget::NativeDestruct()
{
    if (PerkCard1)
    {
        PerkCard1->OnPerkCardSelected.RemoveDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
    }
    if (PerkCard2)
    {
        PerkCard2->OnPerkCardSelected.RemoveDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
    }
    if (PerkCard3)
    {
        PerkCard3->OnPerkCardSelected.RemoveDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
    }

    Super::NativeDestruct();
}

void UPerkSelectionScreenWidget::SetupPerkCards()
{
	TArray<FPerkSelectDataTableRow> SelectedPerkRows = GetRandomPerkDataRows(PerkDataTable, PerkNum);
	
    // 카드에 정보를 등록하고 카드 선택시에 델리게이트 바인딩
	if (PerkCard1 && SelectedPerkRows.IsValidIndex(0))
	{
        PerkCard1->SetupPerkCard(SelectedPerkRows[0].PerkDataAsset);
        PerkCard1->OnPerkCardSelected.AddDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
	}
    if (PerkCard2 && SelectedPerkRows.IsValidIndex(1))
    {
        PerkCard2->SetupPerkCard(SelectedPerkRows[1].PerkDataAsset);
        PerkCard2->OnPerkCardSelected.AddDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
    }
    if (PerkCard3 && SelectedPerkRows.IsValidIndex(2))
    {
        PerkCard3->SetupPerkCard(SelectedPerkRows[2].PerkDataAsset);
        PerkCard3->OnPerkCardSelected.AddDynamic(this, &UPerkSelectionScreenWidget::HandlePerkSelected);
    }
}

//
// -- Efraimidis-Spirakis 알고르즘 --
// Score = Random(0, 1) ^ (1/Weight)으로 각 행에 대한 점수를 계산하여 상위 K를 선택
//
TArray<FPerkSelectDataTableRow> UPerkSelectionScreenWidget::GetRandomPerkDataRows(UDataTable* InDataTable, int32 NumSelect)
{
	TArray<FPerkSelectDataTableRow> ResultRows;

	// 데이터 테이블의 SelectWeight를 기반으로 랜덤한 3개의 퍽 데이터를 선택하여 버튼에 세팅
	if (!InDataTable || NumSelect <= 0) return ResultRows;

	TArray<FPerkSelectDataTableRow*> AllRows;
	InDataTable->GetAllRows<FPerkSelectDataTableRow>(TEXT("Get PerkSelectDataTable"), AllRows);

	if (AllRows.IsEmpty()) return ResultRows;

	// 모든 행이 뽑고 싶은 개수 미만일 경우, 모든 행을 반환하고 종료
	if (AllRows.Num() < NumSelect)
	{
		for (const FPerkSelectDataTableRow* Row : AllRows)
		{
			ResultRows.Add(*Row);
			return ResultRows;
		}
	}

	// 점수 계산  [ Score = Random(0, 1) ^ (1/Weight) ]
	TArray<FWeightCandidate> Candidates;
	Candidates.Reserve(AllRows.Num());

	for (FPerkSelectDataTableRow* Row : AllRows)
	{
		// 잘못된 가중치는 무시
		if (Row->SelectWeight <= 0.0f) continue;
	
		float RandomValue = FMath::FRand();
		double Score = FMath::Pow(RandomValue, 1.0f / Row->SelectWeight);

		Candidates.Add({ Row, Score });
	}

	// 점수를 기준으로 정렬
	Candidates.Sort();

	// 상위 K개 선택
	int32 Count = FMath::Min(NumSelect, Candidates.Num());
	for (int32 i = 0; i < Count; i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Selected Perk: %s with Score: %f"), *Candidates[i].PerkRow->PerkDataAsset->GetName(), Candidates[i].Score);
		ResultRows.Add(*Candidates[i].PerkRow);
	}

	return ResultRows;
}

void UPerkSelectionScreenWidget::HandlePerkSelected(UPerkDataAsset* SelectedPerkData)
{
    if (!SelectedPerkData || !PerkViewModel) return;

    UE_LOG(LogTemp, Warning, TEXT("UPerkSelectionScreenWidget::HandlePerkSelected called"));

    PerkViewModel->RequestEquipPerk(SelectedPerkData);
    OnPerkSelected.Broadcast();
}
