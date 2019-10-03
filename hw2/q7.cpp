#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <iomanip>
using std::fixed;
#include <vector>
using std::vector;
int main() {
    cout.precision(1);
    cout << fixed;

    int N;
    cin >> N;
    vector<long> tree;
    auto iter = tree.begin();
    double median;
    for (int n = 0; n < N; ++n) {
        char ar;
        cin >> ar;
        long x;
        cin >> x;

        if (ar == 'a') {
            auto pos = lower_bound(tree.begin(), tree.end(), x);
            tree.insert(pos, x);
        }

        if (ar == 'r' and !tree.empty()) {
            auto to_remove = lower_bound(tree.begin(), tree.end(), x);
            if (to_remove == tree.end() || *to_remove != x) {
                cout << "Wrong!" << endl;
                continue;
            }
            tree.erase(to_remove);
        }

        if (tree.empty()) {
            cout << "Wrong!" << endl;
            continue;
        }

        if (tree.size() % 2) {
            median = tree[tree.size()/2];
            cout << long(median) << endl;
        } else {
            long l = tree[tree.size()/2-1];
            long r = tree[tree.size()/2];
            median = l/2.+r/2.;
            if ((r%2+2)%2==(l%2+2)%2) {
                cout << long(median) << endl;
            } else {
                cout << median << endl;
            }
        }
    }
}
