#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_CYCLE 10

extern uint8_t timer_flag_2hz;
extern uint8_t timer_flag_1s;
extern uint8_t timer_flag_500ms;

void reset_timer_1s(void);

#endif /* INC_TIMER_H_ */