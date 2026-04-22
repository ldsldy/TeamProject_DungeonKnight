// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MinimapViewModel.generated.h"

// --- 신캡처2D에 미니맵 캡처 요청을 알림 ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinimapCaptureRequested, FVector2D, InMinPoint, FVector2D, InMaxPoint);

// --- 미니맵 초기화 델리게이트 ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinimapInitialized);

class UMinimapManager;
class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UMinimapViewModel : public UObject
{
	GENERATED_BODY()
	
public:
    ///
    /// Call by Model
    /// 
    // --- 미니맵 캡처 요청 함수 ---
    UFUNCTION(BlueprintCallable)
    void RequestMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint);

    // 플레이어 위치 업데이트 (Manager에서 Material 파라미터 설정)
    UFUNCTION(BlueprintCallable)
    void UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw);

    // --- 미니맵 매니저 설정 함수 ---
    void SetMinimapManager(UMinimapManager* InMinimapManager);

    // --- 머티리얼 인스턴스 반환 함수 ---
    UFUNCTION(BlueprintPure)
    UMaterialInstanceDynamic* GetMinimapMaterial() const;

    // SceneCapture 바인딩이 늦게 일어난 경우를 위해 마지막 캡처 요청을 재전송
    void ReplayLastMinimapCaptureRequest();

public:
    // --- 미니맵 캡처 요청 델리게이트 ---
    UPROPERTY(BlueprintAssignable)
    FOnMinimapCaptureRequested OnMinimapCaptureRequested;

    // --- 미니맵 초기화 완료 델리게이트 ---
    UPROPERTY(BlueprintAssignable)
    FOnMinimapInitialized OnMinimapInitialized;

private:
    UPROPERTY()
    TObjectPtr<UMinimapManager> MinimapManager = nullptr;

    bool bHasLastCaptureBounds = false;
    FVector2D LastCaptureMinPoint = FVector2D::ZeroVector;
    FVector2D LastCaptureMaxPoint = FVector2D::ZeroVector;
};
