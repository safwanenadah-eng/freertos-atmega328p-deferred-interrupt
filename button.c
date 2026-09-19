#include <avr/io.h>
#include <avr/interrupt.h>
#include "button.h"
#include "task.h"

xSemaphoreHandle xButtonSemaphore = NULL;

void Button_Init(void)
{
    // 1. Création du sémaphore binaire
    vSemaphoreCreateBinary(xButtonSemaphore);

    if (xButtonSemaphore != NULL)
    {
        // Consommer le sémaphore initialement pour qu'il soit vide (état 0)
        xSemaphoreTake(xButtonSemaphore, 0);

        // 2. Configuration de la broche PD2 (INT0) en entrée avec Pull-Up interne
        DDRD &= ~(1 << DDD2);   // PD2 en entrée
        PORTD |= (1 << PORTD2); // Activation résistance de Pull-Up

        // 3. Configuration de l'interruption externe INT0 sur front descendant
        EICRA |= (1 << ISC01);
        EICRA &= ~(1 << ISC00);

        // 4. Activation de l'interruption externe INT0
        EIMSK |= (1 << INT0);
    }
}

/* ------------------------------------------------------------------ */
/* ISR (Interrupt Service Routine) pour INT0                          */
/* ------------------------------------------------------------------ */
ISR(INT0_vect)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    // Débloquer le sémaphore depuis l'ISR
    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);

    // Sur AVR, si une tâche de plus haute priorité est débloquée, on force le Yield
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        taskYIELD();
    }
}