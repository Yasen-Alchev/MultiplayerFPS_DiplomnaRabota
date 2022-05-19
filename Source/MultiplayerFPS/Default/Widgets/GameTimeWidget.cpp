// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeWidget.h"
#include "Runtime/UMG/Public/UMG.h"


UGameTimeWidget::UGameTimeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameTimeWidget::UpdateGameTime(int minutes, int seconds)
{
	if (!IsValid(TXTBlock_GameTime))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameTimeWidget::UpdateGameTime(int minutes, int seconds) -> TXTBlock_GameTime is not Valid !!! "));
		return;
	}
	TXTBlock_GameTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
}

void UGameTimeWidget::SetGlobalGameMessage(FString Message)
{
	if (!IsValid(TXTBlock_GlobalGameMessage))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameTimeWidget::SetGlobalGameMessage(FString Message) -> TXTBlock_GlobalGameMessage is not Valid !!! "));
		return;
	}
	TXTBlock_GlobalGameMessage->SetText(FText::FromString(Message));
}

void UGameTimeWidget::ClearGlobalGameMessage()
{
	if (!IsValid(TXTBlock_GlobalGameMessage))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameTimeWidget::ClearGlobalGameMessage() -> TXTBlock_GlobalGameMessage is not Valid !!! "));
		return;
	}
	TXTBlock_GlobalGameMessage->SetText(FText::FromString(""));
}



