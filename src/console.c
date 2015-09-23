#include <stddef.h>
#include <stdint.h>
#include "console.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
uint16_t* terminal_buffer;
size_t terminal_column; // x
size_t terminal_row; // y
uint8_t terminal_color;
char *inputbuffer;
size_t inputlength = 0;

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

void clearconsole() {
	memset(terminal_buffer, 0, 4000); // write empty characters to entirety of video buffer
	// causes cursor to not work over coordinates written with '0's
}

void initconsole() {
	// set default console vars
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	
	clearconsole();
}

void PrintCharAt(char c, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, terminal_color);
}

void PrintStringAt(const char* str, size_t x, size_t y) {
	size_t datalen = strlen(str);
	for (size_t i = 0; i < datalen; i++)
		PrintCharAt(str[i], x + i, y);
}

void PrintNumberAt(size_t num, size_t x, size_t y) {
	// hacky way to print a number to string
	// works with hex and positive integers
	// get number of digits as script reads number right to left
	size_t temp = num;
	int length = 0;
	while(temp > 0) {
		temp /= 10;
		length++;
	}
	
	int digitnum = 1;
	while (num > 0) { // for each number
		size_t digit = num % 10;
		
		if(digit == 0) {
			PrintStringAt("0", x + length - digitnum, y);
		}
		if(digit == 1) {
			PrintStringAt("1", x + length - digitnum, y);
		}
		if(digit == 2) {
			PrintStringAt("2", x + length - digitnum, y);
		}
		if(digit == 3) {
			PrintStringAt("3", x + length - digitnum, y);
		}
		if(digit == 4) {
			PrintStringAt("4", x + length - digitnum, y);
		}
		if(digit == 5) {
			PrintStringAt("5", x + length - digitnum, y);
		}
		if(digit == 6) {
			PrintStringAt("6", x + length - digitnum, y);
		}
		if(digit == 7) {
			PrintStringAt("7", x + length - digitnum, y);
		}
		if(digit == 8) {
			PrintStringAt("8", x + length - digitnum, y);
		}
		if(digit == 9) {
			PrintStringAt("9", x + length - digitnum, y);
		}
		
		num /= 10;
		digitnum++;
	}
}

void outb(u16int port, u8int value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port)
{
	u16int ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void movecursor(size_t x, size_t y) {
	u16int cursorLocation = y * 80 + x;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

void* memset(void* pointer, int value, size_t size) {
	unsigned char* dest = (unsigned char*) pointer;
	for(size_t i = 0; i < size; i++)
		dest[i] = (unsigned char) value;
	return pointer;
}