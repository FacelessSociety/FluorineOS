#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <arch/interrupts/IDT.h>

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
}

// The following will be our kernel's entry point.
void _start(void) {
    idt_install();
    done();
}
