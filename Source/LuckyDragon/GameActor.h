// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameActor.generated.h"

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API AGameActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> UIMainMenu;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> UIGoogleSheetMenu;

protected:
	virtual void BeginPlay() override;

private:
	const char* AppIDChar;
};
