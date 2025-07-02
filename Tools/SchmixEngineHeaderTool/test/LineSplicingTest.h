/* test_line_splicing.h - Enhanced for comment splicing tests */
#ifndef TEST_LINE_SPLICING_H
#define TEST_LINE_SPLICING_H

// ===== 1. Normal line continuations =====
#define HELLO_WORLD "Hello \
World"

#define MULTI_LINE_MACRO \
    printf("This is \
spliced"); \
    do_something()

// ===== 2. Comment splicing tests =====
// Single-line comment with \
continuation (should become one line)

/* Multi-line comment \
with explicit splicing */

/* This comment ends with backslash\ */

// ===== 3. Special cases =====
// Should NOT splice (escaped newline in string):
#define ESCAPED_STRING "This newline \\\n remains"

// Empty continuation (should be removed):
#define EMPTY_CONTINUATION \

// Continuation with whitespace:
#  define SPACED_CONTINUATION  123 \
                              + 456

// ===== 4. Real-world examples =====
// Common accidental case (URL):
// https://example.com/very/long/\
path/to/resource

// Assembly inline with continuation:
__asm__ volatile ("mov \
    %eax, %ebx");

// ===== 5. EOF edge cases =====
#define LAST_MACRO \
    1 + \
    2 + \
    3
// (File ends here with no newline)