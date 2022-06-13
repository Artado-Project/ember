#include <keyboard.hpp>
#include <isr.hpp>
#include <io.hpp>
#include <terminal.hpp>
#include <serial.hpp>

namespace keyboard
{
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
    unsigned char scancode;
    scancode = inb(0x60);
    if (scancode & 0x80)
    {
        // A key was released, do nothing for now.
    }
    else
    {
        char toprint[1];
        toprint[0] = keyboard_us[scancode];
        terminal::print(toprint); // TODO: don't let the terminal handle the
                                  // backspace itself, because it allows to
                                  // delete everything including messages 
                                  // printed by the OS.
    }
}

void init(void)
{
   register_interrupt_handler(33, &keyboard_handler);
   serial::write_str_serial("Initialized keyboard.\n");
}
} // namespace keyboard