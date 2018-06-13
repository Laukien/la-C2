#include <stdio.h>
#include <la/console.h>

int main(void) {
    /* console_clear(); */
    console_clear();
    console_setCursor(20, 2);
    printf("WIDTH: %d, HEIGHT: %d\n", console_getWidth(), console_getHeight());

    return 0;
}
