#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stddef.h>
#include <stdint.h>

enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

void PrintCharAt(char c, size_t x, size_t y);
void PrintStringAt(const char* str, size_t x, size_t y);
void PrintNumberAt(size_t num, size_t x, size_t y);
static const size_t VGA_WIDTH;
static const size_t VGA_HEIGHT;
char *inputbuffer;
size_t inputlength;
uint16_t* terminal_buffer;
size_t terminal_column;
size_t terminal_row;
uint8_t terminal_color;
uint8_t make_color(enum vga_color fg, enum vga_color bg);
uint16_t make_vgaentry(char c, uint8_t color);
size_t strlen(const char* str);
void clearconsole();
void initconsole();
void movecursor(size_t x, size_t y);
void* memset(void* pointer, int value, size_t size);

#endif