#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
