// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VaRestJsonObject.h"
#include "Blueprint/UserWidget.h"
#include "VaRestSubsystem.h"
#include "DemoMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UDemoMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ContinueButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* EndButton;

	UPROPERTY(Transient, BlueprintReadOnly, meta=(BindWidgetAnim))
	class UWidgetAnimation* enter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* LogoPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* StoryPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* HomePanel;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* LoginPanel;
			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* PopupPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextStory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextAI;
	
	UVaRestJsonObject* ResultObject;

	UFUNCTION()
	void PlayEnterAnimation();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* NextDayWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* PopupSureWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextDay;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextGold;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextLevel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* GiftWidget;

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	// 打字机效果
	FString FullText;
	FString DisplayedText;
	int32 CurrentCharacterIndex;
	FTimerHandle TypingTimerHandle;

	void StartTypewriterEffect(const FString& TextToType, float Interval = 0.1f);
	void TypeNextCharacter();
private:
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ContinueGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void AfterDelay();
	
	UFUNCTION()
	void OnRequestComplete(UVaRestRequestJSON * Result);

	void PlayStory();

	void RequestAIData();
	
	void HideAllPanel();

	void CheckGameState();
	int32 NewGameState;
	
	void ShowPopup();
	
	UFUNCTION()
	void ClosePopup();

	UFUNCTION()
	void NextDay();
};
