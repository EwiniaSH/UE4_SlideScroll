// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BossMonster.h"
#include "BluePrint/UserWidget.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SLIDESCROLL_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EGamePlayState GetGamePlayState();

private:
	void StageClear();


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StageClearUI;

private:
	EGamePlayState GamePlayState;
	ABossMonster* BossMonster;
};
