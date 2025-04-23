
#include "CPP_BadBotGamemode.h"

void ACPP_BadBotGamemode::IncrementScore()
{
	PlayerScore++;
	UE_LOG(LogTemp, Display, TEXT("Score: %d"), PlayerScore);
}

