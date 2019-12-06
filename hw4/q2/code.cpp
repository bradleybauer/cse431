#include <algorithm>
#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include <fstream>
using clk = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::partition;
using std::lower_bound;
using std::generate_n;
using std::copy_n;
using std::swap;
using std::vector;
using std::array;
using std::cout;
using std::endl;
using std::ofstream;

// from geeks for geeks, needed something quick.
void insertionsort(auto* arr, const int begin, const int end) {
    int i, key, j;
    for (i = begin+1; i < end; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void hybridsort(auto* vec, const int lower, const int upper, const int k, const bool hybrid) {
    if (upper <= lower)
        return;
    if (upper - lower <= k && hybrid)
        return insertionsort(vec, lower, upper+1);
    auto pivot = vec[upper];
    int pivot_index = partition(vec+lower,vec+upper,[pivot](auto& el){ return el < pivot; }) - vec;
    swap(vec[pivot_index], vec[upper]);
    hybridsort(vec, lower, pivot_index-1,k,hybrid);
    hybridsort(vec, pivot_index+1, upper,k,hybrid);
}

void quicksort(auto* vec, int n) {
    hybridsort(vec, 0, n-1, 0, false);
}

void hybridsort(auto* vec, int n, int k) {
    hybridsort(vec, 0, n-1, k, true);
}

void insertionsort(auto* vec, int n) {
    insertionsort(vec, 0, n);
}

auto seconds(auto t) {
    return duration_cast<duration<float>>(t).count();
}

const int AVG_ITERS = 1;
const int MAX_K = 200;
const int MAX_N = 700;

array<double, 3> test(const int n, const int k, auto* x, auto* y, auto* z) {
    array<double,3> times;
    auto t0 = clk::now();
    for (int i = 0; i < AVG_ITERS; ++i)
        hybridsort(x + n*i, n, k);
    times[0] = seconds(clk::now()-t0) / AVG_ITERS;

    t0 = clk::now();
    for (int i = 0; i < AVG_ITERS; ++i)
        quicksort(y + n*i, n);
    times[1] = seconds(clk::now()-t0) / AVG_ITERS;

    t0 = clk::now();
    for (int i = 0; i < AVG_ITERS; ++i)
        insertionsort(z + n*i, n);
    times[2] = seconds(clk::now()-t0) / AVG_ITERS;
    return times;
}

// Which value of k gives gives best avg perf
int main() {
    int* orig = new int[AVG_ITERS*MAX_N];
    int* x = new int[AVG_ITERS*MAX_N];
    int* y = new int[AVG_ITERS*MAX_N];
    int* z = new int[AVG_ITERS*MAX_N];
    srand(120);
    generate_n(orig, AVG_ITERS*MAX_N, rand);

    array<vector<array<double,4>>, MAX_K> k_to_times;
    for (int k = 1; k <= MAX_K; k++) {
        cout << k << '/' << MAX_K << endl;
        for (int n = 1; n <= MAX_N; ++n) {
            copy_n(orig, AVG_ITERS*n, x); // reshuffles
            copy_n(orig, AVG_ITERS*n, y);
            copy_n(orig, AVG_ITERS*n, z);
            auto times = test(n,k,x,y,z);
            k_to_times[k-1].push_back({double(k), times[0], times[1], times[2]});
        }
    }
    ofstream out;
    out.open("times.txt");
    for (auto& v : k_to_times) {
        for (auto& [k,hybrid,quick,insert] : v)
            out << int(k) << ' ' << hybrid << ' ' << quick << ' ' << insert << '\n';
    }
    out.close();
    delete x;
    delete y;
    delete z;
    delete orig;
}

