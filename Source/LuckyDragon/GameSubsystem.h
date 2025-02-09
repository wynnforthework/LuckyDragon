// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MySaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSubsystem.generated.h"


class UMySaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class UMySaveGame*, SaveObject);

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UGameSubsystem();
private:
	FPlayerSaveData PlayerData;

protected:
	FString CurrentSlotName;

	UPROPERTY(Transient)
	TObjectPtr<UMySaveGame> CurrentSaveGame;

public:

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	void AsyncSave();
	void AsyncLoad();
	void NewSave();
	bool HasSaveData();

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	TArray<UItem*> PlayerItems;
	
	UDataTable* DT_Gift;
		
	bool UpdateAmount(const int32& id,const int32& Magnitude);
	void UpdateGold(const int32& Magnitude);
	void UpdateDay();
	void UpdateLevel(const uint32& Magnitude);
	FPlayerSaveData GetPlayerData();
};
