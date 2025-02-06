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

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	
private:
	UFUNCTION()
	void OnRequestComplete(UVaRestRequestJSON * Result);
};
