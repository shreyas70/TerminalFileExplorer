

#include <sys/ioctl.h>
#include <stdio.h>

int main (void)
{
    struct winsize S_windowsize;
    ioctl(0, TIOCGWINSZ, &S_windowsize);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
    return 0;
}