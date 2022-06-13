#pragma once

namespace serial {
    int init();
    char read_serial();
    void write_serial(char a);
    void write_str_serial(char *a);
}