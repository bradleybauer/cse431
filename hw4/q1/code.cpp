#include <algorithm>
#include <array>
#include <ctime>
#include <iostream>
#include <fstream>
#include <chrono>
using std::partition;
using std::lower_bound;
using std::generate_n;
using std::copy_n;
using std::copy_backward;
using std::array;
using std::swap;
using std::cout;
using std::endl;
using std::ofstream;
using clk = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

auto seconds(auto t) {
    return duration_cast<duration<float>>(t).count();
}

void quicksort(auto* vec, const int lower, const int upper) {
    if (lower >= upper)
        return;
    auto pivot = vec[upper];
    int pivot_index = partition(vec+lower,vec+upper,[pivot](auto& x){ return x < pivot; }) - vec;
    swap(vec[pivot_index], vec[upper]);
    quicksort(vec, lower, pivot_index-1);
    quicksort(vec, pivot_index+1, upper);
}

void quicksort(auto* vec, int n) {
    quicksort(vec, 0, n-1);
}

// from geeks for geeks, needed something quick.
void insertionsort(auto* arr, const int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

const int AVG_ITERS = 500;
const int MAX_N = 700;

array<double,2> test(const int n, auto* rand_array1, auto* rand_array2) {
    array<double,2> timings;

    auto t0 = clk::now();
    for (int i = 0; i < AVG_ITERS; ++i)
        insertionsort(rand_array1 + i * n, n);
    timings[0] = seconds(clk::now()-t0) / AVG_ITERS;

    t0 = clk::now();
    for (int i = 0; i < AVG_ITERS; ++i)
        quicksort(rand_array2 + i * n, n);
    timings[1] = seconds(clk::now()-t0) / AVG_ITERS;

    return timings;
}

int main() {
    int* rand_array1 = new int[AVG_ITERS*MAX_N];
    int* rand_array2 = new int[AVG_ITERS*MAX_N];
    int* orig = new int[AVG_ITERS*MAX_N];
    array<array<double,2>, MAX_N> times;
    srand(120);
    generate_n(orig, AVG_ITERS*MAX_N, rand);
    for (int n = 1; n <= MAX_N; ++n) {
        copy_n(orig, AVG_ITERS*n, rand_array1);
        copy_n(orig, AVG_ITERS*n, rand_array2);
        times[n-1] = test(n, rand_array1, rand_array2);
    }
    ofstream out;
    out.open("times.txt");
    for (auto& [quick,insert] : times)
        out << quick << ' ' << insert << endl;
    out.close();
    delete rand_array1;
    delete rand_array2;
    delete orig;
}
