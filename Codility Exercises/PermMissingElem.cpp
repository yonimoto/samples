#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> &A)
{
    int check = 1;
    
    for (vector<int>::iterator it = A.begin(); it != A.end(); it++)
    {
        if (check != *it)
            break;
        else check++;
    }
    return check;
}
