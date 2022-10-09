// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

const float AMonster::AGGRO_RANGE = 500.0f;

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 860.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = EMonsterState::Idle;
	SetStatus();
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetDistanceAndDirection();
	switch (CurrentState)
	{
	case EMonsterState::Idle:
		Idle();
		break;
	case EMonsterState::FollowPlayerChar:
		FollowPlayerChar();
		break;
	case EMonsterState::AttackPlayerChar:
		AttackPlayerChar();
		break;
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::Idle()
{
	if (DistanceFromPlayerChar <= AGGRO_RANGE)
	{
		CurrentState = EMonsterState::FollowPlayerChar;
	}
}

void AMonster::FollowPlayerChar()
{
	if (DistanceFromPlayerChar > AGGRO_RANGE)
	{
		CurrentState = EMonsterState::Idle;
	}

	AddMovementInput(DirectionToPlayerChar);
}

void AMonster::AttackPlayerChar()
{

}

void AMonster::SetStatus()
{
	HP = 100.0f;
	AttackPower = 20.0f;
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ChangeDamageColor();

	HP = FMath::Max(HP - Damage, 0.0f);
	if (HP <= 0)
	{
		PlayAnimMontage(DeathAnim);
	}

	return Damage;
}

void AMonster::ChangeDamageColor()
{
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		}), 0.1f, false);
}

void AMonster::SetDistanceAndDirection()
{
	FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	DirectionToPlayerChar = FVector(0.0f, (playerLocation - GetActorLocation()).Y, 0.0f);
	DistanceFromPlayerChar = DirectionToPlayerChar.Size();
	DirectionToPlayerChar.Normalize();

	//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceFromPlayerChar);
}