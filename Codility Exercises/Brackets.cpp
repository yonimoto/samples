#include <vector>

using namespace std;

int solution(const string &S)
{
    vector<char> stack;
    
    if (S.empty())
        return 1;
    
    //push each character onto the stack.  If the character is an 'opening' bracket,
    //add without question.  If the character is a 'closing' bracket, the top of the stack
    //must be it's complement
    for (int i = 0; i < (int) S.size(); i++)
    {
        switch(S[i])
        {
            case '(':
            case '[':
            case '{':
                stack.push_back(S[i]);
                break;
            case ')':
                if (!stack.empty() && stack.back() == '(')
                    stack.pop_back();
                else
                    return 0;
                break;
            case ']':
                if (!stack.empty() && stack.back() == '[')
                    stack.pop_back();
                else
                    return 0;
                break;
            case '}':
                if (!stack.empty() && stack.back() == '{')
                    stack.pop_back();
                else
                    return 0;
                break;
        }
    }
    
    if (stack.empty())
        return 1;
    else return 0;
}
