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
 *  People who worked on this file:
 *  
 *  Ian Marco Moffett (5purious).
 *
 */

#include <libkern/log.h>
#include <libkern/string.h>
#include <limine.h>

static volatile struct limine_terminal_request term_req = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};


static struct limine_terminal* terminal = NULL;


static void puts(const char* str) {
    if (terminal == NULL) {
        terminal = term_req.response->terminals[0];
    }

    term_req.response->write(terminal, str, strlen(str));
}


void printf(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char* ptr;

    if (memcmp(fmt, KINFO, strlen(KINFO))) {
        puts(KINFO);
        fmt += strlen(KINFO);
    }

    if (memcmp(fmt, KERR, strlen(KERR))) {
        puts(KERR);
        fmt += strlen(KERR);
    }

    for (ptr = fmt; *ptr != '\0'; ++ptr) {
        if (*ptr == '%') {
            ++ptr;
            switch (*ptr) {
                case 's':
                    puts(va_arg(ap, char*));
                    break;
            }
        } else {
            char terminated[2] = {*ptr, 0};
            puts(terminated);
        }
    }
}
