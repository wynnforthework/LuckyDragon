// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor.h"

#include "DemoMainMenu.h"
#include "MySaveGame.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void AGameActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"),*FString(__FUNCTION__));

	// Shut Mouse Cursor
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	check(PlayerController);
	PlayerController->SetShowMouseCursor(true);

	bool IsSavedExist = UGameplayStatics::DoesSaveGameExist("TestSaveSlot",0);
	if (IsSavedExist)
	{
		MySaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("TestSaveSlot",0));
	}
	else
	{
		MySaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		MySaveGame->Save();
	}

	if (UIMainMenu)
	{
		UDemoMainMenu* MainMenu = CreateWidget<UDemoMainMenu>(GetWorld(), UIMainMenu);
		MainMenu->AddToViewport();
		if (IsSavedExist)
		{
			MainMenu->ContinueButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MainMenu->ContinueButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
