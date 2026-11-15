// 验证单文件版 math_1e9_single.cpp 的一维公式（与并查集区间 BFS 对拍）
#define main main_disabled
#include "math_1e9_single.cpp"
#undef main

#include <cstdio>
#include <random>
#include <vector>
using namespace std;

struct Ref {
    int L, t, k;
    vector<int> nxtR, nxtL, dist, que;
    int fr(int x) { while (nxtR[x] != x) { nxtR[x] = nxtR[nxtR[x]]; x = nxtR[x]; } return x; }
    int fl(int x) { while (nxtL[x] != x) { nxtL[x] = nxtL[nxtL[x]]; x = nxtL[x]; } return x; }
    void init(int L_, int t_, int k_) {
        L = L_; t = t_; k = k_;
        nxtR.assign(L + 3, 0); nxtL.assign(L + 3, 0);
        dist.assign(L + 1, -1); que.assign(L + 1, 0);
    }
    void erase_(int x) { nxtR[x] = fr(x + 1); nxtL[x] = fl(x - 1); }
    const vector<int> &run(int a) {
        for (int i = 0; i <= L + 2; i++) { nxtR[i] = i; nxtL[i] = i; }
        fill(dist.begin(), dist.end(), -1);
        int head = 0, tail = 0;
        que[tail++] = a; dist[a] = 0; erase_(a);
        ll U = (ll)t + k;
        while (head < tail) {
            int x = que[head++], d = dist[x] + 1;
            ll l = (ll)x + t, r = min((ll)L, (ll)x + U);
            if (l <= r) { int cur = fr((int)l); while (cur <= r) { dist[cur] = d; que[tail++] = cur; erase_(cur); cur = fr(cur + 1); } }
            l = max(1LL, (ll)x - U); r = (ll)x - t;
            if (l <= r) { int cur = fl((int)r); while (cur >= l) { dist[cur] = d; que[tail++] = cur; erase_(cur); cur = fl(cur - 1); } }
        }
        return dist;
    }
};

int main() {
    Ref ref;
    long long bad = 0, checked = 0;
    // 中间区穷举
    for (int t = 1; t <= 14; t++) {
        for (int k = 2; k <= 14; k++) {
            for (int s = 1; s < k; s++) {
                int L = 2 * t + s;
                for (int a = 1; a <= L; a++) {
                    ref.init(L, t, k);
                    const vector<int> &d = ref.run(a);
                    for (int b = 1; b <= L; b++) {
                        ll f = solve1D(L, a, b, t, k);
                        ll truth = d[b] < 0 ? INF : d[b];
                        checked++;
                        if (f != truth) {
                            if (bad < 10) printf("MID L=%d a=%d b=%d t=%d k=%d truth=%lld got=%lld\n", L, a, b, t, k, truth, f);
                            bad++;
                        }
                    }
                }
            }
        }
    }
    // 全范围随机
    mt19937 rng(2026);
    for (int it = 0; it < 200000; it++) {
        int L = rng() % 120 + 1, t = rng() % 100 + 1, k = rng() % 100;
        int a = rng() % L + 1, b = rng() % L + 1;
        ref.init(L, t, k);
        const vector<int> &d = ref.run(a);
        ll f = solve1D(L, a, b, t, k);
        ll truth = d[b] < 0 ? INF : d[b];
        checked++;
        if (f != truth) {
            if (bad < 10) printf("ANY L=%d a=%d b=%d t=%d k=%d truth=%lld got=%lld\n", L, a, b, t, k, truth, f);
            bad++;
        }
    }
    printf("single-file check: checked=%lld bad=%lld\n", checked, bad);
    return bad ? 1 : 0;
}
