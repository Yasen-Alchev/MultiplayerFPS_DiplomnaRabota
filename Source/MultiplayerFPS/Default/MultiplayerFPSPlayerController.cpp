#include "MultiplayerFPSPlayerController.h"

#include "MultiplayerFPSGameMode.h"
#include "MultiplayerFPSGameState.h"
#include "MultiplayerFPSInGameHUD.h"
#include "TeamBasedClasses/TeamBasedCharacter.h"

AMultiplayerFPSPlayerController::AMultiplayerFPSPlayerController()
{
	bShouldRespawn = true;
}

void AMultiplayerFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	ServerRestartPlayerOnStart();
}

void AMultiplayerFPSPlayerController::OnPossess(APawn* MovieSceneBlends)
{
	Super::OnPossess(MovieSceneBlends);

	AMultiplayerFPSCharacter* MyPawn = Cast<AMultiplayerFPSCharacter>(MovieSceneBlends);
	if (!IsValid(MyPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::OnPossess(APawn* MovieSceneBlends) -> MyPawn is not Valid !!!"), *this->GetName());
		return;
	}
	MyPawn->Init();

}

void AMultiplayerFPSPlayerController::ClientEndGame_Implementation(const FString& Winner)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCEndGame_Implementation(ETeams WinnerTeam) -> World is not Valid !!!"));
		return;
	}

	bShouldRespawn = false;
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	World->GetTimerManager().SetTimer(AntiBlurHandle, [this, World]()
		{
			World->GetTimerManager().ClearTimer(AntiBlurHandle);
			SetPause(true);
		}, 0.5f, false);

	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(InGameHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCEndGame_Implementation(ETeams WinnerTeam) -> InGameHud is not Valid !!!"));
		return;
	}
	SetShowMouseCursor(true);
	InGameHud->GameEnded(Winner);
}

void AMultiplayerFPSPlayerController::ClientUpdateGameTime_Implementation(int minutes, int seconds)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(InGameHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCUpdateGameTime_Implementation(int Minutes, int seconds) -> InGameHud is not Valid !!!"));
		return;
	}
	InGameHud->UpdateGameTime(minutes, seconds);
}

void AMultiplayerFPSPlayerController::ClientUpdateObjectiveStats_Implementation(const TArray<FString>& ObjectiveStats)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(InGameHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::UpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore) -> InGameHud is not Valid !!!"));
		return;
	}
	InGameHud->UpdateObjectiveStats(ObjectiveStats);
}

void AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> Does not have Authority !!!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> World is  not Valid !!!"));
		return;
	}

	AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> GameMode is not Valid !!!"));
		return;
	}
	GameMode->RestartPlayerAtPlayerStart(this, GameMode->ChoosePlayerStart(this));
}

void AMultiplayerFPSPlayerController::ClientSetGlobalGameMessage_Implementation(const FString& Message)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(InGameHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientSetGlobalGameMessage_Implementation(FString Message) -> InGameHud is not Valid !!!"));
		return;
	}
	InGameHud->SetGlobalGameMessage(Message);
}

void AMultiplayerFPSPlayerController::ClientClearGlobalGameMessage_Implementation()
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(InGameHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientClearGlobalGameMessage_Implementation() -> InGameHud is not Valid !!!"));
		return;
	}
	InGameHud->ClearGlobalGameMessage();

}

void AMultiplayerFPSPlayerController::ClientUpdateLeaderBoardStats_Implementation()
{
	AMultiplayerFPSInGameHUD* PlayerHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (!IsValid(PlayerHud))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientUpdateLeaderBoardStats_Implementation() -> PlayerHud is not Valid !!!"));
		return;
	}
	PlayerHud->UpdateLeaderBoard();
}

void AMultiplayerFPSPlayerController::RespawnPlayer(bool instant)
{
	KillPlayer();
	if (!bShouldRespawn) return;

	if (instant)
	{
		ServerRespawnPlayer();
	}
	else
	{
		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::RespawnPlayer(bool instant) -> World is not Valid !!!"), *this->GetName());
			return;
		}

		AMultiplayerFPSGameState* GameState = Cast<AMultiplayerFPSGameState>(World->GetGameState());
		if (!IsValid(GameState))
		{
			UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::RespawnPlayer(bool instant) -> GameState is not Valid !!!"), *this->GetName());
			return;
		}
		GetWorldTimerManager().SetTimer(RespawnHandle, this, &AMultiplayerFPSPlayerController::ServerRespawnPlayer, GameState->PlayerRespawnTime);
	}
}

void AMultiplayerFPSPlayerController::ServerRespawnPlayer_Implementation()
{
	if (!bShouldRespawn) return;

	GetWorldTimerManager().ClearTimer(RespawnHandle);
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> Does not have Authority !!!"), *this->GetName());
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> World is not Valid !!!"), *this->GetName());
		return;
	}

	AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> GameMode is not Valid !!!"), *this->GetName());
		return;
	}

	APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
	if (!IsValid(NewPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPawn is not Valid !!!"), *this->GetName());
		return;
	}

	AMultiplayerFPSCharacter* NewPlayerPawn = Cast<AMultiplayerFPSCharacter>(NewPawn);
	if (!IsValid(NewPlayerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPlayerPawn is not Valid !!!"), *this->GetName());
		return;
	}
	Possess(NewPlayerPawn);
}

void AMultiplayerFPSPlayerController::DisableControls_Implementation(bool disable)
{
	SetIgnoreMoveInput(disable);
	SetIgnoreLookInput(disable);
}

void AMultiplayerFPSPlayerController::KillPlayer()
{
	AMultiplayerFPSCharacter* PlayerPawn = Cast<AMultiplayerFPSCharacter>(GetPawn());
	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::ServerKillPlayer() -> PlayerPawn is not Valid !!!"), *this->GetName());
		return;
	}

	if (HasAuthority())
	{
		PlayerPawn->bDead = true;
		PlayerPawn->ClientDestoryPlayer();
	}

	PlayerPawn->DestoryPlayer();
	UnPossess();
}

