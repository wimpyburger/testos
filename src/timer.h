// timer.h -- Defines the interface for all PIT-related functions.
//            Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#include "console.h"

void startinput();
void init_keyboard();
u16int keycodetoascii(u16int scancode);

#endif
