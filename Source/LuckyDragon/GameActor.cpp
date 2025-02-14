// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor.h"

#include "DemoMainMenu.h"
#include "GameSubsystem.h"
#include "GoogleSheetMenu.h"
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

	bool IsSavedExist = GetGameInstance()->GetSubsystem<UGameSubsystem>()->HasSaveData();

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
		//播放开始动画
		MainMenu->PlayEnterAnimation();
	}
#if WITH_EDITOR
	if (UIGoogleSheetMenu)
	{
		auto GoogleSheetMenu = CreateWidget<UGoogleSheetMenu>(GetWorld(),UIGoogleSheetMenu);
		GoogleSheetMenu->AddToViewport();
	}
#endif
	
}
