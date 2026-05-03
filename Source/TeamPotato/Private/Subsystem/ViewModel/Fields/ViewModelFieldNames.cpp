// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

namespace PlayerResourceVMFields
{
    const FName HealthCurrent(TEXT("HealthCurrent"));
    const FName HealthMax(TEXT("HealthMax"));
    const FName HealthPercent(TEXT("HealthPercent"));

    const FName EnergyCurrent(TEXT("EnergyCurrent"));
    const FName EnergyMax(TEXT("EnergyMax"));
    const FName EnergyPercent(TEXT("EnergyPercent"));

    const FName Gold(TEXT("Gold"));
    const FName WalkSpeed(TEXT("WalkSpeed"));
    const FName AttackPower(TEXT("AttackPower"));
}

namespace WeaponVMFields
{
    const FName MainWeaponData(TEXT("MainWeaponData"));
    const FName SubWeaponData(TEXT("SubWeaponData"));
}

namespace PerkVMFields
{
    const FName EquippedPerks(TEXT("EquippedPerks"));
}

namespace EnemyVMFields
{
    const FName CurrentHealth(TEXT("CurrentHealth"));
    const FName MaxHealth(TEXT("MaxHealth"));
    const FName HealthPercent(TEXT("HealthPercent"));
    const FName IsVisible(TEXT("IsVisible"));
}

namespace MinimapVMFields
{
    const FName IsInitialized(TEXT("IsInitialized"));
}
