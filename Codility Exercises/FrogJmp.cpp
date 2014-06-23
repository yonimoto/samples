#include <cmath>

int solution(int X, int Y, int D)
{
    unsigned long long diff = Y-X;
    int jumps = (int) diff/D;
    jumps += (diff-(D*jumps))%D ? 1 : 0;

    return jumps;
}
