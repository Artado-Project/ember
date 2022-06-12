#include <terminal.hpp>
#include <dt.hpp>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <timer.hpp>

extern "C" void kernel_main(void)
{
  init_descriptor_tables();
  terminal::initialize();

  // Ember logo
  terminal::setcolor(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
  terminal::print("   __ _ _ __ _   _ ___ \n"
                  "  / _` | '__| | | / __|\n"
                  " | (_| | |  | |_| \\__ \\ \n"
                  "  \\__,_|_|   \\__,_|___/ \n");

  terminal::setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  terminal::print("Welcome to Arus!\n");
  terminal::print("Version 1, Build 0.0.1\n");
  
  asm volatile("sti");

  asm volatile ("int $0x3");
  asm volatile ("int $0x4");
  init_timer(50);
}