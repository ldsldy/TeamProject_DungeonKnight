// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGanarator.h"
#include "TeamPotato/Room/RoomBase.h"
#include "TeamPotato/Room/DungeonRoom1.h"
#include "TeamPotato/Logic/ClosingWall.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyCharacter.h"
#include "Item/Weapon/WeaponBoxActor.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADungeonGanarator::ADungeonGanarator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGanarator::BeginPlay()
{
	Super::BeginPlay();

    // MVVM 서브시스템에 자신을 등록
    if(UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->RegisterDungeonGeneratorActor(this);
    }

    InitialRoomAmount = RoomAmount;
    FTimerHandle UnusedHandle;

    //시드 정하기
    SetSeed();

    StageConfigSetting();

    //시작 방 생성
    SpawnStarterRooms();

    //다음 방 생성
	SpawnNextRoom();
    //구조가 한정적이라 생성할 방이 없어져서 계속 찾기만 하는 무한루프 빠지는 버그가 있음 N초 지나면 강제 리셋하는 타이머
    GetWorld()->GetTimerManager().SetTimer(GenerationTimeoutHandle, this, &ADungeonGanarator::OnGenerationTimeout, 4.0f, false);

    //방이 모두 생성되면 1초(임의로 정할 수 있음)후 실행(보스방 만들기, 벽 막기 등등)
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ADungeonGanarator::AfterEndedSpawnNomalRooms, 1.0f, false);
}

void ADungeonGanarator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (UMVVMSubsystem* Subsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMVVMSubsystem>())
    {
        Subsystem->UnregisterDungeonGeneratorActor(this);
    }
}

// Called every frame
void ADungeonGanarator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADungeonGanarator::SpawnStarterRooms()
{
    //시작 방 생성
    ARoomBase* SpawnStartRoom = this->GetWorld()->SpawnActor<ARoomBase>(StartRoom[0]);
	if (SpawnStartRoom)
	{
		SpawnStartRoom->SetActorLocation(this->GetActorLocation());

		SpawnStartRoom->ExitPointsFolder->GetChildrenComponents(false, Exits);

        //리셋을 위해 따로 저장
        GeneratedActors.Add(SpawnStartRoom);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("notfoundstartroom"));
	}
}

void ADungeonGanarator::SpawnNextRoom()
{
    //RoomAmount가 0이거나 저장된 CorridorRooms의 요소가 없으면 리턴
    if (RoomAmount <= 0 || CorridorRooms.Num() == 0)
    {
        //if (RoomAmount <=0)
        //{
        //    AfterEndedSpawnNomalRooms();
        //}
        return;
    };
    bCanSpawn = true;

    //시작 방을 기준으로 시작 방의 출구(Exits)에 복도를 생성, 복도 생성에 성공하면 방을 생성
    //시작 방 생성 -> 복도 생성 시도-> 복도 생성 가능한지 검사 -> 생성 가능하면 방 생성 시도 -> 방 생성 가능한지 검사 -> 생성 가능하면 방 생성 -> RoomAmount != 0이면(아직 생성할 방이 남았으면) 0.01초 후(바로 호출하면 좀 불안정함) 다시 SpawnNextRoom 실행 
    //                                                     ㄴ> 생성 실패하면 Destroy                            ㄴ> 생성 실패하면 복도와 같이 Destroy
    //오버랩 판단과 제거는  RemoveOverlappingRooms()로 함

    //복도 생성
    int32 ExitIndex = RandomStream.RandRange(0, Exits.Num() - 1);
    USceneComponent* SelectedExitPoint = Exits[ExitIndex];

    int32 RandomCorridorIndex = RandomStream.RandRange(0, CorridorRooms.Num() - 1);
    TSubclassOf<ARoomBase> SelectedCorridorClass = CorridorRooms[RandomCorridorIndex];

    ARoomBase* SpawnedCorridor = this->GetWorld()->SpawnActor<ARoomBase>(SelectedCorridorClass);

    if (SpawnedCorridor)
    {
        //복도도 생성 목록에 등록
        GeneratedActors.Add(SpawnedCorridor);
    }
    else
    {
        return;
    }

    // 위치/회전 설정
    SpawnedCorridor->SetActorLocation(SelectedExitPoint->GetComponentLocation());
    SpawnedCorridor->SetActorRotation(SelectedExitPoint->GetComponentRotation());

    // 복도 오버랩 검사
    LastestSpawnRoom = SpawnedCorridor;
    RemoveOverlappingRooms();//오버랩 있으면 제거

    // 복도가 겹쳐서 파괴되었다면 리턴
    if (!IsValid(SpawnedCorridor))
    {
        FTimerHandle TimerHandle;
        // 0.01초 뒤에 SpawnNextRoom을 다시 호출 (즉시 호출 아님 -> 스택 초기화됨)
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADungeonGanarator::SpawnNextRoom, 0.01f, false);
        return;//0.01초 사이에 아래 있는 방 생성 코드가 호출됨 그래서 retrun으로 빠르게 컷 해야함
    }

    TArray<USceneComponent*> CorridorExits;
    SpawnedCorridor->ExitPointsFolder->GetChildrenComponents(false, CorridorExits);

    if (CorridorExits.Num() == 0)
    {
        SpawnedCorridor->Destroy();
        return;
    }

    USceneComponent* CorridorExitPoint = CorridorExits[0];

    //방 생성
    TSubclassOf<ARoomBase> RoomClassToSpawn;
    IsSpawnSpecialRoom = false;

    // N번째 방마다 특수 방 생성 (배열이 비어있는지 체크 후 생성, N은 임의의 정수)
    if (RoomAmount % 3 == 0 && SpecialRoomsToBeSpawned.Num() > 0)
    {
        SpecialRoomIndex = CurrentSpecialRoomIndex % SpecialRoomsToBeSpawned.Num();

        RoomClassToSpawn = SpecialRoomsToBeSpawned[SpecialRoomIndex];
        UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentSpecialRoomIndex);
        IsSpawnSpecialRoom = true;

        //버그 있음 가끔 같은 특수방이 나옴 Seed = 13190 -> 해결함
    }
    else
    {
        int32 RoomIndex = RandomStream.RandRange(0, RoomsToBeSpawned.Num() - 1);
        RoomClassToSpawn = RoomsToBeSpawned[RoomIndex];
    }

    ARoomBase* SpawnedRoom = this->GetWorld()->SpawnActor<ARoomBase>(RoomClassToSpawn);

    if (SpawnedRoom)
    {
        // [추가] 방도 생성 목록에 등록
        GeneratedActors.Add(SpawnedRoom);
    }

    SpawnedRoom->SetActorLocation(CorridorExitPoint->GetComponentLocation());
    SpawnedRoom->SetActorRotation(CorridorExitPoint->GetComponentRotation());

    // 방 오버랩 검사
    LastestSpawnRoom = SpawnedRoom;

    DoorList.Add(SelectedExitPoint);

    if (IsValid(CorridorExitPoint))
    {
        DoorList.Add(CorridorExitPoint); // 2. 복도 -> 다음 방 사이의 문
    }

    RemoveOverlappingRooms();

    if (IsValid(SpawnedRoom))
    {
        // 성공: 기존 출구 제거 및 새 방 출구 추가
        Exits.Remove(SelectedExitPoint);
        if (IsSpawnSpecialRoom)
        {
            CurrentSpecialRoomIndex++;
        }
        RoomAmount--;
        TArray<USceneComponent*> NewRoomExits;
        SpawnedRoom->ExitPointsFolder->GetChildrenComponents(false, NewRoomExits);
        Exits.Append(NewRoomExits);
    }
    else
    {
        // 실패: 방이 겹치면 연결된 복도도 같이 파괴
        if (IsValid(SpawnedCorridor))
        {
           SpawnedCorridor->Destroy();
        }
    }

    //재귀호출로 다음 방 생성
    if (RoomAmount > 0)
    {
        FTimerHandle TimerHandle;
        // 0.01초 뒤에 SpawnNextRoom을 다시 호출 (즉시 호출 아님 -> 스택 초기화됨)
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADungeonGanarator::SpawnNextRoom, 0.01f, false);
        // 그냥 SpawnNextRoom() 바로 불러도 상관 없는데 방 많아지면 좀 불안정해짐
    }


    
}

void ADungeonGanarator::RemoveOverlappingRooms()
{
    //오버랩 된 방 제거
	TArray<USceneComponent*> OverlappedRooms;
	LastestSpawnRoom->OverlapFolder->GetChildrenComponents(false, OverlappedRooms);

	TArray<UPrimitiveComponent*> OverlapingCompoenets;

	for (USceneComponent* Element : OverlappedRooms)
	{
		Cast<UBoxComponent>(Element)->GetOverlappingComponents(OverlapingCompoenets);
	}

	for (USceneComponent* Element : OverlapingCompoenets)
	{
		bCanSpawn = false;
		//RoomAmount++;
		LastestSpawnRoom->Destroy();
		return;
	}
}

//모든 방 생성이 끝나고 실행되는 함수, 보스방 스폰과 닫힌 벽을 막고 방을 들어갔는지 확인하는 콜리전 활성화함
void ADungeonGanarator::AfterEndedSpawnNomalRooms()
{
    //보물방 같은 특수방 지정하는 함수, 단순히 현재 생성된 방들 중 고를거라 보스방 생성 전 돌려야함 아마 사용 안할듯
    //SelectedSpecialRoom();
    //보스방 생성
    if (!SpawnLastRoom())
    {
        return;
        //통로 닫기 전 보스방 생성 성공여부 검사, 생성 실패하면 스테이지 다시 만드는 구조라 ClosingUnuusedWall 호출되지 않게 리턴
    }
    //안쓰는 통로 닫는 함수
    SpawnDoors();

    ClosingUnuusedWall();

    for (AActor* Actor : GeneratedActors)
    {
        ARoomBase* Room = Cast<ARoomBase>(Actor);
        if (Room)
        {
            Room->ActivateBattleTrigger();
        }
    }


    /// 보스방 생성 성공시에 처리(Minimap)
    CalculateDungeonMinMaxPoint();

    if (EndedCreate.IsBound())
    {
        EndedCreate.Broadcast();
    }

    // 스테이지 및 챕터 변경 델리게이트 호출
    OnStageAndChapterChanged.Broadcast(Stage, chapter);

    if (UGameStateSubsystem* GameStateSubsystem = UGameStateSubsystem::Get(this))
    {
        GameStateSubsystem->NotifyDungeonReadyToWarmup();
    }

}

//닫힌 벽 막는 함수
void ADungeonGanarator::ClosingUnuusedWall()
{
    //모든 출구에 대해 검사
    for (USceneComponent* Element : Exits)
    {
        //막을 벽 설정
        AClosingWall* LastestClosingWallSpawned = GetWorld()->SpawnActor<AClosingWall>(ClosingWall[0]);
        FVector RelativeOffset(LastestClosingWallSpawned->GetLoc());//여기 나중에 수정해야 함(하드코딩)
        FVector WorldOffset = Element->GetComponentRotation().RotateVector(RelativeOffset);

        LastestClosingWallSpawned->SetActorLocation(Element->GetComponentLocation() + WorldOffset);
        LastestClosingWallSpawned->SetActorRotation(Element->GetComponentRotation() + LastestClosingWallSpawned->GetRot());

        GeneratedActors.Add(LastestClosingWallSpawned);
    }
    UE_LOG(LogTemp, Warning, TEXT("ClosingUnuusedWall"));
}

bool ADungeonGanarator::SpawnLastRoom()
{
    //보스방 생성
    TArray<TSubclassOf<ARoomBase>>* TargetRoomArray;

    if (chapter == MaxAmout)
    {
        TargetRoomArray = &BossRoomClass;
        UE_LOG(LogTemp, Warning, TEXT("createboss"));
    }
    else
    {
        TargetRoomArray = &PotalRoomClass;
        UE_LOG(LogTemp, Warning, TEXT("createportal"));
    }

    if (TargetRoomArray->Num() > 0 && IsValid(LastestSpawnRoom))//안전검사
    {
        USceneComponent* LastExit = nullptr;
        for (USceneComponent* Exit : Exits)
        {
            if (Exit->GetOwner() == LastestSpawnRoom)
            {
                LastExit = Exit;
                break;
            }
        }

        if (LastExit)
        {
            ARoomBase* LastRoom = GetWorld()->SpawnActor<ARoomBase>((*TargetRoomArray)[0]);
            if (LastRoom)
            {
                LastRoom->SetActorLocation(LastExit->GetComponentLocation());
                LastRoom->SetActorRotation(LastExit->GetComponentRotation());

                ARoomBase* TempLast = LastestSpawnRoom;
                LastestSpawnRoom = LastRoom;
                RemoveOverlappingRooms();

                if (!IsValid(LastRoom)) // 겹쳐서 파괴됨 -> 리셋 시도
                {
                    LastestSpawnRoom = TempLast; // 복구

                    if (CurrentResetCount < MaxResetLimit)
                    {
                        //만약 보스방이 다른 방과 겹쳐서 생성에 문제가 생기면 그냥 맵 리셋 처음부터 다시 생성
                        ResetDungeon();
                        return false;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("한도초과"));
                    }
                }
                else
                {
                    // 성공
                    DoorList.Add(LastExit);
                    GeneratedActors.Add(LastRoom);
                    Exits.Remove(LastExit);
                    UE_LOG(LogTemp, Warning, TEXT("BossRoomSpawn"));
                    CurrentResetCount = 0;
                    GetWorld()->GetTimerManager().ClearTimer(GenerationTimeoutHandle);
                }
            }
        }
    }

    return true;
}

void ADungeonGanarator::SelectedSpecialRoom()
{
    //UE_LOG(LogTemp, Log, TEXT("SelectedSpecialRoom"));

}

void ADungeonGanarator::ResetDungeon()
{

    //현재 진행 중인 타이머 모두 중지
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), AllEnemies);
    TArray<AActor*> WeaponBoxes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponBoxActor::StaticClass(), WeaponBoxes);

    StageConfigSetting();
    for (AActor* EnemyActor : AllEnemies)
    {
        if (IsValid(EnemyActor))
        {
            EnemyActor->Destroy();
        }
    }

    for (AActor* BoxActor : WeaponBoxes)
    {
        if (IsValid(BoxActor))
        {
            BoxActor->Destroy();
        }
    }

    //지금까지 생성된 모든 방/복도 파괴

    for (AActor* Actor : GeneratedActors)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
    GeneratedActors.Empty(); // 목록 비우기
    Exits.Empty();           // 출구 목록 비우기
    DoorList.Empty();
    //변수 초기화
    CurrentSpecialRoomIndex = 0;
    SpecialRoomIndex = 0;
    RoomAmount = InitialRoomAmount; // 방 개수 복구
    bCanSpawn = false;
    CurrentResetCount++;
    SpawnStarterRooms();
    SpawnNextRoom();
    GetWorld()->GetTimerManager().SetTimer(GenerationTimeoutHandle, this, &ADungeonGanarator::OnGenerationTimeout, 4.0f, false);

    //종료 타이머 다시 설정
    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ADungeonGanarator::AfterEndedSpawnNomalRooms, 1.0f, false);

    FTimerHandle DoorHandle;
    GetWorld()->GetTimerManager().SetTimer(DoorHandle, this, &ADungeonGanarator::SpawnDoors, 1.0f, false);
}

void ADungeonGanarator::OnGenerationTimeout()
{
    //안전장치 타이머 끝나면 걍 강제 리셋
    ResetDungeon();
}

void ADungeonGanarator::SetSeed()//게임에 사용할 시드 정하는 함수
{
    int32 Results;
    if (Seed == -1)//-1이면 랜덤 시드
    {
        Results = FMath::Rand();
    }
    else //아니면 미리 입력한 시드
    {
        Results = Seed;
    }
    RandomStream.Initialize(Results);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), Results));//디버그용 시드 출력
}

void ADungeonGanarator::SpawnDoors()
{
    if (!Doors)
    {
        UE_LOG(LogTemp, Error, TEXT("Door is Null"));
        return;
    }

    // 모든 출구에 대해 검사
    for (USceneComponent* Element : DoorList)
    {
        if (!IsValid(Element)) continue;

        ADoor* LastestDoorSpawn = GetWorld()->SpawnActor<ADoor>(Doors);

        if (LastestDoorSpawn)
        {
            FVector RelativeOffset(LastestDoorSpawn->GetLoc());
            FVector WorldOffset = Element->GetComponentRotation().RotateVector(RelativeOffset);

            LastestDoorSpawn->SetActorLocation(Element->GetComponentLocation() + WorldOffset);
            LastestDoorSpawn->SetActorRotation(Element->GetComponentRotation() + LastestDoorSpawn->GetRot());

            GeneratedActors.Add(LastestDoorSpawn);

            AActor* OwnerActor = Element->GetOwner();
            ARoomBase* OwnerRoom = Cast<ARoomBase>(OwnerActor);

            if (OwnerRoom)
            {
                // 방에 문을 등록 (이 함수 안에서 Door->SetOwningRoom도 호출됨)
                OwnerRoom->RegisterDoor(LastestDoorSpawn);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnDoors"));
        }
    }

}

void ADungeonGanarator::StageConfigSetting()
{
    if (StageConfigMap.Contains(Stage))
    {
        FStageRoomConfig& SelectedConfig = StageConfigMap[Stage];
        RoomAmount = SelectedConfig.StageRoomAmount;

        // 1. 일반 방 목록 교체
        if (SelectedConfig.NormalRooms.Num() > 0
            && SelectedConfig.SpecialRooms.Num() > 0
            && SelectedConfig.Corridors.Num() > 0
            && SelectedConfig.StartRooms.Num() > 0
            && SelectedConfig.BossRooms.Num() > 0
            && SelectedConfig.ClosingWalls.Num() > 0
            && SelectedConfig.PotalRooms.Num() > 0
            && RoomAmount > 0
            )
        {
            RoomsToBeSpawned = SelectedConfig.NormalRooms;
            SpecialRoomsToBeSpawned = SelectedConfig.SpecialRooms;
            CorridorRooms = SelectedConfig.Corridors;
            StartRoom = SelectedConfig.StartRooms;
            BossRoomClass = SelectedConfig.BossRooms;
            PotalRoomClass = SelectedConfig.PotalRooms;
            ClosingWall = SelectedConfig.ClosingWalls;
            InitialRoomAmount = RoomAmount;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Stage %d : NormalRooms array is empty! Using default."), Stage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Stage %d config not found in StageConfigMap. Using default arrays."), Stage);
    }
}

void ADungeonGanarator::GoToNextStage(int32 NewChapter)
{
    SetSeed();
    int32 NextStage = NewChapter+1;
    UE_LOG(LogTemp, Error, TEXT("%d스테이지-%d챕터"), Stage, NewChapter);
    if (NextStage > MaxAmout)
    {
        chapter = 1;
        Stage++;
    }
    else
    {
        chapter++;
    }

    if (UGameStateSubsystem* GameStateSubsystem = UGameStateSubsystem::Get(this))
    {
        // 같은 레벨에서 던전 재생성할 때도 로딩 게이트를 사용
        GameStateSubsystem->BeginLoadingGate(true, true);
    }

    ResetDungeon();
}

void ADungeonGanarator::CalculateDungeonMinMaxPoint()
{
    FVector2D MinPoint(FLT_MAX, FLT_MAX);
    FVector2D MaxPoint(-FLT_MAX, -FLT_MAX);

    for (AActor* actor : GeneratedActors)
    {
        if (!IsValid(actor))
        {
            continue;
        }

        FVector2D actorMin, actorMax;

        FVector Origin, Extend;
        actor->GetActorBounds(true, Origin, Extend);

        actorMin.X = Origin.X - Extend.X;
        actorMin.Y = Origin.Y - Extend.Y;
        actorMax.X = Origin.X + Extend.X;
        actorMax.Y = Origin.Y + Extend.Y;

        MinPoint.X = FMath::Min(MinPoint.X, actorMin.X);
        MinPoint.Y = FMath::Min(MinPoint.Y, actorMin.Y);

        MaxPoint.X = FMath::Max(MaxPoint.X, actorMax.X);
        MaxPoint.Y = FMath::Max(MaxPoint.Y, actorMax.Y);
    }

    // 로딩창 종료
    if (OnDungeonGenerationCompleted.IsBound())
    {
        OnDungeonGenerationCompleted.Broadcast(MinPoint, MaxPoint);
    }
}
