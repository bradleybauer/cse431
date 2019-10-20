#include<iostream>
#include<algorithm>
using namespace std;
int main() {
  long i, i1, i2 = 0, i1i = 1;
  cin >> i1 >> i1;
  while (cin >> i) {
    long temp = i1;
    if (i1i)
      i1 -= i;
    i1i = i2+i >= temp;
    i1 = i + max(i1,i2);
    i2 = temp;
  }
  cout << i1 << endl;
}
