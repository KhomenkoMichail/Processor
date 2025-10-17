#include <TXLib.h>

int mod (int ax, int bx);
int mod (int ax, int bx)
    {
    return bx - (ax / bx * bx);
    }

int main()
    {
    txCreateWindow (900, 900);
    txSetColor (TX_DARKGRAY);
    txSetColor (TX_BLACK);

    int ram[100*100 * 3];
    int r_dx = 20;

    for (int i_ax = 0; i_ax < sizearr (ram); i_ax += 3)
        {
        int x_bx = i_ax/3 % 100 - 40;
        int y_cx = i_ax/3 / 100 - 40;

        if (x_bx * x_bx + y_cx * y_cx < r_dx*r_dx)
            {
            ram[i_ax] = 255;
            }
        }

    for (int i = 0; i < sizearr (ram); i += 3)
        {
        int x = i/3 % 100;
        int y = i/3 / 100;

        txSetFillColor (RGB (ram[i], ram[i+1], ram[i+2]));
        txRectangle (x*10, y*10, x*10+15, y*10+15);
        }
    }
