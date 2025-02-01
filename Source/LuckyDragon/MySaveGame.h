// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserIndex;

	UMySaveGame();

	void AsyncSave();
	void Save();
	void AsyncLoad();
	void Load();

private:
	void ProcessSaveComplete(const FString& SlotName, const int32 Index, bool bSuccess);
	void ProcessLoadComplete(const FString& SlotName, const int32 Index,USaveGame* LoadedGameData);
};
