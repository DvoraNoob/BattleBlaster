// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessage(FString Message)
{
	MessageText->SetText(FText::FromString(Message));
}
