#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Interval {
    int l, r;
};

// 鎵╁睍涓€灞傚彲杈惧尯闂?vector<Interval> expand(const vector<Interval>& cur, int N, int t, int k) {
    int m = cur.size();
    vector<Interval> left, right;
    left.reserve(m);
    right.reserve(m);
    long long tk = (long long)t + k;

    for (auto &iv : cur) {
        // 鍚戝乏璧?        long long l1 = (long long)iv.l - tk;
        long long r1 = (long long)iv.r - t;
        if (l1 < 1) l1 = 1;
        if (r1 > N) r1 = N;
        if (l1 <= r1) left.push_back({(int)l1, (int)r1});

        // 鍚戝彸璧?        long long l2 = (long long)iv.l + t;
        long long r2 = (long long)iv.r + tk;
        if (l2 < 1) l2 = 1;
        if (r2 > N) r2 = N;
        if (l2 <= r2) right.push_back({(int)l2, (int)r2});
    }

    // 褰掑苟 left 鍜?right
    vector<Interval> res;
    res.reserve(left.size() + right.size());
    int i = 0, j = 0;
    while (i < (int)left.size() || j < (int)right.size()) {
        Interval curIv;
        if (j >= (int)right.size() || (i < (int)left.size() && left[i].l <= right[j].l)) {
            curIv = left[i++];
        } else {
            curIv = right[j++];
        }
        if (res.empty()) {
            res.push_back(curIv);
        } else {
            Interval &last = res.back();
            if (curIv.l <= last.r + 1) {
                if (curIv.r > last.r) last.r = curIv.r;
            } else {
                res.push_back(curIv);
            }
        }
    }
    return res;
}

// 妫€鏌ヤ袱涓湁搴忓尯闂村垪琛ㄦ槸鍚︽湁浜ら泦
bool intersect(const vector<Interval>& A, const vector<Interval>& B) {
    int i = 0, j = 0;
    while (i < (int)A.size() && j < (int)B.size()) {
        int l = max(A[i].l, B[j].l);
        int r = min(A[i].r, B[j].r);
        if (l <= r) return true;
        if (A[i].r < B[j].r) i++;
        else j++;
    }
    return false;
}

// 涓€缁存渶灏戞鏁帮紝鑻ヨ秴杩?limit 杩斿洖 INF
int solve1D(int N, int a, int b, int t, int k, int limit) {
    if (a == b) return 0;
    if (t > N - 1) return INF; // 鏃犳硶绉诲姩

    if (k == 0) {
        int D = abs(a - b);
        if (D % t != 0) return INF;
        int steps = D / t;
        return steps <= limit ? steps : INF;
    }

    // 鍙屽悜 BFS
    vector<Interval> A = {{a, a}};
    vector<Interval> B = {{b, b}};
    int stepsA = 0, stepsB = 0;
    long long tk = (long long)t + k;

    while (true) {
        if (stepsA + stepsB >= limit) return INF;

        if (stepsA <= stepsB) {
            // 澶у尯闂翠紭鍖栵細鑻?A 宸叉槸涓€涓暱搴?>= 2t 鐨勮繛缁尯闂?            if (A.size() == 1 && (long long)A[0].r - A[0].l + 1 >= 2LL * t) {
                int need = INF;
                int L = A[0].l, R = A[0].r;
                for (auto &iv : B) {
                    if (iv.r < L) {
                        int d = L - iv.r;
                        int nd = (d + tk - 1) / tk;
                        if (nd < need) need = nd;
                    } else if (iv.l > R) {
                        int d = iv.l - R;
                        int nd = (d + tk - 1) / tk;
                        if (nd < need) need = nd;
                    } else {
                        need = 0;
                        break;
                    }
                }
                if (need != INF) { cerr << "SHORTCUT " << stepsA << " " << stepsB << " " << need << "\n";
                    long long total = (long long)stepsA + stepsB + need;
                    return total <= limit ? (int)total : INF;
                }
            }

            A = expand(A, N, t, k);
            stepsA++;
            if (intersect(A, B)) { cerr << "MEET " << stepsA << " " << stepsB << "\n";
                long long total = (long long)stepsA + stepsB;
                return total <= limit ? (int)total : INF;
            }
        } else {
            // 澶у尯闂翠紭鍖栵細鑻?B 宸叉槸涓€涓暱搴?>= 2t 鐨勮繛缁尯闂?            if (B.size() == 1 && (long long)B[0].r - B[0].l + 1 >= 2LL * t) {
                int need = INF;
                int L = B[0].l, R = B[0].r;
                for (auto &iv : A) {
                    if (iv.r < L) {
                        int d = L - iv.r;
                        int nd = (d + tk - 1) / tk;
                        if (nd < need) need = nd;
                    } else if (iv.l > R) {
                        int d = iv.l - R;
                        int nd = (d + tk - 1) / tk;
                        if (nd < need) need = nd;
                    } else {
                        need = 0;
                        break;
                    }
                }
                if (need != INF) { cerr << "SHORTCUT " << stepsA << " " << stepsB << " " << need << "\n";
                    long long total = (long long)stepsA + stepsB + need;
                    return total <= limit ? (int)total : INF;
                }
            }

            B = expand(B, N, t, k);
            stepsB++;
            if (intersect(A, B)) { cerr << "MEET " << stepsA << " " << stepsB << "\n";
                long long total = (long long)stepsA + stepsB;
                return total <= limit ? (int)total : INF;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    int s, k, t;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    int row_steps = solve1D(n, x1, x2, t, k, s);
    if (row_steps > s) {
        cout << "NO\n";
        return 0;
    }

    int remain = s - row_steps;
    int col_steps = solve1D(m, y1, y2, t, k, remain);
    if (col_steps > remain) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES " << row_steps + col_steps << '\n';
    return 0;
}

