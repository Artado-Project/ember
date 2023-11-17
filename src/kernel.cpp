#include <terminal.hpp>
#include <dt.hpp>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <timer.hpp>
#include <keyboard.hpp>
#include <serial.hpp>

using namespace keyboard;
using namespace serial;
using namespace terminal;
using namespace timer;
using namespace vga;

extern "C" void kernel_main(void)
{
  init();
  log("knl", "Initializing");
  log("dts", "Initializing");
  init_descriptor_tables();
  asm volatile ("sti");
  log("tty", "Initializing");
  initialize();

  // Ember logo
  setcolor(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
  print("   __ _ _ __ _   _ ___ \n"
                  "  / _` | '__| | | / __|\n"
                  " | (_| | |  | |_| \\__ \\ \n"
                  "  \\__,_|_|   \\__,_|___/ \n");

  setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  print("Welcome to Arus!\n");
  print("Version 1, Build 0.0.1\n");
  log("pit", "Initializing");
  init(100);
  log("kbd", "Initializing");
  init();
  log("knl", "Initialized");
  print("> ");
  char theinput[25];
  input(24, theinput);
  print("You said this: ");
  print(theinput);
  print("\nTest interrupt:\n");
  asm volatile ("int $0x13");
  for (;;){
    asm volatile("hlt");
  }
}