#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerController.h"
#include "CQ_PlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_PlayerController : public AMultiplayerFPSPlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
};
