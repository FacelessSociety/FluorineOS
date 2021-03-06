/*
 *  MIT License
 *
 *  Copyright (c) 2022 FacelessSociety, Ian Marco Moffett
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */


/*
 *  Contributors of this file:
 *
 *  Ian Marco Moffett (5purious).
 *
 */


#include <arch/interrupts/IDT.h>
#include <arch/interrupts/exceptions.h>

#define TRAP_GATE_FLAGS 0x8F
#define INT_GATE_FLAGS 0x8E
#define IDT_INT_GATE_USER 0xEE


static struct InterruptGateDescriptor idt[256];
static struct IDTR idtr;


static void set_idt_desc(uint8_t vector, void* isr, uint32_t flags) {
    uint64_t addr = (uint64_t)isr;
    struct InterruptGateDescriptor* vec = &idt[vector];
    vec->isr_low16 = addr & 0xFFFF;
    vec->isr_mid16 = (addr >> 16) & 0xFFFF;
    vec->isr_high32 = (addr >> 32);
    vec->p = 1;
    vec->attr = flags;
    vec->cs = 0x28;
    vec->ist = 0;
    vec->dpl = 3;
    vec->reserved = 0;
    vec->zero = 0;
    vec->zero1 = 0;
    vec->zero2 = 0;
}


static void(*exceptions[])(void) = {
    divide_error,
    debug_exception,
    general_protection_fault,
    general_protection_fault,
    overflow,
    bound_range_exceeded,
    invalid_opcode,
    no_mathcoprocessor,
    double_fault,
    general_protection_fault,
    invalid_tss,
    segment_not_present,
    stack_segment_fault,
    general_protection_fault,
    page_fault
};


static void setup_exceptions(void) {
    for (uint8_t i = 0; i < 0xE; ++i) {
        set_idt_desc(i, exceptions[i], TRAP_GATE_FLAGS);
    }
}


void idt_install(void) {
    setup_exceptions();
    idtr.limit = sizeof(struct InterruptGateDescriptor) * 256 - 1;
    idtr.base = (uint64_t)&idt;
    __asm__ __volatile__("lidt %0" :: "m" (idtr));
}
