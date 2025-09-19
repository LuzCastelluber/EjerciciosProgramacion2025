// Evita inclusiones m�ltiples del header
#pragma once

// Incluye el n�cleo de UE: tipos b�sicos, macros, etc.
#include "CoreMinimal.h"
// Incluye la clase base ACharacter, que ya trae movimiento, salto, etc.
#include "GameFramework/Character.h"
// Genera el c�digo de reflexi�n necesario para UCLASS/UPROPERTY/GENERATED_BODY
#include "EjerciciosprogramaciCharacter.generated.h"

// Declara una clase de Character para nuestro juego
UCLASS()
class EJERCICIOSPROGRAMACI_API AEjerciciosprogramaciCharacter : public ACharacter
{
    // Inserta c�digo que UE usa para construcci�n/reflexi�n de la clase
    GENERATED_BODY()

public:
    // Constructor: se usa para fijar valores por defecto de propiedades/componentes
    AEjerciciosprogramaciCharacter();

protected:
    // BeginPlay se ejecuta cuando el juego inicia o el actor aparece en el mundo
    virtual void BeginPlay() override;

    // SetupPlayerInputComponent es donde hacemos el "binding" de Inputs a funciones
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Landed se llama cuando el personaje aterriza; �til si luego quisieras l�gica al tocar el piso
    virtual void Landed(const FHitResult& Hit) override;

    // ----- Par�metros editables desde el editor -----

    // Velocidad normal de caminata (se puede ajustar en el editor sin recompilar)
    UPROPERTY(EditAnywhere, Category = "Movimiento")
    float WalkSpeed = 500.f;

    // Velocidad al correr/sprint (tambi�n editable)
    UPROPERTY(EditAnywhere, Category = "Movimiento")
    float SprintSpeed = 900.f;

    // Cantidad de saltos EXTRA (adem�s del salto base). 1 = doble salto
    UPROPERTY(EditAnywhere, Category = "Salto")
    int32 MaxExtraJumps = 1;

    // ----- Funciones que se bindean a las acciones -----

    // Muestra un mensaje en pantalla al presionar la tecla configurada
    void OnMostrarMensaje();

    // Inicia el sprint (sube la MaxWalkSpeed mientras est� presionado)
    void StartSprinting();

    // Detiene el sprint (restaura la velocidad normal)
    void StopSprinting();

    // Se llama al presionar el input de salto; usa el sistema de Jump del Character
    void OnSaltoExtraPressed();

    // Al soltar Space, detiene la acumulaci�n del salto (mantiene comportamiento esperado)
    void OnSaltoExtraReleased();
};
