#ifndef HELPER_H
#define HELPER_H

class Helper{
    public:
    const static int WINDOW_WIDTH=1000;
    const static int WINDOW_HEIGHT=700;
    static void createWindow(int *, char **);
    static void clearScreen();
    static void plotPoint(int, int);
    static double distanceBetweenPts(int, int, int, int);
};

#endif