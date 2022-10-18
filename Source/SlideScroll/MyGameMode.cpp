// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyGameMode::AMyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* Actor : GetLevel()->Actors)
	{
		if (Actor->ActorHasTag(TEXT("Boss")))
		{
			BossMonster = Cast<ABossMonster>(Actor);
			break;
		}
	}

	GamePlayState = EGamePlayState::Play;
}

// Called every frame
void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GamePlayState == EGamePlayState::Play)
	{
		if (BossMonster->GetMonsterState() == EMonsterState::Death)
		{
			StageClear();
		}
	}
}

EGamePlayState AMyGameMode::GetGamePlayState()
{
	return GamePlayState;
}

void AMyGameMode::StageClear()
{
	UUserWidget* clearWidget = CreateWidget(GetWorld(), StageClearUI);
	clearWidget->AddToViewport();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerController != nullptr && playerCharacter != nullptr)
	{
		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
		playerCharacter->DisableInput(playerController);
	}

	GamePlayState = EGamePlayState::StageClear;
}