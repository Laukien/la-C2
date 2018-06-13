#include <stdio.h>
#include <la/console.h>

int main(void) {
    char key;

    console_puts(CONSOLE_COLOR_LIGHTGREEN, CONSOLE_COLOR_BLUE, "Hit a key!");

    key = console_getKey();

    console_printf(CONSOLE_COLOR_RED, CONSOLE_COLOR_YELLOW, "Key: 0x%X\n", key);

    return 0;
}
