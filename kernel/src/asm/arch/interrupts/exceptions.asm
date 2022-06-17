;;
;;  MIT License
;;
;;  Copyright (c) 2022 FacelessSociety, Ian Marco Moffett
;;
;;  Permission is hereby granted, free of charge, to any person obtaining a copy
;;  of this software and associated documentation files (the "Software"), to deal
;;  in the Software without restriction, including without limitation the rights
;;  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;;  copies of the Software, and to permit persons to whom the Software is
;;  furnished to do so, subject to the following conditions:
;;
;;  The above copyright notice and this permission notice shall be included in all
;;  copies or substantial portions of the Software.
;;
;;  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;;  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;;  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;;  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;;  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;;  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;;  SOFTWARE.

;; Worked on by:
;; Ian Marco Moffett


bits 64

global divide_error
global debug_exception
global overflow
global bound_range_exceeded
global invalid_opcode
global no_mathcoprocessor
global double_fault
global invalid_tss
global segment_not_present
global stack_segment_fault
global general_protection_fault
global page_fault

%macro e_panic 1
    cli
    hlt
%endmacro

divide_error:
    e_panic 0x0

debug_exception:
    e_panic 0x1

overflow:
    e_panic 0x4

bound_range_exceeded:
    e_panic 0x5

invalid_opcode:
    e_panic 0x6

no_mathcoprocessor:
    e_panic 0x7

double_fault:
    e_panic 0x8

invalid_tss:
    e_panic 0xA

segment_not_present:
    e_panic 0xB

stack_segment_fault:
    e_panic 0xC

general_protection_fault:
    e_panic 0xD

page_fault:
    e_panic 0xE
