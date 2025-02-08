// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSubsystem.h"

#include "MySaveGame.h"
#include "SaveGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameSubsystem)

void UGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USaveGameSettings* SGSettings = GetDefault<USaveGameSettings>();
	CurrentSlotName = SGSettings->SaveSlotName;
	DT_Gift = SGSettings->DummyTablePath.LoadSynchronous();
}

void UGameSubsystem::WriteSaveGame()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(WriteSaveGame);

	CurrentSaveGame->PlayerBag.Empty();

	for (UItem* Item : PlayerItems)
	{
		FBagItem BagItem;
		BagItem.ID = Item->ID;
		BagItem.Amount = Item->Amount;

		FMemoryWriter MemWriter(BagItem.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Item->Serialize(Ar);

		CurrentSaveGame->PlayerBag.Add(BagItem);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,CurrentSlotName,0);
	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

void UGameSubsystem::LoadSaveGame()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(LoadSaveGame);

	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName,0))
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName,0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOGFMT(LogTemp,Warning,"Failed to load save game data");
			return;
		}

		UE_LOGFMT(LogTemp, Log, "Loaded SaveGame Data.");

		for (FBagItem BagItem : CurrentSaveGame->PlayerBag)
		{
			FMemoryReader MemReader(BagItem.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
			Ar.ArIsSaveGame = true;
			UItem* Item = NewObject<UItem>();
			Item->Amount = BagItem.Amount;
			Item->ID = BagItem.ID;
			PlayerItems.Add(Item);
		}
		
		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = CastChecked<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		UE_LOGFMT(LogTemp, Log, "No SaveGame Data. Creating new SaveGame.");
	}
}

void UGameSubsystem::AsyncSave()
{
}

void UGameSubsystem::AsyncLoad()
{
}

void UGameSubsystem::NewSave()
{
	CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	CurrentSaveGame->PlayerData.PlayerName = TEXT("PlayerOne");
	CurrentSaveGame->PlayerData.Level = 1;
	CurrentSaveGame->PlayerData.Gold = 0;
	CurrentSaveGame->PlayerData.Day = 1;

	PlayerItems.Empty();

	if (DT_Gift)
	{
		TArray<FItemData*> Rows;
		DT_Gift->GetAllRows(TEXT("RowName"),Rows);
		for (int32 Index = 0;Index < Rows.Num();++Index)
		{
			FItemData* Row = Rows[Index];
			UItem* Item = NewObject<UItem>();
			Item->ID = Row->ID;
			PlayerItems.Add(Item);
			if (CurrentSaveGame->PlayerBag.Num()>=10)
			{
				break;
			}
		}
	}
	WriteSaveGame();
}

bool UGameSubsystem::HasSaveData()
{
	bool IsSavedExist = UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0);
	if (IsSavedExist)
	{
		LoadSaveGame();
	}
	else
	{
		NewSave();
	}
	return IsSavedExist;	
}
