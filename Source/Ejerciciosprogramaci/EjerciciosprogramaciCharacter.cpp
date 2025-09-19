// Copyright Epic Games, Inc.

#include "EjerciciosprogramaciCharacter.h"

// Componentes de cámara y boom
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Movimiento y entrada
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Utilidades
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

// -----------------------------
// Constructor
// -----------------------------
AEjerciciosprogramaciCharacter::AEjerciciosprogramaciCharacter()
{
	// Llamar Tick() cada frame (útil si más adelante querés lógica por frame)
	PrimaryActorTick.bCanEverTick = true;

	// ==== Configuración del Character ====
	// El personaje mira hacia donde se mueve (en lugar de usar rotación del controlador)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configura el movimiento para orientarse a la dirección del input
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;          // gira hacia la dirección de movimiento
		Move->RotationRate = FRotator(0.f, 500.f, 0.f);  // rapidez al girar
		Move->MaxWalkSpeed = VelocidadCaminar;           // velocidad inicial (editable en el Editor)
		// Guardamos el JumpZ original para poder restaurarlo tras SaltoExtra
		JumpZOriginal = Move->JumpZVelocity;
	}

	// ==== Componentes de cámara ====
	// Brazo de cámara (boom) que coloca la cámara por detrás del personaje
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;           // distancia de la cámara al personaje
	CameraBoom->bUsePawnControlRotation = true;    // boom usa la rotación del controlador (mouse/gamepad)

	// Cámara que sigue al personaje (anclada al boom)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // la cámara no rota por sí misma (ya lo hace el boom)
}

// -----------------------------
// BeginPlay
// -----------------------------
void AEjerciciosprogramaciCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Si alguna vez cambiás VelocidadCaminar en el editor antes de Play,
	// asegurate que el movimiento refleje ese valor al iniciar:
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = VelocidadCaminar;
	}
}

// -----------------------------
// Setup de Input (BINDINGS)
// -----------------------------
void AEjerciciosprogramaciCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// =========================
	// PARTE B — TUS 3 ACCIONES
	// =========================

	// 1) MostrarMensaje → tecla M (evento Presionado)
	PlayerInputComponent->BindAction(
		TEXT("MostrarMensaje"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionMostrarMensaje
	);

	// 2) SaltoExtra → SpaceBar (evento Presionado)
	PlayerInputComponent->BindAction(
		TEXT("SaltoExtra"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionSaltoExtra
	);

	// 3) Correr → LeftShift (Presionado/Soltado)
	PlayerInputComponent->BindAction(
		TEXT("Correr"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionCorrer_Pressed
	);
	PlayerInputComponent->BindAction(
		TEXT("Correr"), IE_Released, this, &AEjerciciosprogramaciCharacter::AccionCorrer_Released
	);

	// =========================
	// (Opcional) Si usaras Axis/Action clásicos para mover/mirar,
	// podrías hacer BindAxis aquí. En este template tenemos funciones
	// Move/Look y Do* por si usás UI o Enhanced Input desde BP.
	// =========================
}

// =====================================================
// IMPLEMENTACIÓN — ACCIONES PERSONALIZADAS (Parte B)
// =====================================================

// M → imprime mensaje en pantalla por 3 segundos
void AEjerciciosprogramaciCharacter::AccionMostrarMensaje()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,              // id (-1 = siempre un nuevo mensaje)
			3.f,             // duración
			FColor::Yellow,  // color
			TEXT("¡Acción MostrarMensaje ejecutada!")
		);
	}
}

// SpaceBar → salto extra: sube JumpZ temporalmente y salta; luego restaura
void AEjerciciosprogramaciCharacter::AccionSaltoExtra()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		// Aumenta salto según FactorSaltoExtra (editable en el editor)
		Move->JumpZVelocity = JumpZOriginal * FactorSaltoExtra;

		// Ejecuta un salto
		Jump();

		// Restaura JumpZ a los 0.5 segundos
		GetWorldTimerManager().SetTimer(
			TimerHandle_RestoreJumpZ,
			[this]()
			{
				if (UCharacterMovementComponent* M = GetCharacterMovement())
				{
					M->JumpZVelocity = JumpZOriginal;
				}
			},
			0.5f, false
		);
	}
}

// LeftShift PRESIONADO → corre (sube MaxWalkSpeed)
void AEjerciciosprogramaciCharacter::AccionCorrer_Pressed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = VelocidadCorrer;
	}
}

// LeftShift SOLTADO → vuelve a caminar (restaura MaxWalkSpeed)
void AEjerciciosprogramaciCharacter::AccionCorrer_Released()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = VelocidadCaminar;
	}
}

// =====================================================
// TEMPLATE — Funciones de movimiento/mirada y variantes
// (Se implementan para que el template compile y,
// si las invocás desde UI/Blueprints, funcionen.)
// =====================================================

// Llamada por el sistema de input (Enhanced o similar) para mover
void AEjerciciosprogramaciCharacter::Move(const FInputActionValue& Value)
{
	// Value es un Vector2D: (X = Forward, Y = Right) según el mapeo del template
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (Controller)
	{
		// Dirección hacia adelante y derecha basada en la rotación del controlador (Yaw)
		const FRotator ControlRot = Controller->GetControlRotation();
		const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

		const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, MoveVector.Y);
		AddMovementInput(RightDir, MoveVector.X);
	}
}

// Llamada por el sistema de input para mirar
void AEjerciciosprogramaciCharacter::Look(const FInputActionValue& Value)
{
	// Value es un Vector2D: (X = Yaw, Y = Pitch)
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

// Expuesto a BP/UI: mueve en ejes Right/Forward (útil para UI virtual sticks)
void AEjerciciosprogramaciCharacter::DoMove(float Right, float Forward)
{
	if (Controller)
	{
		const FRotator ControlRot = Controller->GetControlRotation();
		const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

		const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, Forward);
		AddMovementInput(RightDir, Right);
	}
}

// Expuesto a BP/UI: aplica mirada (útil para UI sliders)
void AEjerciciosprogramaciCharacter::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

// Expuesto a BP/UI: iniciar salto
void AEjerciciosprogramaciCharacter::DoJumpStart()
{
	Jump();
}

// Expuesto a BP/UI: terminar salto
void AEjerciciosprogramaciCharacter::DoJumpEnd()
{
	StopJumping();
}
