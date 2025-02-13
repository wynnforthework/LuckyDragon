// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FItemData()
	{
		ID = 0;
		ItemName = "";
		Price = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price;
};

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UItem() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;
};
