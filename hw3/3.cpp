#include<iostream>
#include<vector>
using namespace std;
int main() {
    int N,K;
    string B;
    cin >> N >> K >> B;
    long count = B[0]=='1';
    vector<int> solution{B[0]=='1'};
    for (int i = 1; i < N; ++i) {
        int expected = B[i] == '1';
        int unknown = (count + expected) % 2;
        count += unknown;
        if (i >= K-1)
            count -= solution[i-K+1];
        solution.push_back(unknown);
    }
    for (auto c : solution)
        cout << c;
    cout << endl;
}
