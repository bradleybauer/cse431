#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <stack>
using std::stack;
int main() {
    stack<long> max_stack;
    stack<long> artifacts;
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        long command;
        long x;
        cin >> command;

        if (command == 1) {
            cin >> x;
            artifacts.push(x);
            if (max_stack.empty())
                max_stack.push(x);
            else
                max_stack.push(std::max(x,max_stack.top()));
        } else if (command == 2) {
            artifacts.pop();
            max_stack.pop();
        } else {
            cout << max_stack.top() << endl;
        }
    }
}
