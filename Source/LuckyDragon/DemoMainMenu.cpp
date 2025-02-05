// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoMainMenu.h"

#include "MySaveGame.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UDemoMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDemoMainMenu::Initialize()
{
	Super::Initialize();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UDemoMainMenu::StartGame);
	}
	if (EndButton)
	{
		EndButton->OnClicked.AddDynamic(this, &UDemoMainMenu::QuitGame);
	}
	return true;
}

void UDemoMainMenu::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));

	UMySaveGame* MySaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	MySaveGame->Save();
}

void UDemoMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));

	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}

void UDemoMainMenu::AfterDelay()
{
	UE_LOG(LogTemp,Warning,TEXT("[wyh] [%s] Delay Complete"),*FString(__FUNCTION__));
}

void UDemoMainMenu::PlayEnterAnimation()
{
	if (enter != nullptr)
	{
		PlayAnimation(enter,0.0f,1,EUMGSequencePlayMode::Forward,1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("enter is not found!"));	
	}
}

