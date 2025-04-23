// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_BadBotGamemode.generated.h"

/**
 * 
 */
UCLASS()
class BADBOT_CPP_API ACPP_BadBotGamemode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int PlayerScore = 0;

public:
	UFUNCTION()
	void IncrementScore();
};
