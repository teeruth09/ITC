#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED 5
#define YELLOW 6
#define GREEN 7
#define SW1 8
#define SW2 9
#define SW3 10

QueueHandle_t BlinkQueue;
int count = 1 , pastY;
int pastButton = 0;
int GREEN_status;
void setup()
{
  Serial.begin(9600);
  BlinkQueue = xQueueCreate(5, sizeof(int32_t));
  xTaskCreate(vSenderTask1, "SenderTask 1", 100, SW1, 1, NULL);
  xTaskCreate(vSenderTask2, "SenderTask 2", 100, SW2, 1, NULL);
  xTaskCreate(vSenderTask3, "SenderTask 3", 100, SW3, 1, NULL);
  xTaskCreate(vReceiverTask1, "Receiver Task R", 100, RED, 1, NULL);
  xTaskCreate(vReceiverTask2, "Receiver Task Y", 100, YELLOW, 1, NULL);
  xTaskCreate(vReceiverTask3, "Receiver Task G", 100, GREEN, 1, NULL);
}
//send RED
void vSenderTask1(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  pinMode(SW1, INPUT);
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while (1)
  {
    if (digitalRead(SW1) == HIGH)
      valueToSend = SW1;
    else
      valueToSend = 0;

    if (valueToSend != 0) {
      xQueueSendToBack(BlinkQueue,&valueToSend, xTicksToWait);
      vTaskDelay(36);
    }
  }
}

//send YELLOW
void vSenderTask2(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  pinMode(SW2, INPUT);
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while (1)
  {
    if (digitalRead(SW2) == HIGH)
      valueToSend = SW2;
    else
      valueToSend = 0;

    if (valueToSend != 0) {
      xQueueSendToBack(BlinkQueue,&valueToSend, xTicksToWait);
      vTaskDelay(36);
    }
  }
}

//send GREEN
void vSenderTask3(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  pinMode(SW3, INPUT);
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while (1)
  {
    if (digitalRead(SW3) == HIGH)
      valueToSend = SW3;
    else
      valueToSend = 0;

    if (valueToSend != 0) {
      xQueueSendToFront(BlinkQueue,&valueToSend, xTicksToWait);
      vTaskDelay(pdMS_TO_TICKS(3000));
    }
  }
}
//RED
void vReceiverTask1(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT);
  long RED_TIME = 0;
  while (1)
  {
    qStatus = xQueueReceive(BlinkQueue,&valueReceived, xTicksToWait);

    if (qStatus == pdPASS)
    {

      if (valueReceived == SW1)
      {
       
        digitalWrite(RED, HIGH);
        vTaskDelay(pdMS_TO_TICKS(3000));
        
        digitalWrite(RED, LOW);
      }
      else
        xQueueSendToFront(BlinkQueue,&valueReceived, xTicksToWait);
    }
  }
}

//YELLOW
void vReceiverTask2(void *pvParameters)
{
  int32_t valueReceived;
  int statusY = 0;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(YELLOW, OUTPUT);
  long YELLOW_TIME = 0;

  while (1)
  {
    qStatus = xQueueReceive(BlinkQueue,&valueReceived, xTicksToWait);

    if (qStatus == pdPASS)
    {
      if (valueReceived == SW2)
        statusY = !statusY;
      else
        xQueueSendToFront(BlinkQueue,&valueReceived, xTicksToWait);
    }
    if (statusY == 1)
    {
      digitalWrite(YELLOW, HIGH);
      vTaskDelay(pdMS_TO_TICKS(500));
      digitalWrite(YELLOW, LOW);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}

//Receive GREEN
void vReceiverTask3(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(GREEN, OUTPUT);
  long GREEN_TIME = 0;
  while (1) //true
  {
    qStatus = xQueueReceive(BlinkQueue,&valueReceived, xTicksToWait);
    if (qStatus == pdPASS)
    {
      if (valueReceived == SW3)
      {
        digitalWrite(GREEN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(GREEN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(GREEN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWrite(GREEN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(GREEN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(GREEN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
      }
      else
        xQueueSendToFront(BlinkQueue,&valueReceived, xTicksToWait);
    }
  }
}
void loop() 
{
}
