// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor.h"

#include "DemoMainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AGameActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"),*FString(__FUNCTION__));

	// Shut Mouse Cursor
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	check(PlayerController);
	PlayerController->SetShowMouseCursor(true);

	if (UIMainMenu)
	{
		UDemoMainMenu* MainMenu = CreateWidget<UDemoMainMenu>(GetWorld(), UIMainMenu);
		MainMenu->AddToViewport();
	}
}
