#pragma once
#include <stdint.h>

void timer_init(uint32_t frequency);
uint32_t timer_tick();