// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PGViewModelBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewModelFieldChanged, FName, FieldName);
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPGViewModelBase : public UObject
{
    GENERATED_BODY()

public:
    // 바인딩 대상 모델 초기화 및 해제 함수
    virtual void Initialize(UObject* InModel) { BoundModel = InModel; }
    virtual void Deinitialize() { BoundModel = nullptr; }

    UPROPERTY(BlueprintAssignable, Category = "MVVM")
    FOnViewModelFieldChanged OnFieldChanged;

protected:
    template<typename T>
    bool SetAndNotify(T& Target, const T& NewValue, FName PropertyName)
    {
        if(Target == NewValue)
        {
            return false; // 값이 변경되지 않았으므로 알림을 보내지 않음
        }

        Target = NewValue;
        OnFieldChanged.Broadcast(PropertyName); // 값이 변경되었으므로 알림을 보냄
        return true;
    }

    bool SetAndNotifyFloat(float& Target, float NewValue, FName PropertyName, float Epsilon = KINDA_SMALL_NUMBER)
    {
        if (FMath::IsNearlyEqual(Target, NewValue, Epsilon))
        {
            return false;
        }

        Target = NewValue;
        OnFieldChanged.Broadcast(PropertyName);
        return true;
    }

    UPROPERTY()
    TObjectPtr<UObject> BoundModel = nullptr;
};
