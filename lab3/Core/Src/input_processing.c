#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "main.h"
#include "timer.h"

enum SystemMode {
  MODE_1_NORMAL = 1,
  MODE_2_MODIFY_RED,
  MODE_3_MODIFY_YELLOW,
  MODE_4_MODIFY_GREEN
};

enum TrafficPhase {
  PHASE_1_G1_R2, // Tuyến 1 Xanh, Tuyến 2 Đỏ
  PHASE_2_Y1_R2, // Tuyến 1 Vàng, Tuyến 2 Đỏ
  PHASE_3_R1_G2, // Tuyến 1 Đỏ, Tuyến 2 Xanh
  PHASE_4_R1_Y2  // Tuyến 1 Đỏ, Tuyến 2 Vàng
};

static uint8_t time_red = 5;
static uint8_t time_yellow = 2;
static uint8_t time_green = 3;

static uint8_t temp_time = 0;

static enum SystemMode current_mode = MODE_1_NORMAL;
static enum TrafficPhase current_phase = PHASE_1_G1_R2;

static uint8_t is_system_initialized = 0;

static int8_t count_road1 = 0;
static int8_t count_road2 = 0;

static uint8_t prev_btn1 = 0;
static uint8_t prev_btn2 = 0;
static uint8_t prev_btn3 = 0;

static void initMode1(void) {
  if (time_red != (time_green + time_yellow)) {
    time_red = time_green + time_yellow;
  }
  current_phase = PHASE_1_G1_R2;
  count_road1 = time_green;
  count_road2 = time_red;
  clearAllTrafficLeds();
  HAL_GPIO_WritePin(GREEN_1_GPIO_Port, GREEN_1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RED_2_GPIO_Port, RED_2_Pin, GPIO_PIN_SET);
  setLedBuffer(count_road1, count_road2);
  reset_timer_1s();
}

static void runTrafficLightMode1(void) {
  if (timer_flag_1s) {
    timer_flag_1s = 0;

    count_road1--;
    count_road2--;

    switch (current_phase) {
    case PHASE_1_G1_R2:
      if (count_road1 <= 0) {
        current_phase = PHASE_2_Y1_R2;
        count_road1 = time_yellow;
        clearAllTrafficLeds();
        HAL_GPIO_WritePin(YELLOW_1_GPIO_Port, YELLOW_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RED_2_GPIO_Port, RED_2_Pin, GPIO_PIN_SET);
      }
      break;

    case PHASE_2_Y1_R2:
      if (count_road1 <= 0) {
        current_phase = PHASE_3_R1_G2;
        count_road1 = time_red;
        count_road2 = time_green;
        clearAllTrafficLeds();
        HAL_GPIO_WritePin(RED_1_GPIO_Port, RED_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GREEN_2_GPIO_Port, GREEN_2_Pin, GPIO_PIN_SET);
      }
      break;

    case PHASE_3_R1_G2:
      if (count_road2 <= 0) {
        current_phase = PHASE_4_R1_Y2;
        count_road2 = time_yellow;
        clearAllTrafficLeds();
        HAL_GPIO_WritePin(RED_1_GPIO_Port, RED_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(YELLOW_2_GPIO_Port, YELLOW_2_Pin, GPIO_PIN_SET);
      }
      break;

    case PHASE_4_R1_Y2:
      if (count_road2 <= 0) {
        current_phase = PHASE_1_G1_R2;
        count_road1 = time_green;
        count_road2 = time_red;
        clearAllTrafficLeds();
        HAL_GPIO_WritePin(GREEN_1_GPIO_Port, GREEN_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RED_2_GPIO_Port, RED_2_Pin, GPIO_PIN_SET);
      }
      break;
    }

    uint8_t disp1 = (count_road1 > 0) ? count_road1 : 0;
    uint8_t disp2 = (count_road2 > 0) ? count_road2 : 0;
    setLedBuffer(disp1, disp2);
  }
}

void fsm_for_input_processing(void) {
  if (!is_system_initialized) {
    is_system_initialized = 1;
    initMode1();
  }

  uint8_t curr_btn1 = is_button_pressed(0);
  uint8_t curr_btn2 = is_button_pressed(1);
  uint8_t curr_btn3 = is_button_pressed(2);

  // 1. Nút 1 : chuyển mode
  if (curr_btn1 == 1 && prev_btn1 == 0) {
    clearAllTrafficLeds();

    if (current_mode == MODE_1_NORMAL) {
      current_mode = MODE_2_MODIFY_RED;
      temp_time = time_red;
      setLedBuffer(2, temp_time);
    } else if (current_mode == MODE_2_MODIFY_RED) {
      current_mode = MODE_3_MODIFY_YELLOW;
      temp_time = time_yellow;
      setLedBuffer(3, temp_time);
    } else if (current_mode == MODE_3_MODIFY_YELLOW) {
      current_mode = MODE_4_MODIFY_GREEN;
      temp_time = time_green;
      setLedBuffer(4, temp_time);
    } else if (current_mode == MODE_4_MODIFY_GREEN) {
      current_mode = MODE_1_NORMAL;
      initMode1();
    }
  }
  prev_btn1 = curr_btn1;

  // 2. Nút 2 : tăng thời gian
  if (current_mode != MODE_1_NORMAL) {
    uint8_t need_update_display = 0;

    if (curr_btn2 == 1 && prev_btn2 == 0) {
      temp_time++;
      if (temp_time > 99)
        temp_time = 1;
      need_update_display = 1;
    } else if (is_button_pressed_1s(1) && timer_flag_500ms) {
      timer_flag_500ms = 0;
      temp_time++;
      if (temp_time > 99)
        temp_time = 1;
      need_update_display = 1;
    }

    if (need_update_display) {
      setLedBuffer(current_mode, temp_time);
    }
  }
  prev_btn2 = curr_btn2;

  // 3. Nút 3 : lưu
  if (current_mode != MODE_1_NORMAL) {
    if (curr_btn3 == 1 && prev_btn3 == 0) {
      if (current_mode == MODE_2_MODIFY_RED) {
        time_red = temp_time;
      } else if (current_mode == MODE_3_MODIFY_YELLOW) {
        time_yellow = temp_time;
      } else if (current_mode == MODE_4_MODIFY_GREEN) {
        time_green = temp_time;
      }
    }
  }
  prev_btn3 = curr_btn3;

  // 4. HÀNH VI TỪNG MODE
  switch (current_mode) {
  case MODE_1_NORMAL:
    runTrafficLightMode1();
    break;

  case MODE_2_MODIFY_RED:
    if (timer_flag_2hz) {
      timer_flag_2hz = 0;
      toggleRedLeds();
    }
    break;

  case MODE_3_MODIFY_YELLOW:
    if (timer_flag_2hz) {
      timer_flag_2hz = 0;
      toggleYellowLeds();
    }
    break;

  case MODE_4_MODIFY_GREEN:
    if (timer_flag_2hz) {
      timer_flag_2hz = 0;
      toggleGreenLeds();
    }
    break;
  }
}