/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "display.h"
#include "game_types.h"
#include "player.h"
#include "input.h"
#include "renderer.h"
#include "time.h"
#include "enemy_windmill.h"
#include "game_state.h"
#include "enemy_bullets.h"
#include "backgrounds.h"
#include "background.h"
#include "game_difficulty.h"
#include "wave_manager.h"
#include "level1.h"
#include "sprites.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define START_X    40
#define START_Y    40
#define OPT_X      50
#define OPT_EASY_Y 20
#define OPT_MED_Y  42
#define OPT_LUN_Y  66
#define CURSOR_X   40
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

SPI_HandleTypeDef hspi1;

/* Definitions for TaskDisplay */
osThreadId_t TaskDisplayHandle;
const osThreadAttr_t TaskDisplay_attributes = {
  .name = "TaskDisplay",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskInput */
osThreadId_t TaskInputHandle;
const osThreadAttr_t TaskInput_attributes = {
  .name = "TaskInput",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskGame */
osThreadId_t TaskGameHandle;
const osThreadAttr_t TaskGame_attributes = {
  .name = "TaskGame",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for qInput */
osMessageQueueId_t qInputHandle;
const osMessageQueueAttr_t qInput_attributes = {
  .name = "qInput"
};
/* Definitions for mutexSPI */
osMutexId_t mutexSPIHandle;
const osMutexAttr_t mutexSPI_attributes = {
  .name = "mutexSPI"
};
/* Definitions for mutexGame */
osMutexId_t mutexGameHandle;
const osMutexAttr_t mutexGame_attributes = {
  .name = "mutexGame"
};
/* USER CODE BEGIN PV */
GameState g_state = STATE_MENU;
GameState prev_state = (GameState)-1;
volatile uint16_t adc_buf[2] = {0};
Player g_player;
Enemy g_enemies[MAX_ENEMIES];
Bullet g_enemy_bullets[MAX_ENEMY_BULLETS];
GameDifficulty g_difficulty = DIFFICULTY_EASY;
int g_menu_state  = 0;
int g_menu_cursor = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
void StartTaskDisplay(void *argument);
void StartTaskInput(void *argument);
void StartTaskGame(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  Display_Init();
  Display_FillBlocking();

  __HAL_ADC_ENABLE(&hadc1);
  HAL_Delay(2);
  __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_EOC | ADC_FLAG_OVR);

  hdma_adc1.Instance->CR &= ~DMA_SxCR_EN;
  while (hdma_adc1.Instance->CR & DMA_SxCR_EN);
  HAL_DMA_Start(&hdma_adc1, (uint32_t)&ADC1->DR, (uint32_t)adc_buf, 2);

  ADC1->CR2 |= ADC_CR2_DMA;
  ADC1->CR2 |= ADC_CR2_SWSTART;
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of mutexSPI */
  mutexSPIHandle = osMutexNew(&mutexSPI_attributes);

  /* creation of mutexGame */
  mutexGameHandle = osMutexNew(&mutexGame_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of qInput */
  qInputHandle = osMessageQueueNew (5, sizeof(uint8_t), &qInput_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  qInputHandle = osMessageQueueNew(1, sizeof(InputState_t), &qInput_attributes); // sobreescrevendo o correto
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskDisplay */
  TaskDisplayHandle = osThreadNew(StartTaskDisplay, NULL, &TaskDisplay_attributes);

  /* creation of TaskInput */
  TaskInputHandle = osThreadNew(StartTaskInput, NULL, &TaskInput_attributes);

  /* creation of TaskGame */
  TaskGameHandle = osThreadNew(StartTaskGame, NULL, &TaskGame_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CS_PIN_Pin|DC_PIN_Pin|RST_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SW_PIN_Pin */
  GPIO_InitStruct.Pin = SW_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_PIN_Pin DC_PIN_Pin RST_PIN_Pin */
  GPIO_InitStruct.Pin = CS_PIN_Pin|DC_PIN_Pin|RST_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTaskDisplay */
/**
  * @brief  Function implementing the TaskDisplay thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskDisplay */
void StartTaskDisplay(void *argument)
{
  /* USER CODE BEGIN 5 */

  // ── Histórico do dirty-rect — static = RAM estática, não ocupa stack ──
  static float prev_px = -1, prev_py = -1;
  static float prev_bx[MAX_PLAYER_BULLETS];
  static float prev_by[MAX_PLAYER_BULLETS];
  static int   prev_ba[MAX_PLAYER_BULLETS];

  static float prev_ex[MAX_ENEMIES];
  static float prev_ey[MAX_ENEMIES];
  static int   prev_ew[MAX_ENEMIES];
  static int   prev_eh[MAX_ENEMIES];
  static int   prev_ea[MAX_ENEMIES];
  static int   prev_ef[MAX_ENEMIES];

  static float prev_ebx[MAX_ENEMY_BULLETS];
  static float prev_eby[MAX_ENEMY_BULLETS];
  static int   prev_eba[MAX_ENEMY_BULLETS];
  static int   prev_ebw[MAX_ENEMY_BULLETS];
  static int   prev_ebh[MAX_ENEMY_BULLETS];

  static GameState prev_state       = (GameState)-1;
  static int       prev_menu_state  = -1;
  static int       prev_menu_cursor = -1;

  // ── Snapshots — static para não ocupar stack ──────────────────────────
  static Player snap_player;
  static Enemy  snap_enemies[MAX_ENEMIES];
  static Bullet snap_enemy_bullets[MAX_ENEMY_BULLETS];
  static GameState snap_state;
  static int snap_menu_state;
  static int snap_menu_cursor;

  // Inicializa histórico
  for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
      prev_bx[i] = 0; prev_by[i] = 0; prev_ba[i] = 0;
  }
  for (int i = 0; i < MAX_ENEMIES; i++) {
      prev_ex[i] = 0; prev_ey[i] = 0;
      prev_ew[i] = 0; prev_eh[i] = 0;
      prev_ea[i] = 0; prev_ef[i] = 0;
  }
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
      prev_ebx[i] = 0; prev_eby[i] = 0;
      prev_eba[i] = 0; prev_ebw[i] = 0; prev_ebh[i] = 0;
  }

  for(;;)
  {
      // ── Snapshot rápido com mutex — solta antes de renderizar ─────────
      osMutexAcquire(mutexGameHandle, osWaitForever);
      memcpy(&snap_player,       &g_player,       sizeof(Player));
      memcpy(snap_enemies,        g_enemies,       sizeof(Enemy)   * MAX_ENEMIES);
      memcpy(snap_enemy_bullets,  g_enemy_bullets, sizeof(Bullet)  * MAX_ENEMY_BULLETS);
      snap_state       = g_state;
      snap_menu_state  = g_menu_state;
      snap_menu_cursor = g_menu_cursor;
      osMutexRelease(mutexGameHandle);

      // ── Renderização sem mutex de game — só segura o SPI ─────────────
      osMutexAcquire(mutexSPIHandle, osWaitForever);

      // ── Bloco 1: transição de estado ──────────────────────────────────
      if (snap_state != prev_state)
      {
          if (snap_state == STATE_MENU)
          {
              background_set(menu_bg);
              background_draw_region(0, 0, SCREEN_W, SCREEN_H);
              prev_menu_state  = -1;
              prev_menu_cursor = -1;
          }
          else if (snap_state == STATE_PLAYING)
          {
              background_set(bg1);
              background_draw_region(0, 0, SCREEN_W, SCREEN_H);
          }
          else if (snap_state == STATE_GAME_OVER)
          {
              background_set(game_over_bg);
              background_draw_region(0, 0, SCREEN_W, SCREEN_H);
          }
          prev_state = snap_state;
      }

      // ── Bloco 2: menu ─────────────────────────────────────────────────
      if (snap_state == STATE_MENU)
      {
          int menu_changed = (snap_menu_state  != prev_menu_state ||
                              snap_menu_cursor != prev_menu_cursor);

          if (menu_changed)
          {
              if (snap_menu_state == 0)
              {
                  if (prev_menu_state == 1)
                  {
                      background_draw_region(CURSOR_X, OPT_EASY_Y, select_cursor.width, select_cursor.height);
                      background_draw_region(OPT_X, OPT_EASY_Y,    text_easy.width,     text_easy.height);
                      background_draw_region(OPT_X, OPT_MED_Y,     text_medium.width,   text_medium.height);
                      background_draw_region(OPT_X, OPT_LUN_Y,     text_lunatic.width,  text_lunatic.height);
                  }
                  sprite_draw(&text_start, START_X, START_Y, 0);
              }
              else
              {
                  if (prev_menu_state == 0)
                      background_draw_region(START_X, START_Y, text_start.width, text_start.height);

                  if (prev_menu_state != 1)
                  {
                      sprite_draw(&text_easy,    OPT_X, OPT_EASY_Y, 0);
                      sprite_draw(&text_medium,  OPT_X, OPT_MED_Y,  0);
                      sprite_draw(&text_lunatic, OPT_X, OPT_LUN_Y,  0);
                  }

                  int cursor_ys[] = { OPT_EASY_Y, OPT_MED_Y, OPT_LUN_Y };

                  if (prev_menu_cursor >= 0 && prev_menu_cursor <= 2)
                      background_draw_region(CURSOR_X, cursor_ys[prev_menu_cursor],
                                             select_cursor.width, select_cursor.height);

                  sprite_draw(&select_cursor, CURSOR_X, cursor_ys[snap_menu_cursor], 0);
              }

              prev_menu_state  = snap_menu_state;
              prev_menu_cursor = snap_menu_cursor;
          }

          osMutexRelease(mutexSPIHandle);
          osDelay(8);
          continue;
      }

      if (snap_state == STATE_GAME_OVER)
      {
          osMutexRelease(mutexSPIHandle);
          osDelay(8);
          continue;
      }

      // ── Bloco 3: dirty-rect do jogo ───────────────────────────────────

      // Player
      float cur_px = snap_player.x;
      float cur_py = snap_player.y;

      if (prev_px >= 0) {
          if ((int)prev_px != (int)cur_px || (int)prev_py != (int)cur_py)
              background_draw_region(prev_px - 6, prev_py - 10, 12, 21);
      }

      // Bullets do player
      for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
          float cur_bx = snap_player.bullets[i].def.hitbox.x;
          float cur_by = snap_player.bullets[i].def.hitbox.y;
          int   cur_ba = snap_player.bullets[i].active;

          if (prev_ba[i]) {
              if (!cur_ba || (int)prev_bx[i] != (int)cur_bx || (int)prev_by[i] != (int)cur_by)
                  background_draw_region(
                      prev_bx[i] - player_bullet.width  / 2,
                      prev_by[i] - player_bullet.height / 2,
                      player_bullet.width,
                      player_bullet.height);
          }

          prev_bx[i] = cur_bx;
          prev_by[i] = cur_by;
          prev_ba[i] = cur_ba;
      }

      // Inimigos
      for (int i = 0; i < MAX_ENEMIES; i++) {
          if (prev_ea[i] && (!snap_enemies[i].active ||
              (int)prev_ex[i] != (int)snap_enemies[i].x ||
              (int)prev_ey[i] != (int)snap_enemies[i].y ||
              prev_ef[i] != snap_enemies[i].anim_frame))
          {
              background_draw_region(prev_ex[i] - prev_ew[i]/2,
                                     prev_ey[i] - prev_eh[i]/2,
                                     prev_ew[i], prev_eh[i]);
          }

          if (snap_enemies[i].x + snap_enemies[i].w/2 > 0 &&
              snap_enemies[i].x - snap_enemies[i].w/2 < SCREEN_W)
          {
              prev_ex[i] = snap_enemies[i].x;
              prev_ey[i] = snap_enemies[i].y;
              prev_ew[i] = snap_enemies[i].w;
              prev_eh[i] = snap_enemies[i].h;
              prev_ef[i] = snap_enemies[i].anim_frame;
          }
          prev_ea[i] = snap_enemies[i].active;
      }

      // Bullets inimigos — apaga sempre (balas se movem todo frame)
      for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
          if (!prev_eba[i] && !snap_enemy_bullets[i].active) continue;

          if (prev_eba[i])
              background_draw_region(
                  prev_ebx[i] - prev_ebw[i] / 2,
                  prev_eby[i] - prev_ebh[i] / 2,
                  prev_ebw[i], prev_ebh[i]);

          const Sprite *spr = snap_enemy_bullets[i].def.sprite
                            ? snap_enemy_bullets[i].def.sprite
                            : &bullet_green_small;

          prev_ebx[i] = snap_enemy_bullets[i].def.hitbox.x;
          prev_eby[i] = snap_enemy_bullets[i].def.hitbox.y;
          prev_eba[i] = snap_enemy_bullets[i].active;
          prev_ebw[i] = spr->width;
          prev_ebh[i] = spr->height;
      }

      prev_px = cur_px;
      prev_py = cur_py;

      // ── Draw ──────────────────────────────────────────────────────────
      player_draw(&snap_player);
      enemies_draw(snap_enemies, MAX_ENEMIES);
      enemy_bullets_draw(snap_enemy_bullets, MAX_ENEMY_BULLETS);

      osMutexRelease(mutexSPIHandle);
      osDelay(8);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskInput */
/**
* @brief Function implementing the TaskInput thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskInput */
void StartTaskInput(void *argument)
{
  /* USER CODE BEGIN StartTaskInput */
  InputState_t raw;

  for(;;)
  {
    raw.joy_x = adc_buf[0];
    raw.joy_y = adc_buf[1];
    raw.sw    = HAL_GPIO_ReadPin(SW_PIN_GPIO_Port, SW_PIN_Pin);

    osMessageQueueReset(qInputHandle);
    osMessageQueuePut(qInputHandle, &raw, 0, 0);

    osDelay(16); // ~60Hz
  }
  /* USER CODE END StartTaskInput */
}

/* USER CODE BEGIN Header_StartTaskGame */
/**
* @brief Function implementing the TaskGame thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskGame */
void StartTaskGame(void *argument)
{
  /* USER CODE BEGIN StartTaskGame */
  Input_State input = {0};
  uint32_t last = time_get_ms();

  game_reset();

  int input_cooldown = 0;


  for(;;)
  {
    uint32_t now   = time_get_ms();
    float    delta = (now - last) / 1000.0f;
    last = now;

    // Limita delta para evitar saltos grandes se a task atrasar
    if (delta > 0.05f) delta = 0.05f;

    input_update(&input);


    if (input_cooldown > 0)
    {
        input_cooldown--;
        // zera os inputs para não vazar para o próximo estado
        input.click = 0;
        input.left = input.right = input.up = input.down = 0;
    }

    switch (g_state)
    {
    	case STATE_MENU:
    		if (g_menu_state == 0)
        	{
        		if (input.click)
            	{
            		g_menu_state  = 1;
                	g_menu_cursor = 0;
                	input_cooldown = 10;
            	}
        	}
        	else
        	{
        		if (input.up && g_menu_cursor > 0)
            	{
            		g_menu_cursor--;
                	input_cooldown = 8;
            	}
            	else if (input.down && g_menu_cursor < 2)
            	{
                	g_menu_cursor++;
                	input_cooldown = 8;
            	}
            	else if (input.click)
            	{
                	g_difficulty  = (GameDifficulty)g_menu_cursor;
                	g_menu_state  = 0;   // reseta para próxima vez
                	g_menu_cursor = 0;
                	game_reset();
                	g_state       = STATE_PLAYING;
                	input_cooldown = 10;
            }
        	}
        	break;

    	case STATE_PLAYING:
    	    osMutexAcquire(mutexGameHandle, osWaitForever);
    	    player_update(&g_player, &input, delta);
    	    enemies_update(g_enemies, MAX_ENEMIES, &g_player, delta);
    	    enemy_bullets_update(g_enemy_bullets, MAX_ENEMY_BULLETS, delta);

    	    if (wave_manager_update(g_enemies, MAX_ENEMIES, delta))
    	        g_state = STATE_WIN;  // todos os eventos disparados

    	    if (enemy_bullets_check_player(g_enemy_bullets, MAX_ENEMY_BULLETS, &g_player))
    	        g_state = STATE_GAME_OVER;

    	    osMutexRelease(mutexGameHandle);
    	    break;

        case STATE_GAME_OVER:
        	if (input.click)
        	{
        		g_state = STATE_MENU;
        		input_cooldown = 10;
        	}
        	break;

        case STATE_WIN:
    		g_state = STATE_MENU;
            break;
    }

    osDelay(16);
  }
  /* USER CODE END StartTaskGame */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
