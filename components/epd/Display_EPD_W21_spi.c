#include "Display_EPD_W21_spi.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "epd_spi";

static spi_device_handle_t epd_spi;
static SemaphoreHandle_t busy_sem = NULL;

/* BUSY falling-edge ISR — releases the semaphore so the waiting task wakes up */
static void IRAM_ATTR busy_isr_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(busy_sem, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

/* Hardware SPI + GPIO init */
void EPD_GPIO_Init(void)
{
    /* Output pins: CS, DC, RST (MOSI/SCK are managed by the SPI peripheral) */
    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << EPD_W21_CS_PIN) |
                        (1ULL << EPD_W21_DC_PIN) |
                        (1ULL << EPD_W21_RST_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    /* Input pin: BUSY, falling-edge interrupt */
    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << EPD_W21_BUSY_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE, /* falling edge */
    };
    gpio_config(&in_conf);

    /* Install GPIO ISR service if not yet installed */
    gpio_install_isr_service(0);
    gpio_isr_handler_add(EPD_W21_BUSY_PIN, busy_isr_handler, NULL);

    /* Create binary semaphore for BUSY wait */
    if (busy_sem == NULL)
    {
        busy_sem = xSemaphoreCreateBinary();
    }

    /* Init SPI bus */
    spi_bus_config_t buscfg = {
        .miso_io_num = -1, /* EPD has no MISO */
        .mosi_io_num = EPD_W21_MOSI_PIN,
        .sclk_io_num = EPD_W21_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(EPD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = EPD_SPI_CLOCK_HZ,
        .mode = 0,          /* CPOL=0, CPHA=0 */
        .spics_io_num = -1, /* CS controlled in software */
        .queue_size = EPD_SPI_QUEUE_SIZE,
        .flags = SPI_DEVICE_HALFDUPLEX,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(EPD_SPI_HOST, &devcfg, &epd_spi));

    /* Idle levels */
    EPD_W21_CS_1;
    EPD_W21_RST_1;
    EPD_W21_DC_0;

    ESP_LOGI(TAG, "Hardware SPI + BUSY interrupt initialized");
}

/* Wait for BUSY to go low using interrupt + semaphore */
void Epaper_WaitBusy(void)
{
    /* Fast path: already idle */
    if (isEPD_W21_BUSY == 0)
        return;

    /* Wait for falling-edge ISR to release the semaphore */
    xSemaphoreTake(busy_sem, portMAX_DELAY);
}

/* Hardware SPI write byte */
void SPI_Write(unsigned char value)
{
    spi_transaction_t t = {0};
    t.length = 8; /* 8 bits */
    t.tx_buffer = &value;
    ESP_ERROR_CHECK(spi_device_polling_transmit(epd_spi, &t));
}

/* SPI write command */
void EPD_W21_WriteCMD(unsigned char command)
{
    EPD_W21_CS_0;
    EPD_W21_DC_0; /* D/C#  0:command */
    SPI_Write(command);
    EPD_W21_CS_1;
}

/* SPI write data */
void EPD_W21_WriteDATA(unsigned char datas)
{
    EPD_W21_CS_0;
    EPD_W21_DC_1; /* D/C#  1:data */
    SPI_Write(datas);
    EPD_W21_CS_1;
}
