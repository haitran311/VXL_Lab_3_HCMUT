#include "led_display.h"

// 0: Sáng, 1: Tắt
//
static const uint8_t segment_map[10] = {
    0x40, // 0: a,b,c,d,e,f sáng; g tắt
    0x79, // 1: b,c sáng
    0x24, // 2: a,b,d,e,g sáng
    0x30, // 3: a,b,c,d,g sáng
    0x19, // 4: b,c,f,g sáng
    0x12, // 5: a,c,d,f,g sáng
    0x02, // 6: a,c,d,e,f,g sáng
    0x78, // 7: a,b,c sáng
    0x00, // 8: cả 7 đoạn sáng
    0x10  // 9: a,b,c,d,f,g sáng
};

// led_buffer[0]: hàng chục Tuyến 1 (EN0)
// led_buffer[1]: hàng đơn vị Tuyến 1 (EN1)
// led_buffer[2]: hàng chục Tuyến 2 (EN2)
// led_buffer[3]: hàng đơn vị Tuyến 2 (EN3)
static uint8_t led_buffer[4] = {0, 0, 0, 0};
static uint8_t current_led_index = 0;

// Xuất 1 chữ số ra các chân PB0 - PB6
void display7SEG(int num) {
  if (num < 0 || num > 9)
    return;
  uint8_t pattern = segment_map[num];

  HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin,
                    (pattern & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin,
                    (pattern & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin,
                    (pattern & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin,
                    (pattern & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin,
                    (pattern & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin,
                    (pattern & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin,
                    (pattern & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void setLedBuffer(uint8_t val1, uint8_t val2) {
  led_buffer[0] = val1 / 10;
  led_buffer[1] = val1 % 10;
  led_buffer[2] = val2 / 10;
  led_buffer[3] = val2 % 10;
}


void update7SEG_scan(void) {
 
  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

  display7SEG(led_buffer[current_led_index]);

  switch (current_led_index) {
  case 0:
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
    break;
  case 1:
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
    break;
  case 2:
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
    break;
  case 3:
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
    break;
  }

  current_led_index = (current_led_index + 1) % 4;
}

void clearAllTrafficLeds(void) {
  HAL_GPIO_WritePin(GPIOA,
                    RED_1_Pin | YELLOW_1_Pin | GREEN_1_Pin | RED_2_Pin |
                        YELLOW_2_Pin | GREEN_2_Pin,
                    GPIO_PIN_RESET);
}


void toggleRedLeds(void) {
  HAL_GPIO_TogglePin(RED_1_GPIO_Port, RED_1_Pin);
  HAL_GPIO_TogglePin(RED_2_GPIO_Port, RED_2_Pin);
}


void toggleYellowLeds(void) {
  HAL_GPIO_TogglePin(YELLOW_1_GPIO_Port, YELLOW_1_Pin);
  HAL_GPIO_TogglePin(YELLOW_2_GPIO_Port, YELLOW_2_Pin);
}


void toggleGreenLeds(void) {
  HAL_GPIO_TogglePin(GREEN_1_GPIO_Port, GREEN_1_Pin);
  HAL_GPIO_TogglePin(GREEN_2_GPIO_Port, GREEN_2_Pin);
}