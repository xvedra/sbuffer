/*
 * Example of sbuffer class (derived from Stream)
 * 
 * Autor: Guillermo Moncho
 * xvedra@hotmail.com
 * 
 * Mailing under cbuffer
 * Idle -> Task2 -> Task3 -> Task4 -> Idle
 */

#include "sbuffer.h"
#include "esp_task_wdt.h"

#define USE_WDT
#define TWDT_TIMEOUT_S          3
#define TASK_RESET_PERIOD_S     2

TaskHandle_t Task2, Task3, Task4;
sbuffer ser1, ser2, ser3, ser4;

void loop2(void *parameter);
void loop3(void *parameter);
void loop4(void *parameter);

#define CHECK_ERROR_CODE(returned, expected) ({                        \
            if(returned != expected){                                  \
                Serial.println("*** TWDT ERROR ***");                  \
                abort();                                               \
            }                                                          \
})

void setup() 
{ 
  ser1.begin();
  ser2.begin();
  ser3.begin();
  ser4.begin();
  Serial.begin(115200);

  Serial.printf("\n\ncbuffer Class example\n");
  Serial.printf("by xvedra\n");

  #ifdef USE_WDT
  Serial.println("Enable WDT...");
  CHECK_ERROR_CODE(esp_task_wdt_init(TWDT_TIMEOUT_S, false), ESP_OK);
  #ifndef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(0));
  #endif
  #ifndef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(1));
  #endif
  #else
  Serial.println("Disable WDT...");
  disableCore0WDT();
  #endif

  xTaskCreatePinnedToCore(
    loop2,
    "Task_2",
    4096,
    NULL,
    1,
    &Task2,
    0);

  xTaskCreatePinnedToCore(
    loop3,
    "Task_3",
    4096,
    NULL,
    1,
    &Task3,
    0);

  xTaskCreatePinnedToCore(
    loop4,
    "Task_4",
    4096,
    NULL,
    1,
    &Task4,
    0);

  Serial.printf("\n\nGo...\n");
  delay(1000);
}

void loop() 
{   
  char buf1[100];
  int ctr=0;
  int i = 0;
  int sender, value;
  static int dly = 100;
  
  while(1)
  {
    if(dly) dly--;
    else
    {
      dly = 100;
      sprintf(buf1, "sender 1: %d#", ctr++); //seed
      ser1.print(buf1);   
      Serial.printf("\nput> %s\n", buf1);
    }

    #if(0)
    while(ser4.available()) Serial.print((char)ser4.read());
    #else
    if(ser4.available())
    {
      
      i = ser4.readBytesUntil('#', buf1, 20);
      if(i > 0)
      {
        buf1[i] = '\0';
        sscanf(buf1,"sender %d: %d", &sender, &value);
        Serial.printf("Idle> \"%s\" rec %d bytes from %d: value:%d\n", buf1, i, sender, value);      
      }       
    }    
    #endif
    delay(10);
  }  
}

void loop2(void *parameter){
  char buf1[100];
  int i = 0;
  int sender, value;

  #ifdef USE_WDT
  //Subscribe this task to TWDT, then check if it is subscribed
  CHECK_ERROR_CODE(esp_task_wdt_add(NULL), ESP_OK);
  CHECK_ERROR_CODE(esp_task_wdt_status(NULL), ESP_OK);
  #endif
  
  for(;;)
  {
    #ifdef USE_WDT
    //reset the watchdog:
    CHECK_ERROR_CODE(esp_task_wdt_reset(), ESP_OK);  //Comment this line to trigger a TWDT timeout
    #endif

    vTaskDelay(10);
    if(ser1.available())
    {
      i = ser1.readBytesUntil('#', buf1, 20);
      if(i > 0)
      {
        buf1[i] = '\0';
        sscanf(buf1,"sender %d: %d", &sender, &value);
        Serial.printf("Task2> \"%s\" rec %d bytes from %d: value:%d\n", buf1, i, sender, value);      
        //vTaskDelay(500);
        sprintf(buf1, "sender 2: %d#", value+1);
        ser2.print(buf1);   
      } 
    }
  }
}

void loop3(void *parameter){
  char buf1[100];
  int i = 0;
  int sender, value;

  #ifdef USE_WDT
  //Subscribe this task to TWDT, then check if it is subscribed
  CHECK_ERROR_CODE(esp_task_wdt_add(NULL), ESP_OK);
  CHECK_ERROR_CODE(esp_task_wdt_status(NULL), ESP_OK);
  #endif
  
  for(;;)
  {
    #ifdef USE_WDT
    //reset the watchdog:
    CHECK_ERROR_CODE(esp_task_wdt_reset(), ESP_OK);  //Comment this line to trigger a TWDT timeout
    #endif

    vTaskDelay(10);
    if(ser2.available())
    {
      i = ser2.readBytesUntil('#', buf1, 20);
      if(i > 0)
      {
        buf1[i] = '\0';
        sscanf(buf1,"sender %d: %d", &sender, &value);
        Serial.printf("Task3> \"%s\" rec %d bytes from %d: value:%d\n", buf1, i, sender, value);      
        //vTaskDelay(500);
        sprintf(buf1, "sender 3: %d#", value+1);
        ser3.print(buf1);   
      } 
    }
  }
}

void loop4(void *parameter){
  char buf1[100];
  int i = 0;
  int sender, value;

  #ifdef USE_WDT
  //Subscribe this task to TWDT, then check if it is subscribed
  CHECK_ERROR_CODE(esp_task_wdt_add(NULL), ESP_OK);
  CHECK_ERROR_CODE(esp_task_wdt_status(NULL), ESP_OK);
  #endif
  
  for(;;)
  {
    #ifdef USE_WDT
    //reset the watchdog:
    CHECK_ERROR_CODE(esp_task_wdt_reset(), ESP_OK);  //Comment this line to trigger a TWDT timeout
    #endif

    vTaskDelay(10);
    if(ser3.available())
    {
      i = ser3.readBytesUntil('#', buf1, 20);
      if(i > 0)
      {
        buf1[i] = '\0';
        sscanf(buf1,"sender %d: %d", &sender, &value);
        Serial.printf("Task4> \"%s\" rec %d bytes from %d: value:%d\n", buf1, i, sender, value);      
        //vTaskDelay(500);
        sprintf(buf1, "sender 4: %d#", value+1);
        ser4.print(buf1);   
      } 
    }
  }
}

