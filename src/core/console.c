/*
 * =====================================================================================
 *
 *       Filename:  la_console.c
 *    Description:  basic console commands
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2018
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#endif
#include <la/console.h>

void console_clear() {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    /* get buffer-info */
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    /* get size of buffer */
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the enrire screen */
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten)) return;

    /* set buffers attributes */
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) return;

    /* set cursor */
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
	printf(CONSOLE_CLEAR);
    fflush(stdout);
#endif
}

int console_getWidth() {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT wSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    wSize = csbi.srWindow;

    return wSize.Right - wSize.Left + 1;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_col;
#endif
}

int console_getHeight() {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT wSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    wSize = csbi.srWindow;

    return wSize.Bottom - wSize.Top + 1;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_row;
#endif
}

void console_setCursor(int x, int y) {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p;
    p.X = x;
    p.Y = y;

    SetConsoleCursorPosition(hConsole, p);
    /* WriteConsole(hConsole, "Hello", 5, NULL, NULL); */
#else
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
#endif
}

#ifdef NOT_IMPLEMENTED
const char *getTty() {
    const char *dev;
    dev = ttyname(STDIN_FILENO);
    if (!dev) dev = ttyname(STDOUT_FILENO);
    if (!dev) dev = ttyname(STDERR_FILENO);
    if (!dev) return NULL;

    return dev;
}

int console_getPositionX() {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD wSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    wSize = csbi.dwCursorPosition;

    return wSize.X;
#else
    const char *tty;
    int fd; /* file handle */
    struct termios  saved, temporary;

    /* get TTY */
    tty = getTty();
    if (!tty) return -1;

    /* open TTY */
    fd = open(tty, O_RDWR | O_NOCTTY);
    if (fd == -1) return -1;
/*
 * see
 * https://www.linuxquestions.org/questions/programming-9/get-cursor-position-in-c-947833/
 */
#endif
}

int console_getPositionY() {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD wSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    wSize = csbi.dwCursorPosition;

    return wSize.Y;
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_xpixel;
#endif
}
#endif

/*int console_getKey(int byte, int second) {*/
int console_getKey() {
#ifdef __WIN32
    DWORD mode;
    DWORD cc;
    TCHAR c;
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    if (!hConsole) return -1;

    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    c = 0;
    ReadConsole(hConsole, &c, 1, &cc, NULL);
    SetConsoleMode(hConsole, mode);

    return c;
#else
    struct termios orig_termios; /* terminal structure */
    volatile int ttyfd = STDIN_FILENO; /* STDIN (0) */
    struct termios raw;
	int n; /* count of character */
	int c; /* read character */

	/* save old state */
    if (tcgetattr(ttyfd, &orig_termios) < 0) return -1;

	/* switch to raw */
    raw = orig_termios; /* copy original and then modify below */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    /*raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);*/
    raw.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    raw.c_cc[VMIN] = 1; /* blocking */
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(ttyfd, TCSAFLUSH, &raw) < 0) return -1;

	/* read key form stdin */
	n = read(ttyfd, &c, 1);
	if (n < 0) c = -1;
	else if (n == 0) c = 0;

	/* reset state */
    if (tcsetattr(ttyfd, TCSAFLUSH, &orig_termios) < 0) return -1;

	return c;
#endif
}

void console_puts(int fcolor, int bcolor, const char *str) {
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD attributes;
    WORD wColor;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    attributes = csbi.wAttributes;

    /* wColor = ((bcolor & 0x0F) << 4) + (fcolor & 0x0F); */
    wColor = 0;
    if (bcolor != CONSOLE_COLOR_DEFAULT) wColor = (bcolor & 0x0F) << 4;
    if (fcolor != CONSOLE_COLOR_DEFAULT) wColor += (fcolor & 0x0F); 
    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, wColor);

    puts(str);
    fflush(stdout);

    SetConsoleTextAttribute(hConsole, attributes);
#else
    printf("\033[0;");
    if (fcolor == CONSOLE_COLOR_DEFAULT) printf("39");
    else if (fcolor >= 0 && fcolor <= 7) printf("%d", fcolor + 30);
    else if (fcolor >= 8 && fcolor <= 15) printf("%d", fcolor + 90 - 8);
    if (bcolor == CONSOLE_COLOR_DEFAULT) printf("49");
    else if (bcolor >= 0 && bcolor <= 7) printf(";%d", bcolor + 40);
    else if (bcolor >= 8 && bcolor <= 15) printf(";%d", bcolor + 100 - 8);
    printf("m");
    printf("%s%s\n", str, CONSOLE_DEFAULT);
    fflush(stdout);
#endif
}

void console_printf(int fcolor, int bcolor, const char *fmt, ...) {
	va_list args;
#ifdef __WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD attributes;
    WORD wColor;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    attributes = csbi.wAttributes;

    /* wColor = ((bcolor & 0x0F) << 4) + (fcolor & 0x0F); */
    wColor = 0;
    if (bcolor != CONSOLE_COLOR_DEFAULT) wColor = (bcolor & 0x0F) << 4;
    if (fcolor != CONSOLE_COLOR_DEFAULT) wColor += (fcolor & 0x0F); 
    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, wColor);

	va_start(args, fmt);
    vprintf(fmt, args);
    fflush(stdout);
	va_end(args);

    SetConsoleTextAttribute(hConsole, attributes);
#else
    printf("\033[0;");
    if (fcolor == CONSOLE_COLOR_DEFAULT) printf("39");
    else if (fcolor >= 0 && fcolor <= 7) printf("%d", fcolor + 30);
    else if (fcolor >= 8 && fcolor <= 15) printf("%d", fcolor + 90 - 8);
    if (bcolor == CONSOLE_COLOR_DEFAULT) printf("49");
    else if (bcolor >= 0 && bcolor <= 7) printf(";%d", bcolor + 40);
    else if (bcolor >= 8 && bcolor <= 15) printf(";%d", bcolor + 100 - 8);
    printf("m");
	va_start(args, fmt);
    vprintf(fmt, args);
    printf("%s", CONSOLE_DEFAULT);
    fflush(stdout);
	va_end(args);

#endif
}
