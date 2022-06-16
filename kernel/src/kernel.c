#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <arch/interrupts/IDT.h>
#include <libkern/log.h>

static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
}

// The following will be our kernel's entry point.
void _start(void) {
    printf("%s\n%x\n%d\n%c", "string string", 0xdeadbeef, 1234, 'c');
    idt_install();
    done();
}
