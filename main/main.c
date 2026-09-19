#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"
#include "GUI_Paint.h"
#include "fonts.h"

static const char *TAG = "main";
static unsigned char BlackImage[EPD_ARRAY];

/* 每个 demo 都是独立的静态函数，main 只负责按顺序调用 */

/* Demo 1: 全屏标准刷新显示图片 */
static void demo_full_update(void)
{
    ESP_LOGI(TAG, "Demo 1: full screen update");
    EPD_HW_Init();
    EPD_WhiteScreen_ALL(gImage_BW1, gImage_RW1);
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

/* Demo 2: 全屏快速刷新 */
static void demo_fast_update(void)
{
    ESP_LOGI(TAG, "Demo 2: fast screen update");
    EPD_HW_Init_Fast();
    EPD_WhiteScreen_ALL_Fast(gImage_BW1, gImage_RW1);
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

/* Demo 3: 局部刷新数字滚动 */
static void demo_partial_update(void)
{
    ESP_LOGI(TAG, "Demo 3: partial update (num scroll)");
    unsigned char i;

    EPD_HW_Init();
    EPD_SetRAMValue_BaseMap(gImage_BWbasemap, gImage_RWbasemap);

    for (i = 0; i < 6; i++)
    {
        EPD_Dis_Part_Num(48 * 0, 144, Num[1],
                         48 * 1, 144, gImage_dot,
                         48 * 2, 144, Num[i],
                         48 * 3, 144, Num[5 - i], 104, 48);
    }

    EPD_Dis_Part_Num(48 * 0, 144, Num[2],
                     48 * 1, 144, gImage_dot,
                     48 * 2, 144, Num[2],
                     48 * 3, 144, Num[8], 104, 48);

    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(5000));

    /* 清屏消除残影 */
    EPD_HW_Init();
    EPD_WhiteScreen_White();
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

/* Demo 4: 180 度旋转显示 */
static void demo_rotate_180(void)
{
    ESP_LOGI(TAG, "Demo 4: 180 degree rotation");
    EPD_HW_Init_180();
    EPD_WhiteScreen_ALL(gImage_BW1, gImage_RW1);
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

/* Demo 5: GUI 绘图（英文文字 + 数字） */
static void demo_gui_en(void)
{
    ESP_LOGI(TAG, "Demo 5: GUI English text + numbers");

    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    /* 抵消 EPD_HW_Init_GUI 中 0x01 命令第三字节 0x01 引起的屏幕水平镜像 */
    Paint_SetMirroring(MIRROR_VERTICAL);

    EPD_HW_Init_GUI();
    Paint_Clear(WHITE);

    Paint_DrawString_EN(0, 0, "Good Display", &Font8, WHITE, BLACK);
    Paint_DrawString_EN(0, 10, "Good Display", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(0, 25, "Good Display", &Font16, WHITE, BLACK);
    EPD_Display(BlackImage);

    Paint_Clear(WHITE);
    Paint_DrawNum(0, 45, 123456789, &Font20, WHITE, BLACK);
    Paint_DrawNum(0, 70, 123456789, &Font24, WHITE, BLACK);
    EPD_Display(BlackImage);

    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));

    EPD_HW_Init();
    EPD_WhiteScreen_White();
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

/* Demo 6: GUI 显示中文（使用 Font12CN / Font24CN）
 * Paint_DrawString_CN 实际签名（见 GUI_Paint.h）：
 *   void Paint_DrawString_CN(UWORD Xstart, UWORD Ystart,
 *                            const char *pString,
 *                            cFONT *font,
 *                            UWORD Color_Background,
 *                            UWORD Color_Foreground);
 * 注意参数顺序为 Background, Foreground，故白底黑字应传 (WHITE, BLACK)。
 * Font12CN 和 Font24CN 在 fonts.h 中声明为 cFONT 类型。
 */
static void demo_gui_cn(void)
{
    ESP_LOGI(TAG, "Demo 6: GUI Chinese text");

    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    /* 抵消 EPD_HW_Init_GUI 中 0x01 命令第三字节 0x01 引起的屏幕水平镜像 */
    Paint_SetMirroring(MIRROR_VERTICAL);

    EPD_HW_Init_GUI();
    Paint_Clear(WHITE);

    /* 用 12 号中文字体在顶部显示一行 */
    Paint_DrawString_CN(0, 0, "你好", &Font12CN, WHITE, BLACK);
    /* 用 24 号中文字体在下方再显示一行 */
    Paint_DrawString_CN(0, 20, "你好", &Font24CN, WHITE, BLACK);
    EPD_Display(BlackImage);

    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));

    /* 清屏 */
    EPD_HW_Init();
    EPD_WhiteScreen_White();
    EPD_DeepSleep();
    vTaskDelay(pdMS_TO_TICKS(3000));
}

void app_main(void)
{
    ESP_LOGI(TAG, "EPD GDEY042Z98 demo start");
    EPD_GPIO_Init();

    while (1)
    {
        // demo_full_update();
        // demo_fast_update();
        // demo_partial_update();
        // demo_rotate_180();
        demo_gui_en();
        demo_gui_cn();

        ESP_LOGI(TAG, "One round done, restart in 5s");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
