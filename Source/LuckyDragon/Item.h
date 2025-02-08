// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	FItemData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price = 0;
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

	UFUNCTION(BlueprintCallable,Category = "Item Info")
	void UpdateAmount(const int32& Magnitude);
};
