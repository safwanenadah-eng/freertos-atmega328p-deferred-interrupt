#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "button.h"

// Tâche Trousse de secours / Traitement de l'interruption (Priorité Élevée)
void vButtonHandlerTask(void *pvParameters)
{
    while (1)
    {
        // Attente indéfinie du Sémaphore transmis par l'ISR INT0
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE)
        {
            UART_SendString_Safe("[ISR DEFERRED] Hardware Interrupt INT0 Triggered! Processing in Task context...\r\n");
        }
    }
}

// Tâche périodique de fond (Priorité Basse)
void vHeartbeatTask(void *pvParameters)
{
    while (1)
    {
        UART_SendString_Safe("[SYSTEM] System Running Normally...\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Tick toutes les 1s
    }
}

int main(void)
{
    // Initialisation des périphériques
    UART_Init(9600);
    Button_Init();

    // Création des tâches
    // Tâche Handler avec une priorité supérieure (2) à la tâche de fond (1)
    xTaskCreate(vButtonHandlerTask, "BtnHndlr", 128, NULL, 2, NULL);
    xTaskCreate(vHeartbeatTask,     "Heartbt",  128, NULL, 1, NULL);

    // Activation globale des interruptions
    sei();

    // Démarrage de l'ordonnanceur
    vTaskStartScheduler();

    while (1) { ; }
    return 0;
}

void vApplicationIdleHook(void)
{
}