#pragma once
#include <stdint.h>
#include <vga.hpp>
#include <stddef.h>

namespace terminal {
    void initialize(void);
    void setcolor(enum vga_color fore, enum vga_color back);
    void putentryat(char c, uint8_t color, size_t x, size_t y);
    void move_cursor();
    void scroll();
    void putchar(char c);
    void write(const char *data, size_t size);
    void print(const char *data);
    void clear();
    void print_dec(uint32_t n);
}