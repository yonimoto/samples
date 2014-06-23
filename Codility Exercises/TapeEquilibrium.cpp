#include <vector>
#include <numeric>

using namespace std;

int solution(vector<int> &A)
{
    int size = (int) A.size();
    long long leftSide = 0, rightSide = 0;
    unsigned long long total, diff, best = ~0;
    
    total = accumulate(A.begin(), A.end(), 0);
    
    for (int P = 0; P < size-1; P++)
    {
        leftSide += A[P];
        rightSide = total - leftSide;
        diff = abs(leftSide - rightSide);
        if (diff < best)
            best = diff;
    }
    return (int) best;
}
