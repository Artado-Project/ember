#include <terminal.hpp>
#include <isr.hpp>
#include <io.hpp>

isr_t interrupt_handlers[256];

extern "C" void isr_handler(registers_t regs)
{
  terminal::print("recieved interrupt: ");
  terminal::print_hex(regs.int_no);
  if (regs.err_code != 0) {
    terminal::print(", errno: ");
    terminal::print_hex(regs.err_code);
  }
  terminal::print("\n");
  asm volatile ("cli");
}

extern "C" void irq_handler(registers_t regs)
{
  if (regs.int_no >= 40)
  {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
  if (interrupt_handlers[regs.int_no] != 0)
  {
    isr_t handler = interrupt_handlers[regs.int_no];
    handler(&regs);
  }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
} 