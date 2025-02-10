// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSubsystem.h"

#include "CrashSightAgent.h"
#include "MySaveGame.h"
#include "SaveGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameSubsystem)

UGameSubsystem::UGameSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TableType(TEXT("/Script/Engine.DataTable'/Game/Data/DT_Gift.DT_Gift'"));
	if (TableType.Succeeded())
	{
		DT_Gift = TableType.Object;
	}

}

void UGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USaveGameSettings* SGSettings = GetDefault<USaveGameSettings>();
	CurrentSlotName = SGSettings->SaveSlotName;
}


void UGameSubsystem::WriteSaveGame()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(WriteSaveGame);

	CurrentSaveGame->PlayerBag.Empty();

	for (int32 Index = 0;Index < PlayerItems.Num();Index++)
	{
		UItem* Item = PlayerItems[Index];
		FBagItem BagItem = FBagItem();
		FMemoryWriter MemWriter(BagItem.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, false);
		Ar.ArIsSaveGame = false;
		Ar.ArNoDelta = true;
		Item->Serialize(Ar);

		CurrentSaveGame->PlayerBag.Add(BagItem);
	}

	CurrentSaveGame->PlayerData = PlayerData;

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

		for (int32 Index = 0;Index < CurrentSaveGame->PlayerBag.Num();Index++)
		{
			FBagItem BagItem = CurrentSaveGame->PlayerBag[Index];
			FMemoryReader MemReader(BagItem.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemReader, false);
			Ar.ArIsSaveGame = false;
			Ar.ArNoDelta = true;
			UItem* Item = NewObject<UItem>();
			Item->Serialize(Ar);
			PlayerItems.Add(Item);
		}
		PlayerData = CurrentSaveGame->PlayerData;
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
	//初始化PlayerData
	PlayerData = FPlayerSaveData();

	CurrentSaveGame->PlayerData = PlayerData;

	PlayerItems.Empty();

	if (DT_Gift)
	{
		int32 Index = 0;
		for (auto iter : DT_Gift->GetRowMap())
		{
			// FName RowName = iter.Key;
			FItemData* Row = (FItemData*)iter.Value;
			UItem* Item = NewObject<UItem>();
			Item->ID = Row->ID;
			Item->Amount = 1;
			PlayerItems.Add(Item);
			Index++;
			if (Index >= 10)
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
	return IsSavedExist;	
}

bool UGameSubsystem::UpdateAmount(const int32& id, const int32& Magnitude)
{
	bool FindItemAndCanUpdateAmount = true;
	UItem* FindItem = nullptr;
	for(int32 Index = 0;Index<PlayerItems.Num();Index++)
	{
		auto Item = PlayerItems[Index];
		if (Item->ID == id)
		{
			FindItem = Item;
			break;
		}
	}
	if (FindItem)
	{
		auto NewAmount = FindItem->Amount + Magnitude;
		if (NewAmount <= 0)
		{
			FindItemAndCanUpdateAmount = false;
		}
		else
		{
			FindItem->Amount = FMath::Clamp(NewAmount, 0,INT32_MAX);
		}
	}
	else
	{
		if (Magnitude>0)
		{
			UItem* Item = NewObject<UItem>();
			Item->ID = id;
			Item->Amount = FMath::Clamp(Magnitude, 0,INT32_MAX);
			PlayerItems.Add(Item);
		}
		else
		{
			FindItemAndCanUpdateAmount = false;
		}
	}

	return FindItemAndCanUpdateAmount;
}

bool UGameSubsystem::UpdateGold(const int32& Magnitude)
{
	bool CanUpdateGold = true;
	auto NewGold = PlayerData.Gold + Magnitude;
	if (NewGold < 0)
	{
		CanUpdateGold = false;
	}
	else
	{
		PlayerData.Gold = FMath::Clamp(NewGold, 0,INT32_MAX);
	}

	return CanUpdateGold;
}

void UGameSubsystem::UpdateDay()
{
	PlayerData.Day += 1;
}

void UGameSubsystem::UpdateLevel(const uint32& Magnitude)
{
	uint32 MaxExp = PlayerData.Level * 10000;
	uint32 AddExp = Magnitude;
	do
	{
		if (PlayerData.Exp + AddExp > MaxExp)
		{
			AddExp -= (MaxExp - PlayerData.Exp);
			PlayerData.Level += 1;
		}
		else
		{
			PlayerData.Exp += AddExp;
			AddExp = 0;
		}
	}
	while (AddExp>0);
}

FPlayerSaveData UGameSubsystem::GetPlayerData()
{
	return PlayerData;
}
