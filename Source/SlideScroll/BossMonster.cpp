// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "Kismet/GameplayStatics.h"
#include "BossMonsterAnimInstance.h"

ABossMonster::ABossMonster()
{
	SkillCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SkillCollision"));
	SkillCollision->SetupAttachment(GetMesh(), TEXT("LeftHand"));
}

void ABossMonster::PostInitializeComponents()
{
	Super::Super::PostInitializeComponents();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossMonster::OnBeginWeaponOverlap);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnColStartAttack.BindUFunction(this, FName("OnColStartAttack"));
	AnimInstance->OnColEndAttack.BindUFunction(this, FName("OnColEndAttack"));

	// Only Boss
	SkillCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossMonster::OnBeginSkillOverlap);
	SkillCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UBossMonsterAnimInstance* bossMonsterAnimInstance = Cast<UBossMonsterAnimInstance>(AnimInstance);
	bossMonsterAnimInstance->OnColStartSkill.BindUFunction(this, FName("OnColStartSkill"));
	bossMonsterAnimInstance->OnColEndSkill.BindUFunction(this, FName("OnColEndSkill"));
}

void ABossMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UBossMonsterAnimInstance* bossMonsterAnimInstance = Cast<UBossMonsterAnimInstance>(AnimInstance);
	bossMonsterAnimInstance->OnColStartSkill.Unbind();
	bossMonsterAnimInstance->OnColEndSkill.Unbind();
}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Super::Tick(DeltaTime);

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
	case EMonsterState::SkillAttackPlayerChar:
		SkillAttackPlayerChar(DeltaTime);
		break;
	case EMonsterState::Death:
		Death(DeltaTime);
		break;
	}
}

void ABossMonster::FollowPlayerChar(float DeltaTime)
{
	if (DistanceFromPlayerChar > AggroRange)
	{
		SkillDeltaTime = 0.0f;
		CurrentState = EMonsterState::IdleCombat;
	}
	else
	{
		SkillDeltaTime += DeltaTime;
		if (DistanceFromPlayerChar <= AttackRange)
		{
			CurrentState = SkillDeltaTime < SkillCoolTime ? EMonsterState::AttackPlayerChar : EMonsterState::SkillAttackPlayerChar;
		}
		else
		{
			AddMovementInput(DirectionToPlayerChar);
		}
	}
}

void ABossMonster::AttackPlayerChar(float DeltaTime)
{
	SkillDeltaTime += DeltaTime;
	if (SkillDeltaTime >= SkillCoolTime)
	{
		CurrentState = EMonsterState::SkillAttackPlayerChar;
		return;
	}

	if (!AnimInstance->Montage_IsPlaying(AttackAnim) &&
		!AnimInstance->Montage_IsPlaying(SkillReadyAnim) &&
		!AnimInstance->Montage_IsPlaying(SkillAttackAnim))
	{
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
}

void ABossMonster::SkillAttackPlayerChar(float DeltaTime)
{
	if (!AnimInstance->Montage_IsPlaying(AttackAnim) &&
		!AnimInstance->Montage_IsPlaying(SkillReadyAnim) &&
		!AnimInstance->Montage_IsPlaying(SkillAttackAnim))
	{
		if (DistanceFromPlayerChar > AttackRange)
		{
			CurrentState = EMonsterState::FollowPlayerChar;
		}
		else
		{
			ChangeSkillColor();
			SetActorRotation(DirectionToPlayerChar.Rotation());
			PlayAnimMontage(SkillReadyAnim, 1.0f, FName("Start"));
			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (HP > 0)
					{
						PlayAnimMontage(SkillAttackAnim);
					}
				}), 0.8f, false);

			SkillDeltaTime = 0.0f;
			CurrentState = EMonsterState::AttackPlayerChar;
		}
	}
}

void ABossMonster::SetStatus()
{
	HP = 150.0f;
	AttackPower = 20.0f;
	AggroRange = 500.0f;
	AttackRange = 120.0f;

	// Only Boss
	SkillPower = 40.0f;
	SkillCoolTime = 3.0f;
}

void ABossMonster::ChangeSkillColor()
{
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(0.3f, 0.3f, 1.0f, 1.0f)));
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		}), 3.0f, false);
}

void ABossMonster::OnColStartSkill()
{
	SkillCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABossMonster::OnColEndSkill()
{
	SkillCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossMonster::OnBeginSkillOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this &&
		OtherActor->ActorHasTag("Player") &&
		OtherComp->ComponentHasTag(FName("Character")))
	{
		UGameplayStatics::ApplyDamage(OtherActor, AttackPower, GetController(), nullptr, NULL);
	}
}