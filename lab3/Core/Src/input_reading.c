#include "input_reading.h"
#include "main.h"
#include "timer.h"

#define NO_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING (1000 / TIMER_CYCLE)

#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static GPIO_TypeDef *BUTTON_PORT[NO_OF_BUTTONS] = {
    BUTTON_1_GPIO_Port, BUTTON_2_GPIO_Port, BUTTON_3_GPIO_Port};
static uint16_t BUTTON_PIN[NO_OF_BUTTONS] = {BUTTON_1_Pin, BUTTON_2_Pin,
                                             BUTTON_3_Pin};

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {
    BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {
    BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {
    BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};

void button_reading(void) {
  for (uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
    debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
    debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_PORT[i], BUTTON_PIN[i]);

    if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
      buttonBuffer[i] = debounceButtonBuffer1[i];

      if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
        if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
          counterForButtonPress1s[i]++;
        } else {
          flagForButtonPress1s[i] = 1;
        }
      } else {
        counterForButtonPress1s[i] = 0;
        flagForButtonPress1s[i] = 0;
      }
    }
  }
}

unsigned char is_button_pressed(unsigned char index) {
  if (index >= NO_OF_BUTTONS)
    return 0;
  return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
  if (index >= NO_OF_BUTTONS)
    return 0;
  return (flagForButtonPress1s[index] == 1);
}