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
		
	}

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int64 Gold;

	UPROPERTY()
	int32 Day;
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

	void UpdateAmount(const int32& id,const int32& Magnitude);
};
