#include "display.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
static ST7735_Object_t hst7735;

/* ── IO callbacks ─────────────────────────────────────────── */

static int32_t IO_Init(void)    { return 0; }
static int32_t IO_GetTick(void) { return (int32_t)HAL_GetTick(); }

static int32_t IO_WriteReg(uint8_t cmd, uint8_t *data, uint32_t len)
{
    HAL_GPIO_WritePin(DC_PIN_GPIO_Port,  DC_PIN_Pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CS_PIN_GPIO_Port,  CS_PIN_Pin,  GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);

    if (len > 0) {
        HAL_GPIO_WritePin(DC_PIN_GPIO_Port, DC_PIN_Pin, GPIO_PIN_SET);
        HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, GPIO_PIN_SET);
    return 0;
}

static int32_t IO_SendData(uint8_t *data, uint32_t len)
{
    HAL_GPIO_WritePin(DC_PIN_GPIO_Port, DC_PIN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, GPIO_PIN_SET);
    return 0;
}

static int32_t IO_ReadReg(uint8_t addr, uint8_t *data) { return 0; }
static int32_t IO_RecvData(uint8_t *data, uint32_t len) { return 0; }

/* ── Funções públicas ─────────────────────────────────────── */

void Display_Init(void)
{
    // Estado inicial — CS deve começar HIGH
    HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_PIN_GPIO_Port, DC_PIN_Pin, GPIO_PIN_SET);

    // Reset físico
    HAL_GPIO_WritePin(RST_PIN_GPIO_Port, RST_PIN_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(RST_PIN_GPIO_Port, RST_PIN_Pin, GPIO_PIN_SET);
    HAL_Delay(150);

    ST7735_IO_t io = {
        .Init     = IO_Init,
        .DeInit   = NULL,
        .Address  = 0,
        .WriteReg = IO_WriteReg,
        .ReadReg  = IO_ReadReg,
        .SendData = IO_SendData,
        .RecvData = IO_RecvData,
        .GetTick  = IO_GetTick,
    };

    ST7735_RegisterBusIO(&hst7735, &io);
    ST7735_Init(&hst7735, ST7735_FORMAT_RBG565, ST7735_ORIENTATION_PORTRAIT_ROT180);}


void Display_FillRect(int x, int y, int w, int h, uint32_t color)
{
    ST7735_FillRect(&hst7735, x, y, w, h, color);
}

// display.c
void Display_FillBlocking(void)
{
    for (int y = 0; y < 160; y++)
        ST7735_DrawHLine(&hst7735, 0, y, 128, COLOR_BLACK);
}

void Display_FillScreen(uint32_t color)
{
    ST7735_FillRect(&hst7735, 0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

void Display_DrawSprite(int x, int y, int w, int h, uint8_t *data)
{
    if (x >= LCD_WIDTH  || y >= LCD_HEIGHT) return;
    if (x + w <= 0      || y + h <= 0)     return;

    // Calcula a região visível
    int src_x = 0, src_y = 0;
    int dst_x = x, dst_y = y;
    int draw_w = w, draw_h = h;

    if (dst_x < 0) { src_x = -dst_x; draw_w -= src_x; dst_x = 0; }
    if (dst_y < 0) { src_y = -dst_y; draw_h -= src_y; dst_y = 0; }
    if (dst_x + draw_w > LCD_WIDTH)  draw_w = LCD_WIDTH  - dst_x;
    if (dst_y + draw_h > LCD_HEIGHT) draw_h = LCD_HEIGHT - dst_y;

    // Monta buffer só com os pixels visíveis
    uint8_t buf[draw_w * draw_h * 2];

    for (int row = 0; row < draw_h; row++)
    {
        for (int col = 0; col < draw_w; col++)
        {
            int src_idx = ((src_y + row) * w + (src_x + col)) * 2;
            int dst_idx = (row * draw_w + col) * 2;
            buf[dst_idx]     = data[src_idx];
            buf[dst_idx + 1] = data[src_idx + 1];
        }
    }

    ST7735_FillRGBRect(&hst7735, dst_x, dst_y, buf, draw_w, draw_h);
}

void Display_DrawRawBuffer(int x, int y, int w, int h, uint8_t *buf)
{
    ST7735_FillRGBRect(&hst7735, x, y, buf, w, h);
}
