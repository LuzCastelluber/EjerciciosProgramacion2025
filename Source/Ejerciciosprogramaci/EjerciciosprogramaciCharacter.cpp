#include "EjerciciosprogramaciCharacter.h"                 // nuestro header
#include "GameFramework/CharacterMovementComponent.h"      // para modificar velocidades y salto
#include "Engine/Engine.h"                                 // para GEngine->AddOnScreenDebugMessage

// Constructor: define valores por defecto
AEjerciciosprogramaciCharacter::AEjerciciosprogramaciCharacter()
{
    // Configura velocidad inicial de caminata
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Configura saltos: 1 (básico) + extras (para doble salto si MaxExtraJumps = 1)
    JumpMaxCount = 1 + MaxExtraJumps;

    // Fuerza vertical del salto
    GetCharacterMovement()->JumpZVelocity = 600.f;

    // Tiempo máximo para mantener presionado el salto
    JumpMaxHoldTime = 0.25f;
}

// Se llama al comenzar el juego
void AEjerciciosprogramaciCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Asegura que la velocidad inicial sea la de caminar
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Configuración de los inputs (bindings)
void AEjerciciosprogramaciCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Acción MostrarMensaje (tecla M)
    PlayerInputComponent->BindAction("MostrarMensaje", IE_Pressed, this, &AEjerciciosprogramaciCharacter::OnMostrarMensaje);

    // Acción SaltoExtra (SpaceBar)
    PlayerInputComponent->BindAction("SaltoExtra", IE_Pressed, this, &AEjerciciosprogramaciCharacter::OnSaltoExtraPressed);
    PlayerInputComponent->BindAction("SaltoExtra", IE_Released, this, &AEjerciciosprogramaciCharacter::OnSaltoExtraReleased);

    // Acción Correr (LeftShift)
    PlayerInputComponent->BindAction("Correr", IE_Pressed, this, &AEjerciciosprogramaciCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Correr", IE_Released, this, &AEjerciciosprogramaciCharacter::StopSprinting);
}

// Muestra un mensaje en pantalla y log
void AEjerciciosprogramaciCharacter::OnMostrarMensaje()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                // ID automático
            2.0f,              // duración en segundos
            FColor::Green,     // color del texto
            TEXT("¡Acción MostrarMensaje ejecutada!") // texto
        );
    }

    UE_LOG(LogTemp, Display, TEXT("Acción MostrarMensaje"));
}

// Salto extra al presionar Space
void AEjerciciosprogramaciCharacter::OnSaltoExtraPressed()
{
    Jump();
}

// Detiene el salto sostenido al soltar Space
void AEjerciciosprogramaciCharacter::OnSaltoExtraReleased()
{
    StopJumping();
}

// Llamado al aterrizar
void AEjerciciosprogramaciCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    // Con JumpMaxCount nativo no necesitamos resetear manualmente nada
}

// Inicia sprint
void AEjerciciosprogramaciCharacter::StartSprinting()
{
    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = SprintSpeed;
    }
}

// Detiene sprint y vuelve a caminar
void AEjerciciosprogramaciCharacter::StopSprinting()
{
    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = WalkSpeed;
    }
}
