#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <array>
#include <tuple>
#include <vector>
#include <set>
#include <unordered_set>
using clk = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::clock;
using std::array;
using std::tuple;
using std::vector;
using std::multiset;
using std::unordered_multiset;
using std::generate_n;
using std::cout;
using std::endl;
using std::ofstream;

auto seconds(auto t) {
    return duration_cast<duration<float>>(t).count();
}

const int AVG_ITERS = 10;
const int MAX_N = 10000;

array<double,2> test(const int n, int* rand_array) {
    array<double,2> times{0,0};

    auto t0 = clk::now();
    for (int t = 0; t < AVG_ITERS; ++t) {
        vector<int> vec;
        t0 = clk::now();
        for (int i = 0; i < n; ++i) {
            int x = rand_array[t*n+i];
            auto p = lower_bound(vec.begin(),vec.end(),x);
            vec.insert(p,x);
        }
        times[0] += seconds(clk::now()-t0);
    }
    times[0] /= AVG_ITERS;

    for (int t = 0; t < AVG_ITERS; ++t) {
        multiset<int> binary_tree;
        t0 = clk::now();
        for (int i = 0; i < n; ++i)
            binary_tree.insert(rand_array[t*n + i]);
        times[1] += seconds(clk::now()-t0);
    }
    times[1] /= AVG_ITERS;

    return times;
}

int rand_array[AVG_ITERS*MAX_N];
vector<array<double,2>> times;

int main() {
    generate_n(rand_array, AVG_ITERS*MAX_N, rand);
    for (int n = 1; n <= MAX_N; ++n) {
        auto start = clk::now();
        times.push_back(test(n,rand_array));
        if (n % 1000 == 0)
            cout << n << '/' << MAX_N << endl;
    }
    ofstream out;
    out.open("times.txt");
    for (auto& [arr,tree] : times)
        out << arr << ' ' << tree << '\n';
    out.close();
}

