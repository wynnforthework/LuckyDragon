// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UMySaveGame::AsyncSave()
{
	if (UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &UMySaveGame::ProcessSaveComplete);
		SaveGameInstance->PlayerName = TEXT("PlayerOne");
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex, SavedDelegate);
	}
}

void UMySaveGame::Save()
{
	if(UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())))
	{
		SaveGameInstance->PlayerName = TEXT("PlayerOne");
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex))
		{
			// 保存成功
		}
	}
}

void UMySaveGame::ProcessSaveComplete(const FString& SlotName, const int32 Index, bool bSuccess)
{
	if (bSuccess)
	{
		// 保存成功
	}
}

void UMySaveGame::AsyncLoad()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &UMySaveGame::ProcessLoadComplete);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, UserIndex, LoadedDelegate);
}


void UMySaveGame::Load()
{
	if (UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,UserIndex)))
	{
		UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"),*LoadedGame->PlayerName);
	}
}

void UMySaveGame::ProcessLoadComplete(const FString& SlotName, const int32 Index, USaveGame* LoadedGameData)
{
	
}
