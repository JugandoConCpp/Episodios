#ifndef TERMINAL_H__INCLUDE
#define TERMINAL_H__INCLUDE

#define ENDL "\n"

#define COLOR(n) "\x1b[1;" #n "m"

#define COLOR_BLACK           COLOR(30)
#define COLOR_RED             COLOR(31)
#define COLOR_GREEN           COLOR(32)
#define COLOR_YELLOW          COLOR(33)
#define COLOR_BLUE            COLOR(34)
#define COLOR_MAGENTA         COLOR(35)
#define COLOR_CYAN            COLOR(36)
#define COLOR_WHITE           COLOR(37)
#define COLOR_DEFAULT         COLOR(39)

#define COLOR_BRIGHT_BLACK    COLOR(90)
#define COLOR_BRIGHT_RED      COLOR(91)
#define COLOR_BRIGHT_GREEN    COLOR(92)
#define COLOR_BRIGHT_YELLOW   COLOR(93)
#define COLOR_BRIGHT_BLUE     COLOR(94)
#define COLOR_BRIGHT_MAGENTA  COLOR(95)
#define COLOR_BRIGHT_CYAN     COLOR(96)
#define COLOR_BRIGHT_WHITE    COLOR(97)

#endif  // TERMINAL_H__INCLUDE
