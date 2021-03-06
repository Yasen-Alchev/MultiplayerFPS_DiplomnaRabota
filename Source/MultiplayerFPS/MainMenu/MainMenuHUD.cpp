/* Copyright (C) 2022 Alexander Hadzhiev, Yasen Alcev

   MultiplayerFPS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MultiplayerFPS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MultiplayerFPS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MainMenuHUD.h"
#include "MainMenuWidget.h"

AMainMenuHUD::AMainMenuHUD() {}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MainMenuWidgetClass))
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (IsValid(MainMenuWidget))
		{
			MainMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMainMenuHUD::BeginPlay() -> MainMenuWidgetClass is not Valid!!!"));
	}
}

void AMainMenuHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

