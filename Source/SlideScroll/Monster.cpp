// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetMesh(), TEXT("RightHand"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 860.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = EMonsterState::IdleCombat;
	SetStatus();
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginWeaponOverlap);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnColStartAttack.BindUFunction(this, FName("OnColStartAttack"));
	AnimInstance->OnColEndAttack.BindUFunction(this, FName("OnColEndAttack"));
}

void AMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AnimInstance->OnColStartAttack.Unbind();
	AnimInstance->OnColEndAttack.Unbind();
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlayerCharInfo();
	switch (CurrentState)
	{
	case EMonsterState::IdleReady:
		IdleReady(DeltaTime);
		break;
	case EMonsterState::IdleCombat:
		IdleCombat(DeltaTime);
		break;
	case EMonsterState::FollowPlayerChar:
		FollowPlayerChar(DeltaTime);
		break;
	case EMonsterState::AttackPlayerChar:
		AttackPlayerChar(DeltaTime);
		break;
	case EMonsterState::Death:
		Death(DeltaTime);
		break;
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::IdleReady(float DeltaTime)
{

}

void AMonster::IdleCombat(float DeltaTime)
{
	if (DistanceFromPlayerChar <= AggroRange)
	{
		CurrentState = EMonsterState::FollowPlayerChar;
	}
}

void AMonster::FollowPlayerChar(float DeltaTime)
{
	if (DistanceFromPlayerChar > AggroRange)
	{
		CurrentState = EMonsterState::IdleCombat;
	}
	else if (DistanceFromPlayerChar <= AttackRange)
	{
		CurrentState = EMonsterState::AttackPlayerChar;
	}
	else
	{
		AddMovementInput(DirectionToPlayerChar);
	}
}

void AMonster::AttackPlayerChar(float DeltaTime)
{
	if (AnimInstance->Montage_IsPlaying(AttackAnim))
	{
		return;
	}

	if (DistanceFromPlayerChar > AttackRange)
	{
		CurrentState = EMonsterState::FollowPlayerChar;
	}
	else
	{
		SetActorRotation(DirectionToPlayerChar.Rotation());
		PlayAnimMontage(AttackAnim);
	}
}

void AMonster::Death(float DeltaTime)
{

}

void AMonster::SetStatus()
{
	HP = 100.0f;
	AttackPower = 20.0f;
	AggroRange = 500.0f;
	AttackRange = 160.0f;
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ChangeDamageColor();

	HP = FMath::Max(HP - Damage, 0.0f);
	if (HP <= 0)
	{
		PlayAnimMontage(DeathAnim);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CurrentState = EMonsterState::Death;
	}

	UE_LOG(LogTemp, Warning, TEXT("AMonster TAKE DAMEGE %f hp %f"), Damage, HP);
	return Damage;
}

void AMonster::ChangeDamageColor()
{
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		}), 0.2f, false);
}

void AMonster::UpdatePlayerCharInfo()
{
	AMyCharacter* myCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector playerLocation = myCharacter->GetActorLocation();
	DirectionToPlayerChar = FVector(0.0f, (playerLocation - GetActorLocation()).Y, 0.0f);
	DistanceFromPlayerChar = DirectionToPlayerChar.Size();
	DirectionToPlayerChar.Normalize();
	IsDeathPlayerChar = myCharacter->IsDeath;

	if (IsDeathPlayerChar)
	{
		CurrentState = EMonsterState::IdleReady;
	}
}

void AMonster::OnColStartAttack()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMonster::OnColEndAttack()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonster::OnBeginWeaponOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this &&
		OtherActor->ActorHasTag("Player") &&
		OtherComp->ComponentHasTag(FName("Character")))
	{
		UGameplayStatics::ApplyDamage(OtherActor, AttackPower, GetController(), nullptr, NULL);
	}
}