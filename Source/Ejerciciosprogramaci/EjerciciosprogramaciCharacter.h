// --------------------------------------------------------------------------------------
// Archivo: EjerciciosprogramaciCharacter.h
// Objetivo: Personaje con c�mara b�sica y 3 acciones mapeadas desde C++.
// Cada l�nea est� comentada justo arriba (Parte A del TP).
// --------------------------------------------------------------------------------------

#pragma once
// Evita m�ltiples inclusiones del header durante la compilaci�n.

#include "CoreMinimal.h"
// Tipos/utilidades base de Unreal (FString, FVector, logs, etc.).

#include "GameFramework/Character.h"
// Clase base de personaje con movimiento, c�psula, etc.

#include "Camera/CameraComponent.h"
// Permite declarar y usar la c�mara del jugador.

#include "GameFramework/SpringArmComponent.h"
// Permite usar el brazo de c�mara (boom) con colisi�n y distancia.

#include "TimerManager.h"
// Permite usar timers (lo necesitamos para restaurar el FOV en AccionEspecial).

#include "EjerciciosprogramaciCharacter.generated.h"
// Macro obligatoria para generar el c�digo de reflexi�n de Unreal.

// Declaro una categor�a de logs propia para este Character.
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// UCLASS marca esta clase para el sistema de reflexi�n.
UCLASS()
class AEjerciciosprogramaciCharacter : public ACharacter
	// Heredamos de ACharacter para tener movimiento est�ndar.
{
	GENERATED_BODY()
	// Inserta boilerplate requerido por Unreal.

public:
	// Constructor: creo componentes y valores por defecto.
	AEjerciciosprogramaciCharacter();

protected:
	// BeginPlay: se ejecuta al empezar el juego o al spawnear.
	virtual void BeginPlay() override;

	// Enlaces de entradas: donde bindeamos teclas/acciones a funciones C++.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// ---- Componentes de c�mara ----
	UPROPERTY(VisibleAnywhere, Category = "C�mara")
	USpringArmComponent* SpringArm;
	// Brazo que mantiene la c�mara a una distancia con suavizado/colisi�n.

	UPROPERTY(VisibleAnywhere, Category = "C�mara")
	UCameraComponent* FollowCamera;
	// C�mara que sigue al personaje al final del SpringArm.

	// ---- Par�metros de movimiento ----
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float WalkSpeed = 300.f;
	// Velocidad de caminar (editable desde el editor).

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float RunSpeed = 650.f;
	// Velocidad de correr (editable desde el editor).

	// ---- Estado ----
	bool bIsRunning = false;
	// Flag para saber si estamos corriendo.

	// ---- Acciones (m�todos) ----
	UFUNCTION()
	void AccionMostrarMensaje();
	// Muestra un mensaje en pantalla (Parte B: acci�n 1).

	UFUNCTION()
	void AccionAlternarCorrer();
	// Alterna entre caminar y correr (Parte B: acci�n 2).

	UFUNCTION()
	void AccionEspecial();
	// Cambia FOV temporalmente (Parte B: acci�n 3).

	// ---- Soporte de AccionEspecial ----
	float FOVOriginal = 90.f;
	// Guardamos FOV original para restaurarlo.

	UPROPERTY(EditAnywhere, Category = "C�mara")
	float FOVEspecial = 70.f;
	// FOV temporal cuando disparamos la acci�n especial.

	UPROPERTY(EditAnywhere, Category = "C�mara")
	float DuracionFOV = 0.75f;
	// Cu�nto dura el FOV especial antes de restaurar.

	FTimerHandle TimerHandle_RestoreFOV;
	// Timer para volver al FOV original.

	// Funci�n privada para restaurar el FOV tras el timer.
	void RestaurarFOV();

	// ---- Movimiento anal�gico (opcional �til) ----
	void MoveForward(float Value);
	// Avanza/retrocede con W/S o stick.

	void MoveRight(float Value);
	// Se mueve lateralmente con A/D o stick.

	void Turn(float Value);
	// Yaw con mouse/stick horizontal.

	void LookUp(float Value);
	// Pitch con mouse/stick vertical.
};
