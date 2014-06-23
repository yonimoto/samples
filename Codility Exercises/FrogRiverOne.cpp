#include <vector>
#include <map>

using namespace std;

int solution(int X, vector<int> &A) 
{
    // write your code in C++11
    map<int, int> pos_time;
    
    for (int i = 0; i < A.size(); i++)
    {
        if(!pos_time[A[i]])
            pos_time[A[i]] = i; //save only the earliest time each position appears
    }
    
    int maxTime = 0;
    
    for (int i = 0; i <= X; i++)
    {
        if (pos_time[i] > maxTime)
            maxTime = pos_time[i];
    }
    
    return maxTime;
}
