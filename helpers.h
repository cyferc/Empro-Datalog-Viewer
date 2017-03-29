
#pragma once

#define ABS(x)         ((x < 0) ? (-x) : x)
#define IS_NEGATIVE(x) ((x < 0) ? true : false)

// a = target variable, b = bit number to act upon 0-n
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

// x = target variable, y = mask
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))

#define U16_HI8(data)                        ((quint8)(((data) >> 8) & 0xff))                                                                   // Extract the high 8 bits of a 16-bit value (Most Significant Byte)
#define U16_LO8(data)                        ((quint8)((data) & 0xff))                                                                          // Extract the low 8 bits of a 16-bit value (Least Significant Byte)

#define U32_HI8(data)                        ((quint8)(((data) >> 24) & 0xff))                                                                  // Extract the high 8 bits (bits 31..24) of a 32-bit value.
#define U32_MH8(data)                        ((quint8)(((data) >> 16) & 0xff))                                                                  // Extract the medium high 8 bits (bits 23..16) of a 32-bit value.
#define U32_ML8(data)                        ((quint8)(((data) >> 8) & 0xff))                                                                   // Extract the medium low 8 bits (bits 15..8) of a 32-bit value.
#define U32_LO8(data)                        ((quint8)((data) & 0xff))                                                                          // Extract the low 8 bits (bits 7..0) of a 32-bit value.

#define U16_CONCAT_U8(hi8, lo8)              ((((quint16)(hi8)) << 8) + ((quint16)(lo8)))                                                       // Concatenate 2 x 8 bits to form 16-bit value.
#define U32_CONCAT_U8(hi8, mh8, ml8, lo8)    ((((quint32)(hi8)) << 24) + (((quint32)(mh8)) << 16) + (((quint32)(ml8)) << 8) + ((quint32)(lo8))) // Concatenate 4 x 8 bits to form 32-bit value.

class Helpers
{
public:
    static int randInt(int low, int high);
    static int round_to_nearest(double input, int to_nearest);
    static int round_up_to(int round_to_nearest, int num);
    static int round_down_to(int round_to_nearest, int num);
    static double LinearInterpolation(double x0, double y0, double x1, double y1, double x);
    static double BilinearInterpolation(double q11, double q12, double q21, double q22, double x1, double x2, double y1, double y2, double x, double y) ;
    static double Map(double x, double in_min, double in_max, double out_min, double out_max);
};
