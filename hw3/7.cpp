#include <bits/stdc++.h>
using namespace std;
const int my_fav_prime = 1000000009;
int main() {
    int N,L;
    cin >> N;
    vector<int> dp(1000001);
    while (cin >> N >> L) {
        vector<int> Ls(N);
        copy_n(istream_iterator<int>(cin), N, Ls.begin());
        sort(Ls.begin(), Ls.end());
        dp[0] = 1;
        for (int i = 1; i <= L; ++i) {
            dp[i] = 0;
            for (int Li : Ls) {
                if (Li > i)
                    break;
                dp[i] = (dp[i] + dp[i-Li]) % my_fav_prime;
            }
        }
        cout << dp[L] << endl;
    }
}
