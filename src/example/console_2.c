#include <stdio.h>
#include <la/console.h>

int main(void) {
    /* console_clear(); */
    console_clear();
    console_puts(CONSOLE_COLOR_BLACK, CONSOLE_COLOR_LIGHTGREEN, "Spuuuucky!");
    console_setCursor(0, 10);

    console_printf(CONSOLE_COLOR_LIGHTGREEN, CONSOLE_COLOR_BLACK, "colored printf (%d, %d)\n", CONSOLE_COLOR_LIGHTGREEN, CONSOLE_COLOR_BLACK);

    return 0;
}
