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

void startinput() {
	terminal_row++;
	if(terminal_row > 20) {
		clearconsole();
		terminal_row = 1;
	}
	terminal_column = 0;
	inputlength = 0;
	memset(inputbuffer, 0, 20);
	PrintStringAt(">", terminal_column++, terminal_row+=2);
	PrintCharAt(32, terminal_column, terminal_row);
	movecursor(terminal_column, terminal_row);
}

static void timer_callback(registers_t regs)
{
	asm("in $0x60, %al;"
		"mov $0x20, %al;"
		"out %al, $0x20;"
	); // allows multiple keyboard interrupts

	u16int keycode = inb(0x60);
	// print code
	PrintStringAt("   ", 77, 24);
	PrintNumberAt(keycode, 77, 24);
	if(keycode == 28) { // return key
		// do stuff with input
		PrintStringAt(inputbuffer, 0, terminal_row + 1);
		// prepare for new input
		startinput();
	}
	if(keycode == 14) { // backspace
		if(terminal_column > 1) {
			inputlength--;
			memset(inputbuffer + inputlength, 0, 1);
			terminal_column--;
			PrintStringAt(" ", terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
		}
	}
	if(keycodetoascii(keycode) != 0) {
		if(inputlength < 19) {
			memset(inputbuffer + inputlength, keycodetoascii(keycode), 1);
			inputlength++;
			PrintCharAt(keycodetoascii(keycode), terminal_column++, terminal_row);
			// hacky code to get cursor to work
			PrintCharAt(32, terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
		}
	}
	
    //PrintStringAt("Tick: ", terminal_column, terminal_row);
	if(terminal_row > 25) {
		terminal_row = 1;
	}
	
}

void init_timer(u32int frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ1, &timer_callback);
}
