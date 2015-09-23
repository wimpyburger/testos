// timer.c -- KEYBOARD STUFF
//            Based loosely for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "console.h"

u16int keycodetoascii(u16int scancode) {
	u16int ascii = 0;
	if(scancode == 30) {
		ascii = 97; // a
	}
	if(scancode == 31) {
		ascii = 115; // s
	}
	if(scancode == 32) {
		ascii = 100; // d
	}
	if(scancode == 33) {
		ascii = 102; // f
	}
	return ascii;
}

static void timer_callback(registers_t regs)
{
	asm("in $0x60, %al;"
		"mov $0x20, %al;"
		"out %al, $0x20;"
	); // allows multiple keyboard interrupts

	u16int keycode = inb(0x60);
	if(keycodetoascii(keycode) != 0) {
		PrintCharAt(keycodetoascii(keycode), terminal_column++, terminal_row);
		
		// hacky code to get cursor to work
		terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);
		PrintCharAt(32, terminal_column, terminal_row);
		movecursor(terminal_column, terminal_row);
		
		// print scan code
		terminal_color = make_color(COLOR_WHITE, COLOR_DARK_GREY);
		PrintStringAt("   ", 15, 15);
		PrintNumberAt(keycode, 15, 15);
	}
	
    //PrintStringAt("Tick: ", terminal_column, terminal_row);
	if(terminal_row > 25) {
		terminal_row = 0;
	}
	
}

void init_timer(u32int frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ1, &timer_callback);
}
