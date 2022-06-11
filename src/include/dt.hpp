#pragma once
#include <stdint.h>

// GDT stuff
struct gdt_entry_struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

// IDT stuff
struct idt_entry_struct
{
   uint16_t base_lo;
   uint16_t sel;
   uint8_t  always0;
   uint8_t  flags;
   uint16_t base_hi;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Init descriptor tables
void init_descriptor_tables();