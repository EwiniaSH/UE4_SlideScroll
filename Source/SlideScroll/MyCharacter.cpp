// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Player"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bInheritYaw = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetMesh(), TEXT("RightHand"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 860.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	IsDeath = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetStatus();
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginWeaponOverlap);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance = Cast<UMyCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnColStartAttack.BindUFunction(this, FName("OnColStartAttack"));
	AnimInstance->OnColEndAttack.BindUFunction(this, FName("OnColEndAttack"));
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AnimInstance->OnColStartAttack.Unbind();
	AnimInstance->OnColEndAttack.Unbind();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxis", this, &AMyCharacter::Movement);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping);
}

void AMyCharacter::SetStatus()
{
	HP = 200.0f;
	AttackPower = 50.0f;
}

void AMyCharacter::Movement(float Value)
{
	if (!AnimInstance->Montage_IsPlaying(AttackAnim))
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	}
}

void AMyCharacter::Attack()
{
	if (!AnimInstance->Montage_IsPlaying(AttackAnim))
	{
		PlayAnimMontage(AttackAnim);
	}
}

void AMyCharacter::Jump()
{
	Super::Jump();

	if (!AnimInstance->Montage_IsPlaying(JumpAnim))
	{
		PlayAnimMontage(JumpAnim);
	}
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (AnimInstance->Montage_IsPlaying(JumpAnim))
	{
		StopAnimMontage(JumpAnim);
	}
}


float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ChangeDamageColor();

	HP = FMath::Max(HP - Damage, 0.0f);
	if (HP <= 0)
	{
		IsDeath = true;
		PlayAnimMontage(DeathAnim);
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	UE_LOG(LogTemp, Warning, TEXT("PLAYER TAKE DAMEGE %f hp %f"), Damage, HP);
	return Damage;
}

void AMyCharacter::ChangeDamageColor()
{
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetVectorParameterValueOnMaterials(TEXT("MainColor"), FVector(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		}), 0.2f, false);
}

void AMyCharacter::OnColStartAttack()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMyCharacter::OnColEndAttack()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMyCharacter::OnBeginWeaponOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this &&
		OtherActor->ActorHasTag("Monster") &&
		OtherComp->ComponentHasTag(FName("Character")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("OverLap %s"), *OtherActor->GetName());
		UGameplayStatics::ApplyDamage(OtherActor, AttackPower, GetController(), nullptr, NULL);
	}
}