#ifndef LINUX_MAC_H
#define LINUX_MAC_H

#include <unistd.h>       // For STDIN_FILENO
#include <termios.h>      // For terminal control
#include <sys/select.h>   // For select function

// Function declarations
void setNonCanonicalMode(bool enable);
bool _kbhit();
char _getch();

#endif // LINUX_MAC_H
