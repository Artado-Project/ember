#include <keyboard.hpp>
#include <isr.hpp>
#include <io.hpp>
#include <terminal.hpp>
#include <serial.hpp>
#include <stdbool.h>

using namespace terminal;
using namespace serial;

namespace keyboard
{
bool ginput = false;
char lastchar = 0;
bool gotchar = false;
unsigned char scancode;

unsigned char keyboard_us[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, // Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, // Left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',	'm', ',', '.', '/',   0, // Right shift
    '*', 0, // Alt
    ' ', 0, // Caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 - F10
    0,	// 69 - Num lock
    0,	// Scroll Lock
    0,	// Home key
    0,	// Up Arrow
    0,	// Page Up
    '-',
    0,	// Left Arrow
    0,
    0,	// Right Arrow
    '+',
    0,	// 79 - End key
    0,	// Down ArroW
    0,	// Page Down
    0,	// Insert Key
    0,	// Delete Key
    0,   0,   0,
    0, 0,	// F11 - F12
    0,	// All other keys are undefined
};

static void keyboard_handler(registers_t *regs)
{
    (void)regs; // `regs` is not used
    scancode = inb(0x60);
    if (ginput) {
        if (scancode & 0x80)
        {
            // A key was released, do nothing for now.
        }
        else
        {
            lastchar = keyboard_us[scancode];
            gotchar = true;
        }
    }
}

void init(void)
{
   register_interrupt_handler(33, &keyboard_handler);
   log("kbd", "Initialized");
}

char getchar()
{
    lastchar = 0; // set last character to null character
    ginput = true; // enable input
    while (gotchar == false) io_wait(); // wait until key press
    ginput = false; // disable input
    gotchar = false; // set gotchar to false
    return lastchar; // return the character
}

void input(unsigned int input_length, char *theinput)
{
    int last_position = input_length - 1;
    int position = 0;
    char character = 0;
    while (character != '\n') { // until the user presses enter
        character = getchar();
        if (character == '\b') {
            if (position != 0) {
                theinput[position] = 0;
                position--;
                putchar(character);
            }
        } else if (position != last_position) {
            if (character != '\n') theinput[position] = character;
            position++;
            putchar(character);
        }
    }
}
} // namespace keyboard