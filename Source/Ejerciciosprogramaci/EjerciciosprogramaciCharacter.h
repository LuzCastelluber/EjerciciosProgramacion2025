// --------------------------------------------------------------------------------------
// Archivo: EjerciciosprogramaciCharacter.h
// Objetivo: Personaje con cámara básica y 3 acciones mapeadas desde C++.
// Cada línea está comentada justo arriba (Parte A del TP).
// --------------------------------------------------------------------------------------

#pragma once
// Evita múltiples inclusiones del header durante la compilación.

#include "CoreMinimal.h"
// Tipos/utilidades base de Unreal (FString, FVector, logs, etc.).

#include "GameFramework/Character.h"
// Clase base de personaje con movimiento, cápsula, etc.

#include "Camera/CameraComponent.h"
// Permite declarar y usar la cámara del jugador.

#include "GameFramework/SpringArmComponent.h"
// Permite usar el brazo de cámara (boom) con colisión y distancia.

#include "TimerManager.h"
// Permite usar timers (lo necesitamos para restaurar el FOV en AccionEspecial).

#include "EjerciciosprogramaciCharacter.generated.h"
// Macro obligatoria para generar el código de reflexión de Unreal.

// Declaro una categoría de logs propia para este Character.
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// UCLASS marca esta clase para el sistema de reflexión.
UCLASS()
class AEjerciciosprogramaciCharacter : public ACharacter
	// Heredamos de ACharacter para tener movimiento estándar.
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
	// ---- Componentes de cámara ----
	UPROPERTY(VisibleAnywhere, Category = "Cámara")
	USpringArmComponent* SpringArm;
	// Brazo que mantiene la cámara a una distancia con suavizado/colisión.

	UPROPERTY(VisibleAnywhere, Category = "Cámara")
	UCameraComponent* FollowCamera;
	// Cámara que sigue al personaje al final del SpringArm.

	// ---- Parámetros de movimiento ----
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float WalkSpeed = 300.f;
	// Velocidad de caminar (editable desde el editor).

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float RunSpeed = 650.f;
	// Velocidad de correr (editable desde el editor).

	// ---- Estado ----
	bool bIsRunning = false;
	// Flag para saber si estamos corriendo.

	// ---- Acciones (métodos) ----
	UFUNCTION()
	void AccionMostrarMensaje();
	// Muestra un mensaje en pantalla (Parte B: acción 1).

	UFUNCTION()
	void AccionAlternarCorrer();
	// Alterna entre caminar y correr (Parte B: acción 2).

	UFUNCTION()
	void AccionEspecial();
	// Cambia FOV temporalmente (Parte B: acción 3).

	// ---- Soporte de AccionEspecial ----
	float FOVOriginal = 90.f;
	// Guardamos FOV original para restaurarlo.

	UPROPERTY(EditAnywhere, Category = "Cámara")
	float FOVEspecial = 70.f;
	// FOV temporal cuando disparamos la acción especial.

	UPROPERTY(EditAnywhere, Category = "Cámara")
	float DuracionFOV = 0.75f;
	// Cuánto dura el FOV especial antes de restaurar.

	FTimerHandle TimerHandle_RestoreFOV;
	// Timer para volver al FOV original.

	// Función privada para restaurar el FOV tras el timer.
	void RestaurarFOV();

	// ---- Movimiento analógico (opcional útil) ----
	void MoveForward(float Value);
	// Avanza/retrocede con W/S o stick.

	void MoveRight(float Value);
	// Se mueve lateralmente con A/D o stick.

	void Turn(float Value);
	// Yaw con mouse/stick horizontal.

	void LookUp(float Value);
	// Pitch con mouse/stick vertical.
};
