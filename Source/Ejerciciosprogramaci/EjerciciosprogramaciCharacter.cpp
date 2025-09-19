// Incluye el header de esta clase para definir sus funciones
#include "EjerciciosprogramaciCharacter.h"
// Incluye el componente de movimiento del Character para editar velocidades, etc.
#include "GameFramework/CharacterMovementComponent.h"
// Permite mostrar mensajes on-screen vía GEngine->AddOnScreenDebugMessage
#include "Engine/Engine.h"

// Constructor: define valores por defecto al crear el personaje
AEjerciciosprogramaciCharacter::AEjerciciosprogramaciCharacter()
{
	// Asegura que el personaje pueda hacer el salto "sostenido" si se desea
	JumpMaxHoldTime = 0.25f;

	// Configura la velocidad de caminata inicial del Character
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Usa el sistema nativo de "multi-salto" del Character:
	// JumpMaxCount = 1 (salto base) + MaxExtraJumps (por ej., 1 => doble salto)
	JumpMaxCount = 1 + MaxExtraJumps;

	// Fija la fuerza vertical del salto (editable desde el Editor si lo prefieres)
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

// Se llama una vez cuando el juego empieza o el actor spawnea
void AEjerciciosprogramaciCharacter::BeginPlay()
{
	// Llama a la lógica base de ACharacter
	Super::BeginPlay();

	// Garantiza que la MaxWalkSpeed refleje el valor editable actual
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Hace el binding de acciones a funciones C++ (se ejecuta al poseer el pawn)
void AEjerciciosprogramaciCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Llama al Setup base para mantener cualquier binding de la superclase
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Acción "MostrarMensaje" (Project Settings → Input) al PRESIONAR
	PlayerInputComponent->BindAction("MostrarMensaje", IE_Pressed, this, &AEjerciciosprogramaciCharacter::OnMostrarMensaje);

	// Acción "SaltoExtra": usamos el salto nativo del Character
	PlayerInputComponent->BindAction("SaltoExtra", IE_Pressed, this, &AEjerciciosprogramaciCharacter::OnSaltoExtraPressed);
	PlayerInputComponent->BindAction("SaltoExtra", IE_Released, this, &AEjerciciosprogramaciCharacter::OnSaltoExtraReleased);

	// Acción "Correr": al presionar, sprint; al soltar, volver a caminar
	PlayerInputComponent->BindAction("Correr", IE_Pressed, this, &AEjerciciosprogramaciCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Correr", IE_Released, this, &AEjerciciosprogramaciCharacter::StopSprinting);
}

// Mensaje en pantalla al presionar la tecla de "MostrarMensaje"
void AEjerciciosprogramaciCharacter::OnMostrarMensaje()
{
	// Si el engine está disponible, imprime un mensaje verde por 2 segundos
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,              // ID automático (no reemplaza otros)
			2.0f,            // duración en segundos
			FColor::Green,   // color del texto
			TEXT("¡Acción MostrarMensaje ejecutada!")
		);
	}

	// También podrías registrar en Output Log:
	UE_LOG(LogTemp, Display, TEXT("Acción MostrarMensaje"));
}

// Al presionar Space ejecuta el salto del Character (con soporte de doble salto nativo)
void AEjerciciosprogramaciCharacter::OnSaltoExtraPressed()
{
	// Llama a Jump() de ACharacter; con JumpMaxCount>1 permite saltos en el aire
	Jump();
}

// Al soltar Space deja de "cargar" el salto
void AEjerciciosprogramaciCharacter::OnSaltoExtraReleased()
{
	// Detiene el salto sostenido (si estuviera activo)
	StopJumping();
}

// Aterrizar: aquí podrías resetear cosas si implementaras un sistema propio de saltos
void AEjerciciosprogramaciCharacter::Landed(const FHitResult& Hit)
{
	// Llama a la lógica base para mantener el comportamiento estándar
	Super::Landed(Hit);

	// Si más adelante implementas conteo manual de saltos, este es buen lugar para reset
	// (con JumpMaxCount nativo no hace falta hacer nada aquí)
}

// Empieza el sprint subiendo la MaxWalkSpeed
void AEjerciciosprogramaciCharacter::StartSprinting()
{
	// Cambia la velocidad máxima de caminata a la velocidad de sprint
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

// Detiene el sprint y vuelve a la velocidad normal
void AEjerciciosprogramaciCharacter::StopSprinting()
{
	// Restaura la velocidad de caminata
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
