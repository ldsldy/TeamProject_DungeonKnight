// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LevelDataAsset.h"

bool ULevelDataAsset::GetLevelDataByState(EGameState State, FLevelData& OutLevelData) const
{
	const FLevelData* FoundLevelData = LevelsByState.Find(State);
	if (!FoundLevelData)
	{
		return false;
	}

	OutLevelData = *FoundLevelData;
	return true;
}
