#include "io.hpp"
#include "string.hpp"
#include "vga.hpp"
#include <stddef.h>
#include <stdint.h>

namespace terminal {
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
int cursor_y = 0;
int cursor_x = 0;
uint8_t backColour = 0;
uint8_t foreColour = 15;

size_t row;
size_t column;
uint8_t color;
uint16_t *buffer;

void initialize(void) {
  row = 0;
  column = 0;
  color = vga::vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  buffer = (uint16_t *)0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      buffer[index] = vga::vga_entry(' ', color);
    }
  }
}

void setcolor(enum vga_color fore, enum vga_color back) {
  color = fore | back << 4;
  backColour = back;
  foreColour = fore;
}

void putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  buffer[index] = vga::vga_entry(c, color);
}

// Updates the hardware cursor.
static void move_cursor() {
  // The screen is 80 characters wide...
  uint16_t cursorLocation = cursor_y * 80 + cursor_x;
  outb(0x3D4, 14); // Tell the VGA board we are setting the high cursor byte.
  outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  outb(0x3D4, 15); // Tell the VGA board we are setting the low cursor byte.
  outb(0x3D5, cursorLocation); // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll() {

  // Get a space character with the default colour attributes.
  uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  // Row 25 is the end, this means we need to scroll up
  if (cursor_y >= 25) {
    // Move the current text chunk that makes up the screen
    // back in the buffer by a line
    int i;
    for (i = 0 * 80; i < 24 * 80; i++) {
      buffer[i] = buffer[i + 80];
    }

    // The last line should now be blank. Do this by writing
    // 80 spaces to it.
    for (i = 24 * 80; i < 25 * 80; i++) {
      buffer[i] = blank;
    }
    // The cursor should now be on the last line.
    cursor_y = 24;
  }
}

// Writes a single character out to the screen.
void putchar(char c) {
  // The background colour is black (0), the foreground is white (15).

  // The attribute byte is made up of two nibbles - the lower being the
  // foreground colour, and the upper the background colour.
  uint8_t attributeByte = (backColour << 4) | (foreColour & 0x0F);
  // The attribute byte is the top 8 bits of the word we have to send to the
  // VGA board.
  uint16_t attribute = attributeByte << 8;
  uint16_t *location;

  // Handle a backspace, by moving the cursor back one space
  if (c == 0x08 && cursor_x) {
    cursor_x--;
  }

  // Handle a tab by increasing the cursor's X, but only to a point
  // where it is divisible by 8.
  else if (c == 0x09) {
    cursor_x = (cursor_x + 8) & ~(8 - 1);
  }

  // Handle carriage return
  else if (c == '\r') {
    cursor_x = 0;
  }

  // Handle newline by moving cursor back to left and increasing the row
  else if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  }
  // Handle any other printable character.
  else if (c >= ' ') {
    location = buffer + (cursor_y * 80 + cursor_x);
    *location = c | attribute;
    cursor_x++;
  }

  // Check if we need to insert a new line because we have reached the end
  // of the screen.
  if (cursor_x >= 80) {
    cursor_x = 0;
    cursor_y++;
  }

  // Scroll the screen if needed.
  scroll();
  // Move the hardware cursor.
  move_cursor();
}

void write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    putchar(data[i]);
  scroll();
  move_cursor();
}

void print(const char *data) { write(data, strlen(data)); }

// Clears the screen, by copying lots of spaces to the framebuffer.
void clear() {
  // Make an attribute byte for the default colours
  uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  int i;
  for (i = 0; i < 80 * 25; i++) {
    buffer[i] = blank;
  }

  // Move the hardware cursor back to the start.
  cursor_x = 0;
  cursor_y = 0;
  move_cursor();
}

} // namespace terminal