#include <bits/stdc++.h>
using namespace std;

int N,M;
vector<vector<bool>> board;

bool valid(int n, int r, int c) {
    int rp = r - n/2;
    int cp = c - n/2;
    for (int i = 0; i < n; ++i)
        if (board[rp+i][c] or board[r][cp+i])
            return false;
    return true;
}

bool overlap(int n, int r0, int c0, int m, int r1, int c1) {
    int adr = abs(r0-r1);
    int adc = abs(c0-c1);
    if (r0 != r1 and c0 != c1)
        return ((adr<=n/2 and adc<=m/2)
             or (adr<=m/2 and adc<=n/2));
    if (r0 == r1)
        return adc - 1 < n/2 + m/2;
    return adr - 1 < n/2 + m/2;
}

int main() {
    cin >> N >> M;
    for (int i = 0; i < N; ++i) {
        board.push_back({});
        for (int j = 0; j < M; ++j) {
            char c;
            cin >> c;
            board.back().push_back(c=='B');
        }
    }

    int sz = min(N,M) / 2 * 2 + 1;

    // find all possible positions for a cross of size n
    unordered_map<int,vector<pair<int,int>>> sz2xs;
    for (int n = sz; n > 0; n -= 2)
        for (int r = n/2; r < N-n/2; ++r)
            for (int c = n/2; c < M-n/2; ++c)
                if (valid(n,r,c))
                    sz2xs[n].emplace_back(r,c);

    int max_ = 0;
    int max_m = 0;

    // given two crosses, check if they fit on the board without overlap
    for (int n = sz; n > max_m; n -= 2) {
        auto& nxs = sz2xs[n];
        for (int m = n; m > max_m; m -= 2) {
            auto& mxs = sz2xs[m];
            for (auto [r0,c0] : nxs) {
                for (auto [r1,c1] : mxs) {
                    if (!overlap(n,r0,c0,m,r1,c1)) {
                        max_ = max((n*2-1)*(m*2-1), max_);
                        max_m = m;
                        goto BREAK;
                    }
                }
            }
        }
        BREAK:;
    }
    cout << max_ << endl;
}

// #include <bits/stdc++.h>
// using namespace std;
//
// int N,M;
// vector<vector<bool>> board;
//
// bool valid(int n, int r, int c) {
//     int rp = r - n/2;
//     int cp = c - n/2;
//     for (int i = 0; i < n; ++i)
//         if (board[rp+i][c] or board[r][cp+i])
//             return false;
//     return true;
// }
//
// bool overlap(int n, int r0, int c0, int m, int r1, int c1) {
//     int adr = abs(r0-r1);
//     int adc = abs(c0-c1);
//     if (r0 != r1 and c0 != c1)
//         return ((adr<=n/2 and adc<=m/2)
//              or (adr<=m/2 and adc<=n/2));
//     if (r0 == r1)
//         return adc - 1 < n/2 + m/2;
//     return adr - 1 < n/2 + m/2;
// }
//
// int main() {
//     cin >> N >> M;
//     for (int i = 0; i < N; ++i) {
//         board.push_back({});
//         for (int j = 0; j < M; ++j) {
//             char c;
//             cin >> c;
//             board.back().push_back(c=='B');
//         }
//     }
//
//     int sz = min(N,M) / 2 * 2 + 1;
//     int max_ = 0;
//     int max_m = 0;
//
//     for (int n = sz; n > max_m; n -= 2) {
//         for (int m = n; m > 0; m -= 2) {
//             for (int r0 = n/2; r0 < N-n/2; ++r0) {
//                 for (int c0 = n/2; c0 < M-n/2; ++c0) {
//                     if (!valid(n,r0,c0))
//                         continue;
//                     for (int r1 = m/2; r1 < N-m/2; ++r1) {
//                         for (int c1 = m/2; c1 < M-m/2; ++c1) {
//                             if (!valid(m,r1,c1))
//                                 continue;
//                             if (!overlap(n,r0,c0,m,r1,c1)) {
//                                 max_ = max((n*2-1)*(m*2-1), max_);
//                                 max_m = m;
//                                 goto BREAK;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         BREAK:;
//     }
//
//     cout << max_ << endl;
// }
