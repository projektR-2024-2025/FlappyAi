#include "linuxMac.h"
#include <iostream>
#include <unistd.h>       // For STDIN_FILENO
#include <termios.h>      // For terminal control
#include <sys/select.h>   // For select function

// Set or restore non-canonical mode
void setNonCanonicalMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt); // Save terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings
    }
}

// Function to check if a key is pressed
bool _kbhit() {
    timeval timeout = {0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout) > 0;
}

// Function to get a single character input without waiting for Enter
char _getch() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}