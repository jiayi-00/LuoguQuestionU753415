// 楠岃瘉 solve1e9.hpp锛氬鎷?O(L) 鍖洪棿 BFS
#include "solve1e9.hpp"
#include <random>
#include <cstdio>

// ---------- 鍙傝€冨疄鐜帮細鍖洪棿 BFS + 骞舵煡闆?----------
struct Ref {
    int L, t, k;
    vector<int> nxtR, nxtL, dist, que;
    vector<int> findR(int x) { return vector<int>(); } // placeholder
    int fr(int x) { while (nxtR[x] != x) { nxtR[x] = nxtR[nxtR[x]]; x = nxtR[x]; } return x; }
    int fl(int x) { while (nxtL[x] != x) { nxtL[x] = nxtL[nxtL[x]]; x = nxtL[x]; } return x; }
    void init(int L_, int t_, int k_) {
        L = L_; t = t_; k = k_;
        nxtR.resize(L + 3); nxtL.resize(L + 3);
        dist.assign(L + 1, -1); que.resize(L + 1);
        for (int i = 0; i <= L + 2; i++) { nxtR[i] = i; nxtL[i] = i; }
    }
    void erase_(int x) { nxtR[x] = fr(x + 1); nxtL[x] = fl(x - 1); }
    // 杩斿洖璺濈鏁扮粍
    const vector<int>& run(int a) {
        fill(dist.begin(), dist.end(), -1);
        for (int i = 0; i <= L + 2; i++) { nxtR[i] = i; nxtL[i] = i; }
        int head = 0, tail = 0;
        que[tail++] = a; dist[a] = 0; erase_(a);
        long long U = (long long)t + k;
        while (head < tail) {
            int x = que[head++];
            int d = dist[x] + 1;
            {
                long long l = (long long)x + t, r = min((long long)L, (long long)x + U);
                if (l <= r) {
                    int cur = fr((int)l);
                    while (cur <= r) { dist[cur] = d; que[tail++] = cur; erase_(cur); cur = fr(cur + 1); }
                }
            }
            {
                long long l = max(1LL, (long long)x - U), r = (long long)x - t;
                if (l <= r) {
                    int cur = fl((int)r);
                    while (cur >= l) { dist[cur] = d; que[tail++] = cur; erase_(cur); cur = fl(cur - 1); }
                }
            }
        }
        return dist;
    }
};

int main(int argc, char** argv) {
    Ref ref;
    long long bad = 0, checked = 0;
    // ---------- 1) 涓棿鍖虹┓涓?----------
    int TMAX = 26, KMAX = 20;
    if (argc > 1) TMAX = atoi(argv[1]);
    if (argc > 2) KMAX = atoi(argv[2]);
    for (int t = 1; t <= TMAX; t++) {
        for (int k = 1; k <= KMAX; k++) {
            for (int s = 1; s < k; s++) {
                int L = 2 * t + s;
                for (int a = 1; a <= L; a++) {
                    ref.init(L, t, k);
                    const vector<int>& d = ref.run(a);
                    for (int b = 1; b <= L; b++) {
                        ll f = one_d::solve(L, a, b, t, k);
                        ll truth = d[b] < 0 ? INF1 : d[b];
                        checked++;
                        if (f != truth) {
                            if (bad < 20) printf("MID MISMATCH L=%d a=%d b=%d t=%d k=%d truth=%lld got=%lld\n",
                                                 L, a, b, t, k, truth, f);
                            bad++;
                        }
                    }
                }
            }
        }
    }
    printf("middle exhaustive: checked=%lld bad=%lld\n", checked, bad);

    // ---------- 2) 鍏ㄨ寖鍥撮殢鏈哄皬鏁版嵁锛堜笁涓尯閮芥祴锛?----------
    std::mt19937 rng(12345);
    long long bad2 = 0, checked2 = 0;
    for (int iter = 0; iter < 200000; iter++) {
        int L = rng() % 120 + 1;
        int t = rng() % 100 + 1;
        int k = rng() % 100;
        int a = rng() % L + 1, b = rng() % L + 1;
        ref.init(L, t, k);
        const vector<int>& d = ref.run(a);
        ll f = one_d::solve(L, a, b, t, k);
        ll truth = d[b] < 0 ? INF1 : d[b];
        checked2++;
        if (f != truth) {
            if (bad2 < 20) printf("ANY MISMATCH L=%d a=%d b=%d t=%d k=%d truth=%lld got=%lld\n",
                                  L, a, b, t, k, truth, f);
            bad2++;
        }
    }
    printf("random small: checked=%lld bad=%lld\n", checked2, bad2);

    // ---------- 3) 闅忔満涓瓑鏁版嵁锛圠 鍒?2e5锛?----------
    long long bad3 = 0, checked3 = 0;
    for (int iter = 0; iter < 4000; iter++) {
        int L = rng() % 200000 + 1;
        int t = rng() % L + 1;
        int k = rng() % (L + 2);
        int a = rng() % L + 1, b = rng() % L + 1;
        ref.init(L, t, k);
        const vector<int>& d = ref.run(a);
        ll f = one_d::solve(L, a, b, t, k);
        ll truth = d[b] < 0 ? INF1 : d[b];
        checked3++;
        if (f != truth) {
            if (bad3 < 20) printf("BIG MISMATCH L=%d a=%d b=%d t=%d k=%d truth=%lld got=%lld\n",
                                  L, a, b, t, k, truth, f);
            bad3++;
        }
    }
    printf("random medium: checked=%lld bad=%lld\n", checked3, bad3);

    // ---------- 4) 涓棿鍖哄ぇ L 瀹氬悜闅忔満锛圠 鍒?3e5锛宼 澶?k 灏忥紝璐磋竟璧风偣锛?----------
    long long bad4 = 0, checked4 = 0;
    for (int iter = 0; iter < 20000; iter++) {
        int t = rng() % 90000 + 2;
        int kmax = (iter % 3 == 0) ? 3 : (int)(rng() % 40 + 2);
        int k = (int)(rng() % kmax) + 2;      // k >= 2
        int s = (int)(rng() % (k - 1)) + 1;   // 1 <= s <= k-1
        int L = 2 * t + s;
        int a = (iter % 3 == 0) ? (int)(rng() % min(L, 50) + 1) : (int)(rng() % L + 1);
        if (iter % 7 == 0) a = L + 1 - a;
        int b = (int)(rng() % L + 1);
        if (iter % 5 == 0) b = (int)(rng() % min(L, 50) + 1);
        ref.init(L, t, k);
        const vector<int>& d = ref.run(a);
        ll f = one_d::solve(L, a, b, t, k);
        ll truth = d[b] < 0 ? INF1 : d[b];
        checked4++;
        if (f != truth) {
            if (bad4 < 20) printf("MID-BIG MISMATCH L=%d a=%d b=%d t=%d k=%d s=%d truth=%lld got=%lld\n",
                                  L, a, b, t, k, s, truth, f);
            bad4++;
        }
    }
    printf("middle large random: checked=%lld bad=%lld\n", checked4, bad4);

    // ---------- 5) 贴边/极端 s 定向 ----------
    long long bad5 = 0, checked5 = 0;
    for (int iter = 0; iter < 60000; iter++) {
        int t = (int)(rng() % 60000) + 2;
        int k = (int)(rng() % 60) + 2;
        int s = (iter & 1) ? 1 : k - 1;
        if (s < 1 || s >= k) continue;
        int L = 2 * t + s;
        int edge = (int)(rng() % 25) + 1;
        int a = (iter % 4 == 0) ? edge : ((iter % 4 == 1) ? L + 1 - edge : (int)(rng() % L + 1));
        int b = (iter % 3 == 0) ? (int)(rng() % min(L, 25) + 1) : ((iter % 3 == 1) ? L - (int)(rng() % min(L, 25)) : (int)(rng() % L + 1));
        ref.init(L, t, k);
        const vector<int>& d = ref.run(a);
        ll f = one_d::solve(L, a, b, t, k);
        ll truth = d[b] < 0 ? INF1 : d[b];
        checked5++;
        if (f != truth) {
            if (bad5 < 20) printf("EDGE MISMATCH L=%d a=%d b=%d t=%d k=%d s=%d truth=%lld got=%lld\n",
                                  L, a, b, t, k, s, truth, f);
            bad5++;
        }
    }
    printf("edge targeted: checked=%lld bad=%lld\n", checked5, bad5);
    return (bad || bad2 || bad3 || bad4 || bad5) ? 1 : 0;
}

