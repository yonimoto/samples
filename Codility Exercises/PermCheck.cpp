#include <algorithm>
#include <vector>
 
using namespace std;
 
int solution(vector<int> &A)
{
    int size = (int) A.size();
 
    sort(A.begin(), A.end());
 
    A.erase(unique(A.begin(), A.end()), A.end());
 
    if (size == A.back())
        return 1;
    else return 0;
}
