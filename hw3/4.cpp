#include <bits/stdc++.h>
using namespace std;

// q  = S / d
// 1 + q * ans(d)
// 1 for the crystal that is broken during this call
// q is the number of crystals that have been created
// q * ans(d) counts the number of hours that we get for the q crystals of size d

auto ans(const auto& F, auto& memo, auto S) {
    if (memo.count(S))
        return memo[S];
    for (auto d : F)
        if (S > d && S % d == 0)
            memo[S] = max(memo[S], 1 + S / d * ans(F, memo, d));
    return memo[S];
}

int main() {
    uint64_t S,K;
    cin >> S;
    while (cin >> S >> K) {
        vector<uint64_t> F(K);
        unordered_map<uint64_t,uint64_t> memo;
        copy_n(istream_iterator<uint64_t>(cin), K, F.begin());
        cout << ans(F, memo, S) << endl;
    }
}
