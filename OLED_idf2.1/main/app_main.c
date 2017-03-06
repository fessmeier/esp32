
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include "u8g2.h"
#include "esp_log.h"


/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

u8g2_t u8g2;

// These defines come from the sample I2C code provided by in the ESP_IDF
#define DATA_LENGTH          512  /*!<Data buffer length for test buffer*/
#define RW_TEST_LENGTH       129  /*!<Data length for r/w test, any value from 0-DATA_LENGTH*/
#define DELAY_TIME_BETWEEN_ITEMS_MS   1234 /*!< delay time between different test items */

// The other I2C port is I2C_NUM_0 pins 25 SDA 26 SCL
#define I2C_MASTER_SCL_IO    19    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    18    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ    400000     /*!< I2C master clock frequency */

#define OLED_ADDR 0x3C              /*!< OLED address, you can set any 7bit value */
#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */
#define ACK_CHECK  ACK_CHECK_EN

static i2c_cmd_handle_t i2c_cmd;

void i2c_master_init()
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

uint8_t u8x8_gpio_and_delay_esp32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	return 0;
}

uint8_t u8x8_byte_esp32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	switch(msg){
		case U8X8_MSG_BYTE_INIT: {
			i2c_master_init();
			break;
		}
		case U8X8_MSG_BYTE_START_TRANSFER: {
			i2c_cmd = i2c_cmd_link_create();
			i2c_master_start(i2c_cmd);
			i2c_master_write_byte(i2c_cmd, ( OLED_ADDR << 1 ) | WRITE_BIT, ACK_CHECK);
			break;
		}
		case U8X8_MSG_BYTE_SEND: {
            for (int i = 0; i < arg_int; i++){
               i2c_master_write_byte(i2c_cmd,  ((uint8_t*)arg_ptr)[i], ACK_CHECK);
            }
            // Write does not work as I expected.  Had to loop through the data manually.
            // Need to figure out why.  
			//i2c_master_write(i2c_cmd, (uint8_t*)arg_ptr, arg_int, ACK_CHECK);
			break;
		}
		case U8X8_MSG_BYTE_END_TRANSFER: {
            i2c_master_stop(i2c_cmd);
			i2c_master_cmd_begin(I2C_MASTER_NUM, i2c_cmd, 1000 / portTICK_RATE_MS);
		    i2c_cmd_link_delete(i2c_cmd);
		}
	}
	return 1;
}

uint8_t u8x8_byte_esp32_2nd_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	return 1;
}

void draw_task(void *pvParameter)
{
	char buf1[32] = "Hello World!";
    char buf2[32] = "This is a test.";
    char buf3[32] = "It works!";
    char buf4[32] = "font size 5x7";

    u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, U8G2_R0, u8x8_byte_esp32_hw_i2c, u8x8_gpio_and_delay_esp32);
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display

	while(1){

		  u8g2_ClearBuffer(&u8g2);
		  u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
		  u8g2_DrawStr(&u8g2, 2, 7, buf1);
          u8g2_DrawStr(&u8g2, 2, 15, buf2);
          u8g2_DrawStr(&u8g2, 2, 23, buf3);
          u8g2_DrawStr(&u8g2, 2, 31, buf4);
		  u8g2_DrawFrame(&u8g2, 0, 0, 128, 32);
		  u8g2_SendBuffer(&u8g2);

		  vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    nvs_flash_init();

    xTaskCreate(&blink_task, "blink_task", 512, NULL, 5, NULL);
    xTaskCreate(&draw_task, "draw_task", 2048, NULL, 10, NULL);
}
