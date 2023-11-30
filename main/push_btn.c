#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "cJSON.h"
#include "app.h"


#define BTN 34

//for IDF V5
static SemaphoreHandle_t btn_sem;

static void IRAM_ATTR on_btn_pushed(void *args)
{
  xSemaphoreGiveFromISR(btn_sem, NULL);
}

static void btn_push_task(void *params)
{
  while (true)
  {
    xSemaphoreTake(btn_sem, portMAX_DELAY);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddBoolToObject(payload, "btn_state", gpio_get_level(BTN));
    char *message = cJSON_Print(payload);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    printf("message: %s\n", message);
    send_ws_message(message);
    cJSON_Delete(payload);
    free(message);
  }
}

void init_btn(void)
{
  xTaskCreate(btn_push_task, "btn_push_task", 2048, NULL, 5, NULL);
  btn_sem = xSemaphoreCreateBinary();
  // not required for version 5
  // gpio_pad_select_gpio(BTN);
  gpio_set_direction(BTN, GPIO_MODE_INPUT);
  gpio_set_intr_type(BTN, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(BTN, on_btn_pushed, NULL);
}




 // #include <u8g2.h>
 // #include "u8g2_esp32_hal.h"
 // 
 // u8g2_t u8g2;
 // Then if you are using i2c you can do something like...
 // 
 // // Setup HAL
 // u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
 // u8g2_esp32_hal.sda = GPIO_SDA;
 // u8g2_esp32_hal.scl = GPIO_SCL;
 // u8g2_esp32_hal_init(u8g2_esp32_hal);
 // 
 // // Init Driver
 // u8g2_Setup_ssd1306_i2c_128x64_noname_f(
 // 	&u8g2,
 // 	U8G2_R0,
 // 	u8g2_esp32_i2c_byte_cb,
 // 	u8g2_esp32_gpio_and_delay_cb);
 // u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);
 // 
 // // Blank Screen
 // u8g2_InitDisplay(&u8g2);
 // u8g2_SetPowerSave(&u8g2, 0);
 // u8g2_ClearBuffer(&u8g2);
 // u8g2_SendBuffer(&u8g2);