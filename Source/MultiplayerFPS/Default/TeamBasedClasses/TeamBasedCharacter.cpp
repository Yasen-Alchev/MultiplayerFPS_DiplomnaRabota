#include "TeamBasedCharacter.h"

#include "TeamBasedPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ATeamBasedCharacter::ATeamBasedCharacter()
{
	myMaterial = nullptr;
	Team = TEAM_NONE;
}

void ATeamBasedCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaterialChange();
}

void ATeamBasedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeamBasedCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamBasedCharacter, Team);
}


void ATeamBasedCharacter::InitTeam()
{
	ATeamBasedPlayerState* PlayerStateVar = Cast<ATeamBasedPlayerState>(GetPlayerState());
	if (IsValid(PlayerStateVar))
	{
		this->Team = PlayerStateVar->Team;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedCharacter::InitTeam(AActor * Player)->PlayerStateVar is not Valid !!!"), *PlayerName);
	}
	MaterialChange();
}

void ATeamBasedCharacter::MaterialChange()
{
	if (Team != TEAM_NONE)
	{
		if (Team == TEAM_BLUE)
		{
			myMaterial = FullBodyMesh->CreateDynamicMaterialInstance(0, TeamBlueSkin);
		}
		else
		{
			myMaterial = FullBodyMesh->CreateDynamicMaterialInstance(0, TeamRedSkin);
		}
		if (IsValid(myMaterial))
		{
			FullBodyMesh->SetMaterial(0, myMaterial);
			FirstPersonMesh->SetMaterial(0, myMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedCharacter::MaterialChange()-> MaterialInstance is not Valid!!!"), *PlayerName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ATeamBasedCharacter::MaterialChange() -> Team is TEAM_NONE"), *PlayerName);
	}
}

