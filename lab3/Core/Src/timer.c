#include "timer.h"
#include "input_reading.h"
#include "led_display.h"
#include "main.h"

uint8_t timer_flag_2hz = 0;
static uint16_t timer_counter_2hz = 0;

uint8_t timer_flag_1s = 0;
static uint16_t timer_counter_1s = 0;

uint8_t timer_flag_500ms = 0;
static uint16_t timer_counter_500ms = 0;

void reset_timer_1s(void) {
  timer_counter_1s = 0;
  timer_flag_1s = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {

    button_reading();

    static uint8_t counter_7seg = 0;
    counter_7seg++;
    if (counter_7seg >= 5) {
      counter_7seg = 0;
      update7SEG_scan();
    }

    timer_counter_2hz++;
    if (timer_counter_2hz >= (250 / TIMER_CYCLE)) {
      timer_flag_2hz = 1;
      timer_counter_2hz = 0;
    }

    timer_counter_1s++;
    if (timer_counter_1s >= (1000 / TIMER_CYCLE)) {
      timer_flag_1s = 1;
      timer_counter_1s = 0;
    }

    timer_counter_500ms++;
    if (timer_counter_500ms >= (500 / TIMER_CYCLE)) {
      timer_flag_500ms = 1;
      timer_counter_500ms = 0;
    }
  }
}