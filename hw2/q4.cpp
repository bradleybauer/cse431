#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <algorithm>
using std::sort;
using std::max;
#include <vector>
using std::vector;
int main() {
    long N, K;
    cin >> N >> K;
    vector<long> positions;
    for (int i = 0; i < K; ++i) {
        int x;
        cin >> x;
        positions.push_back(x);
    }
    sort(positions.begin(),positions.end());
    long max_dist = max(positions[0], (N - 1 - positions.back()));
    for (int i = 1; i < K; ++i) {
        max_dist = max(max_dist, (positions[i]-positions[i-1]) / 2);
    }
    cout << max_dist << endl;
}
