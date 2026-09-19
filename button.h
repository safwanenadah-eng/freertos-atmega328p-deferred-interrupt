#ifndef BUTTON_H
#define BUTTON_H

#include "FreeRTOS.h"
#include "semphr.h"

// Variable globale externe du Sémaphore Binaire
extern xSemaphoreHandle xButtonSemaphore;

// Initialisation de la broche PD2 (INT0) et configuration de l'interruption
void Button_Init(void);

#endif /* BUTTON_H */