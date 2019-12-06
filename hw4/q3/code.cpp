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
using std::array;
using std::copy_n;
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

const int AVG_ITERS = 8;
const int MAX_N = 1e7;

array<double,2> test(int n, int* rand_array) {
    array<double,2> times{0,0};

    auto t0 = clk::now();
    for (int t = 0; t < AVG_ITERS; ++t) {
        multiset<int> binary_tree;
        t0 = clk::now();
        for (int i = 0; i < n; ++i)
            binary_tree.insert(rand_array[t*n + i]);
        times[0] += seconds(clk::now()-t0);
    }
    times[0] /= AVG_ITERS;

    for (int t = 0; t < AVG_ITERS; ++t) {
        unordered_multiset<int> hash_table;
        t0 = clk::now();
        for (int i = 0; i < n; ++i)
            hash_table.insert(rand_array[t*n + i]);
        times[1] += seconds(clk::now()-t0);
    }
    times[1] /= AVG_ITERS;

    return times;
}

int rand_array[AVG_ITERS*MAX_N];
int x[AVG_ITERS*MAX_N];
vector<tuple<int,double,double>> times;

int main() {
    ofstream out;
    out.open("times.txt");
    double time_taken = 0;
    double n = 1;
    int i = 0;
    generate_n(x, AVG_ITERS*MAX_N, rand);
    while (time_taken < 3 and n <= MAX_N) {
        copy_n(x,AVG_ITERS*int(n),rand_array);
        auto result = test(int(n),rand_array);
        times.emplace_back(int(n),result[0],result[1]);
        time_taken = (result[0] + result[1]) / 2.;
        cout << i << ' ' << int(n) << ' ' << time_taken << endl;
        n *= 3;
        i += 1;
    }
    for (auto& [n,tree,table] : times)
        out << n << ' ' << tree << ' ' << table << '\n';
    out.close();
}

