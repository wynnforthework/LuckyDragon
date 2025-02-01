// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void AfterDelay();
protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	
};
