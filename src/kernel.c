/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
#include "console.h"
#include "descriptor_tables.h"
#include "timer.h"

void render();

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main() {
	initconsole();
	
	init_descriptor_tables();
	
	movecursor(0,0);

	//asm volatile("int $0x3");
    //asm volatile("int $0x4");

    asm volatile("sti"); // gets irq working
    init_timer(50); // change this eventually
	
	int lastkey;
	
	while(1==1) {
		render();
		
		// keyboard input
		/*u16int keycode = inb(0x60);
		if(keycode != lastkey && keycode < 50) {
			PrintCharAt(keycodetoascii(keycode), terminal_column++, terminal_row);
			terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);
			PrintCharAt(32, terminal_column, terminal_row);
			movecursor(terminal_column, terminal_row);
			terminal_color = make_color(COLOR_WHITE, COLOR_DARK_GREY);
			lastkey = keycode;
		}
		PrintStringAt("   ", 0, 2);
		PrintNumberAt(keycode, 0, 2);*/
	}
}

void render() {
	terminal_color = make_color(COLOR_WHITE, COLOR_DARK_GREY);
	for (size_t i = 5; i < 80; i++)
		PrintStringAt(" ", i, 0);
	
	PrintStringAt("START", 0, 0);
}

void kernel_test() {
	PrintStringAt("KEk", 0, 0);
	kernel_test();
}