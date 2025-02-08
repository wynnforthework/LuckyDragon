// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UMySaveGame::UpdateAmount(const int32& id, const int32& Magnitude)
{
	auto Item = PlayerBag.FindByPredicate([id](const FBagItem& Item) {return Item.ID == id; });
	Item->Amount += Magnitude;
	Item->Amount = FMath::Clamp(Item->Amount, 0,UINT32_MAX);
}
