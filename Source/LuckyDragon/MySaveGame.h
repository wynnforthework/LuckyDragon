// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:
	FPlayerSaveData()
	{
		PlayerName = TEXT("PlayerOne");
		Level = 1;
		Gold = 0;
		Day = 1;
		Exp = 0;
	}

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int64 Gold;

	UPROPERTY()
	int32 Day;
	
	UPROPERTY()
	int32 Exp;
};

USTRUCT()
struct FBagItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Amount;

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FBagItem> PlayerBag;
	
	UPROPERTY()
	FPlayerSaveData PlayerData;

};
