#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <algorithm>
using std::partition;
using std::swap;
using std::max;
using std::min;
#include <map>
using std::map;
#include <utility>
using std::pair;

void quicksort(auto& vec, int lower, int upper, auto& transpositions) {
    if (lower > upper)
        return;
    swap(vec[lower + rand() % (upper - lower + 1)], vec[upper]);
    auto [pivot, pivot_input_index] = vec[upper];
    int pivot_sorted_index = partition(vec.begin()+lower,vec.begin()+upper,[pivot](auto& el){ return el.first < pivot; }) - vec.begin();
    swap(vec[pivot_sorted_index], vec[upper]);
    int i = min(pivot_input_index, pivot_sorted_index);
    int j = max(pivot_input_index, pivot_sorted_index);
    if (transpositions.count(i) && transpositions[i] != j) {
        cout << "no" << endl; // then not a product of disjoint transpositions
        exit(1);
    } else if (i != j) {
        transpositions[i] = j;
    }
    quicksort(vec, lower, pivot_sorted_index-1, transpositions);
    quicksort(vec, pivot_sorted_index+1, upper, transpositions);
}

int main() {
    int N; cin >> N;
    vector<pair<int,int>> vec(N);
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        vec[i] = {x,i};
    }
    map<int,int> transpositions;
    quicksort(vec, 0, N-1, transpositions);

    if (transpositions.empty()) { // vec is the identity
        cout << "yes" << endl;
    } else if (transpositions.size() == 1) { // vec is a transposition
        auto [i,j] = *transpositions.begin();
        cout << "yes" << endl;
        cout << "swap " << i+1 << " " << j+1 << endl;
    } else if (transpositions.size() > 1) { // check if vec is a reversal
        bool is_reversal = true;
        auto [i0,j0] = *transpositions.begin();
        for (int i=i0, j=j0; i > j; i--, j++)
            if (!transpositions.count(i) || transpositions[i] != j) {
                is_reversal = false;
                break;
            }
        if (!is_reversal || (j0-i0+1) / 2 != transpositions.size()) {
            cout << "no" << endl;
            return 1;
        }
        cout << "yes" << endl;
        cout << "reverse " << i0 + 1  << " " << j0 + 1 << endl;
    }
}
