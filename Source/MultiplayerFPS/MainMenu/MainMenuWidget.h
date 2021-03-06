/*  Copyright (C) 2001-2003 Free Software Foundation, Inc.

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

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Runtime/UMG/Public/UMG.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
		void HostGame();

	UFUNCTION()
		void ConnectToServer();

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void LoginUser();

	UFUNCTION()
		void LogOut();

	UFUNCTION()
		void RegisterUser();

	UFUNCTION()
		void RecoverPassword();

	UFUNCTION()
		void GoToJoinMenu();

	UFUNCTION()
		void GoToMainMenu();

	UFUNCTION()
		void GoToLoginMenu();

	UFUNCTION()
		void GoToRegisterMenu();

	UFUNCTION()
		void GoToPasswordRecoveryMenu();

	UFUNCTION()
		void SwitchGameModeLeft();

	UFUNCTION()
		void SwitchGameModeRight();

	UFUNCTION()
		FText GetGameModePosText(int pos);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TXTBlock_GameModeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_LoginUsername;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_LoginPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_RegisterUsername;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_RegisterPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_RegisterEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UEditableTextBox* EditableTextBox_RecoveryEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* ButtonLeft_ChooseGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* ButtonRight_ChooseGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_HostGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_JoinGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_QuitGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_LoginQuitGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_ConnectToServer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_Login;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_LogOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_BackToLogin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_SignUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_Register;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_ResetPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_BackToMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_PasswordRecoveryBackToLogin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Button_PasswordSendRecoveryEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;

	//Menus Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* JoinMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* LoginMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* RegisterMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UWidget* PasswordRecoveryMenu;

private:
	UPROPERTY()
	int gameModePos;

	UPROPERTY()
	int  numberOfGameModes;
};
