// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// --- Tipos b�sicos y utilidades de Unreal
#include "CoreMinimal.h"

// --- Clase base de personaje
#include "GameFramework/Character.h"

// --- Logs del template
#include "Logging/LogMacros.h"

// --- Necesario porque en este header guardamos un FTimerHandle como miembro
#include "TimerManager.h"

#include "EjerciciosprogramaciCharacter.generated.h"

// ---------- Forward declarations de componentes / input ----------
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

// ---------- Categor�a de logs del template ----------
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  Personaje third person controlable por el jugador,
 *  con c�mara orbitando (template de UE5)
 */
UCLASS(abstract)
class AEjerciciosprogramaciCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// ---------- Constructor ----------
	AEjerciciosprogramaciCharacter();

protected:
	// ---------- Ciclo de vida ----------
	virtual void BeginPlay() override;

	// ---------- Enlace de entradas (acciones/ejes) ----------
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ================== INPUTS DEL TEMPLATE (Enhanced Input) ==================
	/** Acci�n de salto (template) */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Acci�n de movimiento (template) */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Acci�n de mirar (template) */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Acci�n de mirar con mouse (template) */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

protected:
	/** Llamada por el sistema de input para mover */
	void Move(const FInputActionValue& Value);

	/** Llamada por el sistema de input para mirar */
	void Look(const FInputActionValue& Value);

public:
	/** Movimiento desde UI/control (expuesto a Blueprint) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Mirar desde UI/control (expuesto a Blueprint) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Inicio de salto desde UI/control (expuesto a Blueprint) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Fin de salto desde UI/control (expuesto a Blueprint) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	// ================== COMPONENTES DE C�MARA ==================
	/** Brazo de c�mara (coloca la c�mara detr�s del personaje) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** C�mara que sigue al personaje */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// ================== ACCIONES NUEVAS (Parte B) ==================
public:
	// --- Acci�n: al presionar "M" se muestra un mensaje en pantalla
	void AccionMostrarMensaje();

	// --- Acci�n: al presionar "SpaceBar" se realiza un salto m�s alto temporalmente
	void AccionSaltoExtra();

	// --- Acci�n: al mantener "LeftShift" se aumenta la velocidad (correr)
	void AccionCorrer_Pressed();

	// --- Acci�n: al soltar "LeftShift" se vuelve a la velocidad normal (caminar)
	void AccionCorrer_Released();

protected:
	// ================== PAR�METROS EDITABLES DESDE EL EDITOR ==================
	// --- Velocidad de caminar por defecto (c�mbiala en el Editor para probar sin recompilar)
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadCaminar = 300.f;

	// --- Velocidad al correr (c�mbiala en el Editor)
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float VelocidadCorrer = 650.f;

	// --- Multiplicador del salto extra (1.5 = 50% m�s alto)
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float FactorSaltoExtra = 1.5f;

	// --- Guardamos el JumpZVelocity original para restaurarlo despu�s del salto extra
	float JumpZOriginal = 600.f;

	// --- Timer para restaurar el JumpZVelocity tras el SaltoExtra
	FTimerHandle TimerHandle_RestoreJumpZ;

public:
	// ================== GETTERS DE COMPONENTES ==================
	/** Devuelve el subobjeto CameraBoom */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Devuelve el subobjeto FollowCamera */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
void AEjerciciosprogramaciCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MostrarMensaje", IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionMostrarMensaje);
	PlayerInputComponent->BindAction("SaltoExtra", IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionSaltoExtra);
	PlayerInputComponent->BindAction("Correr", IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionCorrer_Pressed);
	PlayerInputComponent->BindAction("Correr", IE_Released, this, &AEjerciciosprogramaciCharacter::AccionCorrer_Released);
}
void AEjerciciosprogramaciCharacter::AccionMostrarMensaje()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("�Acci�n MostrarMensaje ejecutada!"));
	}
}

void AEjerciciosprogramaciCharacter::AccionSaltoExtra()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->JumpZVelocity = JumpZOriginal * FactorSaltoExtra;
		Jump();

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

void AEjerciciosprogramaciCharacter::AccionCorrer_Pressed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = VelocidadCorrer;
	}
}

void AEjerciciosprogramaciCharacter::AccionCorrer_Released()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = VelocidadCaminar;
	}
}
