// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 플레이어 리소스 뷰모델에서 사용할 필드 이름 상수들
namespace PlayerResourceVMFields
{
    TEAMPOTATO_API extern const FName HealthCurrent;
    TEAMPOTATO_API extern const FName HealthMax;
    TEAMPOTATO_API extern const FName HealthPercent;

    TEAMPOTATO_API extern const FName EnergyCurrent;
    TEAMPOTATO_API extern const FName EnergyMax;
    TEAMPOTATO_API extern const FName EnergyPercent;

    TEAMPOTATO_API extern const FName Gold;
    TEAMPOTATO_API extern const FName WalkSpeed;
    TEAMPOTATO_API extern const FName AttackPower;
}

namespace WeaponVMFields
{
    TEAMPOTATO_API extern const FName MainWeaponData;
    TEAMPOTATO_API extern const FName SubWeaponData;
}

namespace PerkVMFields
{
    TEAMPOTATO_API extern const FName EquippedPerks;
}

namespace EnemyVMFields
{
    TEAMPOTATO_API extern const FName CurrentHealth;
    TEAMPOTATO_API extern const FName MaxHealth;
    TEAMPOTATO_API extern const FName HealthPercent;
    TEAMPOTATO_API extern const FName IsVisible;
}

namespace MinimapVMFields
{
    TEAMPOTATO_API extern const FName IsInitialized;
}
