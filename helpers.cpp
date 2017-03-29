#include "helpers.h"
#include <cmath>
#include <qglobal.h>

// Random number between low and high
int Helpers::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

int Helpers::round_to_nearest(double input, int to_nearest)
{
    return (int)round(input / to_nearest) * to_nearest;
}

/* Round up a number                               */
/* (Negative numbers go DOWN)                      */
int Helpers::round_up_to(int round_to_nearest, int num)
{
    int sign = 1;

    if (num < 0)
    {
        num  = (-1) * num;
        sign = -1;
    }
    num = (num + round_to_nearest - 1) / round_to_nearest;
    num = num * round_to_nearest;

    return sign * num;
}

/* Round down a number                             */
/* (Negative numbers go UP)                        */
int Helpers::round_down_to(int round_to_nearest, int num)
{
    int sign = 1;

    if (num < 0)
    {
        num  = (-1) * num;
        sign = -1;
    }
    num = num / round_to_nearest;
    num = num * round_to_nearest;

    return sign * num;
}

double Helpers::LinearInterpolation(double x0, double y0, double x1, double y1, double x)
{
   double a = (y1 - y0) / (x1 - x0);
   double b = -a*x0 + y0;
   double y = a * x + b;
   return y;
}

double Helpers::BilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y)
{
    double x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

double Helpers::Map(double x, double in_min, double in_max, double out_min, double out_max)
{
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
