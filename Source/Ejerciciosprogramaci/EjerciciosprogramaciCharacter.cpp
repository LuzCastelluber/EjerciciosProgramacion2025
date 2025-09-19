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
// --------------------------------------------------------------------------------------
// Archivo: EjerciciosprogramaciCharacter.cpp
// Implementación del personaje con cámara, movimiento y 3 acciones en C++.
// Todas las líneas están comentadas arriba (Parte A).
// --------------------------------------------------------------------------------------

#include "EjerciciosprogramaciCharacter.h"
// Incluyo mi propio header para definir los métodos de la clase.

#include "GameFramework/CharacterMovementComponent.h"
// Para acceder a GetCharacterMovement() y configurar velocidades.

#include "Components/InputComponent.h"
// Para bindeos de input con UInputComponent.

#include "Engine/Engine.h"
// Para GEngine->AddOnScreenDebugMessage.

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
// Defino la categoría de logs declarada en el header.

// Constructor: creo componentes y seteo defaults.
AEjerciciosprogramaciCharacter::AEjerciciosprogramaciCharacter()
{
	// Creo el SpringArm como subobjeto del Character.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// Adjunto el brazo al RootComponent (cápsula del Character).
	SpringArm->SetupAttachment(RootComponent);
	// Distancia de la cámara desde el personaje.
	SpringArm->TargetArmLength = 400.f;
	// Permite que el brazo use el control del jugador para rotar.
	SpringArm->bUsePawnControlRotation = true;
	// Habilita colisión del boom para no atravesar paredes.
	SpringArm->bDoCollisionTest = true;

	// Creo la cámara y la adjunto al final del SpringArm.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// La cámara se adjunta al socket del final del brazo.
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// La cámara NO rota con el control (deja esa tarea al boom).
	FollowCamera->bUsePawnControlRotation = false;
	// FOV por defecto (lo guardamos también en FOVOriginal en BeginPlay).
	FollowCamera->FieldOfView = 90.f;

	// Configuro la rotación del Character respecto al movimiento (gira hacia donde se mueve).
	bUseControllerRotationYaw = false;
	// El movimiento orienta la rotación del Character hacia la dirección de entrada.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Velocidad inicial como caminar.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	// Multiplicador de giro al moverse (suavidad).
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

// BeginPlay: inicializo valores que dependen de componentes ya creados.
void AEjerciciosprogramaciCharacter::BeginPlay()
{
	// Llamo a la implementación base (buena práctica).
	Super::BeginPlay();

	// Guardo el FOV actual como original para restaurarlo luego.
	if (FollowCamera)
	{
		FOVOriginal = FollowCamera->FieldOfView;
	}

	// Log de confirmación en la salida de Output Log.
	UE_LOG(LogTemplateCharacter, Log, TEXT("Character listo. Velocidad actual: %.1f"), GetCharacterMovement()->MaxWalkSpeed);
}

// Setup de inputs: mapea teclas a funciones C++.
void AEjerciciosprogramaciCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Llamo al padre para asegurar comportamiento base.
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ---- Ejes de movimiento (WSAD/Arrows) ----
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEjerciciosprogramaciCharacter::MoveForward);
	// Eje vertical: W/S o Up/Down para avanzar/retroceder.

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEjerciciosprogramaciCharacter::MoveRight);
	// Eje horizontal: A/D o Left/Right para moverse lateralmente.

	// ---- Ejes de cámara (mouse o stick derecho) ----
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEjerciciosprogramaciCharacter::Turn);
	// Girar en yaw (mouse X).

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEjerciciosprogramaciCharacter::LookUp);
	// Mirar en pitch (mouse Y).

	// ----------------------------------------------------------------------------------
	// PARTE B — Acciones en C++ (creá las Action Mappings en Project Settings → Input)
	// 1) Action Mapping: "MostrarMensaje" (tecla sugerida: M)
	// 2) Action Mapping: "AlternarCorrer" (tecla sugerida: LeftShift)
	// 3) Action Mapping: "AccionEspecial" (tecla sugerida: Q)
	// ----------------------------------------------------------------------------------

	// Bind de acción 1: al presionar, mostrar mensaje.
	PlayerInputComponent->BindAction(TEXT("MostrarMensaje"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionMostrarMensaje);

	// Bind de acción 2: al presionar, alternar velocidad caminar/correr.
	PlayerInputComponent->BindAction(TEXT("AlternarCorrer"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionAlternarCorrer);

	// Bind de acción 3: al presionar, activar FOV especial temporal.
	PlayerInputComponent->BindAction(TEXT("AccionEspecial"), IE_Pressed, this, &AEjerciciosprogramaciCharacter::AccionEspecial);
}

// Movimiento hacia adelante/atrás.
void AEjerciciosprogramaciCharacter::MoveForward(float Value)
{
	// Si hay input (Value != 0), agrego movimiento en la dirección forward del controlador.
	if (Controller && Value != 0.f)
	{
		// Obtengo rotación del controlador (cámara).
		const FRotator ControlRot = Controller->GetControlRotation();
		// Construyo rotación solo en yaw (plano horizontal).
		const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);
		// Convierto yaw a vector forward.
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		// Añado movimiento en esa dirección, escalado por Value.
		AddMovementInput(Direction, Value);
	}
}

// Movimiento lateral.
void AEjerciciosprogramaciCharacter::MoveRight(float Value)
{
	// Si hay input lateral, muevo a la derecha/izquierda.
	if (Controller && Value != 0.f)
	{
		// Rotación del controlador.
		const FRotator ControlRot = Controller->GetControlRotation();
		// Yaw solamente.
		const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);
		// Vector derecha (eje Y).
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		// Aplico movimiento lateral.
		AddMovementInput(Direction, Value);
	}
}

// Giro en yaw con mouse/stick.
void AEjerciciosprogramaciCharacter::Turn(float Value)
{
	// Añado entrada de yaw (rotación horizontal) si hay valor.
	AddControllerYawInput(Value);
}

// Elevación en pitch con mouse/stick.
void AEjerciciosprogramaciCharacter::LookUp(float Value)
{
	// Añado entrada de pitch (rotación vertical) si hay valor.
	AddControllerPitchInput(Value);
}

// Acción 1: mostrar un mensaje en pantalla.
void AEjerciciosprogramaciCharacter::AccionMostrarMensaje()
{
	// Verifico que GEngine exista (contexto de editor/juego).
	if (GEngine)
	{
		// Muestro un mensaje por 2 segundos (Key -1 para que no reemplace otros).
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("¡Hola! Acción: MostrarMensaje"));
	}

	// También lo mando al Output Log vía UE_LOG.
	UE_LOG(LogTemplateCharacter, Log, TEXT("AccionMostrarMensaje ejecutada"));
}

// Acción 2: alternar entre caminar y correr.
void AEjerciciosprogramaciCharacter::AccionAlternarCorrer()
{
	// Invierto el estado de correr.
	bIsRunning = !bIsRunning;

	// Elijo velocidad según el estado.
	const float NewSpeed = bIsRunning ? RunSpeed : WalkSpeed;

	// Seteo la velocidad máxima de caminata del CharacterMovement.
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	// Mensaje en pantalla para feedback rápido.
	if (GEngine)
	{
		const FString Texto = bIsRunning ? TEXT("Corriendo") : TEXT("Caminando");
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, Texto);
	}

	// Log informativo.
	UE_LOG(LogTemplateCharacter, Log, TEXT("Velocidad actual: %.1f"), NewSpeed);
}

// Acción 3: FOV especial temporal (zoom).
void AEjerciciosprogramaciCharacter::AccionEspecial()
{
	// Si hay cámara válida, aplico FOV especial y arranco un timer para restaurar.
	if (FollowCamera)
	{
		// Aplico el FOV reducido/ampliado (editable en el editor).
		FollowCamera->SetFieldOfView(FOVEspecial);

		// Seteo un timer que llamará a RestaurarFOV luego de DuracionFOV segundos.
		GetWorldTimerManager().SetTimer(TimerHandle_RestoreFOV, this, &AEjerciciosprogramaciCharacter::RestaurarFOV, DuracionFOV, false);

		// Mensaje de feedback.
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Yellow, TEXT("Acción Especial: FOV temporal"));
		}
	}
}

// Restaura el FOV de la cámara al valor original.
void AEjerciciosprogramaciCharacter::RestaurarFOV()
{
	// Si hay cámara válida, restauro el FOV original guardado.
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(FOVOriginal);
	}
}
