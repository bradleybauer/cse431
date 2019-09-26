#include<iostream>
#include<cmath>

int main()
{
  int T; // Number of test cases

  std::cin >> T;

  int N = 1e6;
  for (int i = 0; i < T; i++) {

    int S; // Starting score
    int k; // Number of times exploit is used

    std::cin >> S >> k;
    for (int j = 0; j < k; ++j) {
      if (S % 2) {
        S = (S - 15) * 2;
      } else {
        S = (S - 99) * 3;
      }
      S = (S % N + N) % N;
    }

    std::cout << S << std::endl;
  }
}
