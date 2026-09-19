#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* ESP32 pin assignment */
#define EPD_W21_MOSI_PIN GPIO_NUM_14
#define EPD_W21_CLK_PIN GPIO_NUM_13
#define EPD_W21_CS_PIN GPIO_NUM_5
#define EPD_W21_DC_PIN GPIO_NUM_17
#define EPD_W21_RST_PIN GPIO_NUM_16
#define EPD_W21_BUSY_PIN GPIO_NUM_4

/* SPI device config — used by hardware SPI */
#define EPD_SPI_HOST SPI2_HOST
#define EPD_SPI_CLOCK_HZ (4 * 1000 * 1000) /* 4 MHz, safe for most e-paper */
#define EPD_SPI_QUEUE_SIZE 8

/* Pin-level macros (CS, DC, RST still controlled in software) */
#define EPD_W21_CS_0 gpio_set_level(EPD_W21_CS_PIN, 0)
#define EPD_W21_CS_1 gpio_set_level(EPD_W21_CS_PIN, 1)
#define EPD_W21_DC_0 gpio_set_level(EPD_W21_DC_PIN, 0)
#define EPD_W21_DC_1 gpio_set_level(EPD_W21_DC_PIN, 1)
#define EPD_W21_RST_0 gpio_set_level(EPD_W21_RST_PIN, 0)
#define EPD_W21_RST_1 gpio_set_level(EPD_W21_RST_PIN, 1)
#define isEPD_W21_BUSY gpio_get_level(EPD_W21_BUSY_PIN)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

/* Wait for BUSY falling edge via interrupt (non-blocking) */
void Epaper_WaitBusy(void);

#endif
