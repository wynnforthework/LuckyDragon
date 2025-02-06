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
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UDemoMainMenu::ContinueGame);
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

void UDemoMainMenu::ContinueGame()
{
	UVaRestSubsystem * VaRestSubsystem = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	UVaRestRequestJSON * RequestJSON = VaRestSubsystem->ConstructVaRestRequestExt(EVaRestRequestVerb::POST,EVaRestRequestContentType::json);
	UVaRestJsonObject* JsonObject = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject->SetStringField(TEXT("role"), TEXT("user"));
	JsonObject->SetStringField(TEXT("content"), TEXT("Hello!"));
	const TArray<UVaRestJsonObject*> JsonArray = {JsonObject};
	UVaRestJsonObject* JsonObject2 = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject2->SetStringField(TEXT("model"), TEXT("deepseek-chat"));
	JsonObject2->SetObjectArrayField(TEXT("messages"),JsonArray);
	RequestJSON->SetRequestObject(JsonObject2);
	FString ApiKey;
	const FString DefaultGamePath = FString::Printf(TEXT("%sDeepSeek.ini"), *FPaths::SourceConfigDir());
	GConfig->GetString(TEXT("DeepSeek"), TEXT("ApiKey"), ApiKey, DefaultGamePath);
	RequestJSON->SetHeader(TEXT("Authorization"), ApiKey);
	RequestJSON->SetHeader(TEXT("Content-Type"),TEXT("application/json"));
	RequestJSON->OnRequestComplete.AddDynamic(this, &UDemoMainMenu::OnRequestComplete);
	
	FLatentActionInfo LatentInfo = FLatentActionInfo();
	RequestJSON->ApplyURL(TEXT("https://api.deepseek.com/chat/completions"),ResultObject,this,LatentInfo);
	// VaRestSubsystem->CallURL(TEXT("https://api.deepseek.com/chat/completions"),EVaRestRequestVerb::POST,EVaRestRequestContentType::json,JsonObject,);
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
void UDemoMainMenu::OnRequestComplete(UVaRestRequestJSON * Result) {
	UE_LOG(LogTemp, Display, TEXT("Response Complete"));
	TArray<UVaRestJsonObject*> VaRestJsonObjects = Result->GetResponseObject()->GetObjectArrayField(TEXT("choices"));
	if (VaRestJsonObjects.Num()>0)
	{
		UVaRestJsonObject* VaRestJsonObject = VaRestJsonObjects[0]->GetObjectField(TEXT("message"));
		FString role = VaRestJsonObject->GetStringField(TEXT("role"));
		FString content = VaRestJsonObject->GetStringField(TEXT("content"));
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] role:%s content:%s"), *FString(__FUNCTION__), *role, *content);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));
	}
}