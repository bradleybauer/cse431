// Hey, i can count!
#include <bits/stdc++.h>
using namespace std;
using I = int64_t;
const I P = 1000000007;
// compute x^n (mod P)
I power(I x, I h) {
    I y = 1;
    while (h) {
        if (h & 1)
            y = (y * x) % P;
        x = (x * x) % P;
        h >>= 1;
    }
    return y;
}
int main() {
    I T,H,L;
    cin >> T;
    for (I t = 0; t < T; ++t) {
        cin >> H >> L;
        vector<I> oxi(L+1, 0); // total num confs for 1 by i case
        vector<I> ttl(L+1, 0); // total num confs for H by i case
        vector<I> inv(L+1, 0); // num invalid confs for H by i case
        vector<I> val(L+1, 0); // num valid confs for H by i case
        oxi[0] = 1;
        for (I i = 1; i <= L; ++i) {
            for (I ml : {1,2,3,4}) {
                if (ml > i)
                    break;
                oxi[i] = (oxi[i] + oxi[i-ml]) % P;
            }
            ttl[i] = power(oxi[i], H);
            // valid confs = total confs - invalid confs
            for (I j = 1; j < i; ++j)
                inv[i] = (inv[i] + val[j] * ttl[i-j]) % P;
            val[i] = (ttl[i] - inv[i] + P) % P;
        }
        cout << val[L] << endl;
    }
}
