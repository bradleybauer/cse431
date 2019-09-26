#include <iostream>
using namespace std;
int main() {
  int N;
  cin >> N;
  for (int i = 0; i < N; ++i) {
    int n, m, c;
    cin >> n >> m >> c;
    int g = n / m;
    int G = g;
    while (g >= c) {
      G += g / c;
      g = g / c + g % c;
    }
    cout << G << endl;
  }
}
