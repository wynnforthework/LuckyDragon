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
	class UCanvasPanel* LogoPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* StoryPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* StoryText;
	
	UVaRestJsonObject* ResultObject;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ContinueGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void AfterDelay();

	UFUNCTION()
	void PlayEnterAnimation();
	void HideLogoPanel();

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
	void OnRequestComplete(UVaRestRequestJSON * Result);
};
