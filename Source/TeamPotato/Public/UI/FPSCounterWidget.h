// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCounterWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UFPSCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFPSCounterWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "FPS")
	int32 GetCurrentFPS() const { return CurrentFPS; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UTextBlock* ResolveFPSText();
	void UpdateFPSText();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> FPSText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS", meta = (ClampMin = "0.05", UIMin = "0.05"))
	float UpdateInterval = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS")
	FText TextFormat;

private:
	float AccumulatedTime = 0.0f;
	int32 AccumulatedFrames = 0;
	int32 CurrentFPS = 0;
};
