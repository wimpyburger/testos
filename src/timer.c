// timer.c -- KEYBOARD STUFF
//            Based loosely for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "console.h"
#include <stdbool.h>

bool caps = false;

u16int keycodetoascii(u16int scancode) {
	u16int ascii = 0;
	if(scancode == 2) {
		ascii = 49; // 1
	}
	if(scancode == 3) {
		ascii = 50; // 2
	}
	if(scancode == 4) {
		ascii = 51; // 3
	}
	if(scancode == 5) {
		ascii = 52; // 4
	}
	if(scancode == 6) {
		ascii = 53; // 5
	}
	if(scancode == 7) {
		ascii = 54; // 6
	}
	if(scancode == 8) {
		ascii = 55; // 7
	}
	if(scancode == 9) {
		ascii = 56; // 8
	}
	if(scancode == 10) {
		ascii = 57; // 9
	}
	if(scancode == 11) {
		ascii = 48; // 0
	}
	if(scancode == 16) {
		ascii = 113; // q
	}
	if(scancode == 17) {
		ascii = 119; // w
	}
	if(scancode == 18) {
		ascii = 101; // e
	}
	if(scancode == 19) {
		ascii = 114; // r
	}
	if(scancode == 20) {
		ascii = 116; // t
	}
	if(scancode == 21) {
		ascii = 121; // y
	}
	if(scancode == 22) {
		ascii = 117; // u
	}
	if(scancode == 23) {
		ascii = 105; // i
	}
	if(scancode == 24) {
		ascii = 111; // o
	}
	if(scancode == 25) {
		ascii = 112; // p
	}
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
	if(scancode == 34) {
		ascii = 103; // g
	}
	if(scancode == 35) {
		ascii = 104; // h
	}
	if(scancode == 36) {
		ascii = 106; // j
	}
	if(scancode == 37) {
		ascii = 107; // k
	}
	if(scancode == 38) {
		ascii = 108; // l
	}
	if(scancode == 44) {
		ascii = 122; // z
	}
	if(scancode == 45) {
		ascii = 120; // x
	}
	if(scancode == 46) {
		ascii = 99; // c
	}
	if(scancode == 47) {
		ascii = 118; // v
	}
	if(scancode == 48) {
		ascii = 98; // b
	}
	if(scancode == 49) {
		ascii = 110; // n
	}
	if(scancode == 50) {
		ascii = 109; // m
	}
	
	
	if(caps && ascii >= 97 && ascii <= 122) {
		ascii -= 32; // works for letter only
	}
	return ascii;
}

void startinput() {
	terminal_row++;
	if(terminal_row > 20) {
		clearconsole();
		terminal_row = 0;
	}
	terminal_column = 0;
	inputlength = 0;
	memset(inputbuffer, 0, 80);
	PrintStringAt(">", terminal_column++, terminal_row+=2);
	PrintCharAt(32, terminal_column, terminal_row);
	movecursor(terminal_column, terminal_row);
}

void checkcommand(const char* command) {
	if(stringstartswith(command, "clr")) {
		clearconsole();
		terminal_column = 0;
		terminal_row = 0;
	}
	
	if(stringstartswith(command, "echo")) {
		terminal_column = 0;
		PrintStringAt(command + 5, terminal_column, terminal_row + 1);
	}
}

static void keyboard_callback(registers_t regs)
{
	asm("in $0x60, %al;"
		"mov $0x20, %al;"
		"out %al, $0x20;"
	); // allows multiple keyboard interrupts

	u16int keycode = inb(0x60); // get interrupt data
	
	// print interrupt code in bottom right
	PrintStringAt("   ", 77, 24);
	PrintNumberAt(keycode, 77, 24);
	
	// RETURN KEY
	if(keycode == 28) {
		// do stuff with input
		checkcommand(inputbuffer);
		// prepare for new input
		startinput();
	}
	
	// BACKSPACE
	if(keycode == 14) {
		if(terminal_column > 1) {
			inputlength--;
			memset(inputbuffer + inputlength, 0, 1);
			terminal_column--;
			PrintStringAt(" ", terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
		}
	}
	
	// SHIFT
	if(keycode == 42) { // pressed
		caps = true;
	}
	if(keycode == 170) { // released
		caps = false;
	}
	
	// CAPS LOCK
	if(keycode == 58) {
		caps = !caps;
	}
	
	// SPACE
	if(keycode == 57) {
		if(inputlength < 79) { // char limit
			memset(inputbuffer + inputlength, 32, 1);
			inputlength++;
			terminal_column++;
			PrintStringAt(" ", terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
		}
	}
	
	// LETTER
	if(keycodetoascii(keycode) != 0) {
		if(inputlength < 79) { // 19 char limit
			memset(inputbuffer + inputlength, keycodetoascii(keycode), 1); // add character to end of buffer
			inputlength++;
			PrintCharAt(keycodetoascii(keycode), terminal_column++, terminal_row);
			// hacky code to get cursor to work (problem with screen clearing function)
			PrintCharAt(32, terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
		}
	}
	
    //PrintStringAt("Tick: ", terminal_column, terminal_row);
	if(terminal_row > 25) {
		terminal_row = 1;
	}
	
}

void init_keyboard()
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
