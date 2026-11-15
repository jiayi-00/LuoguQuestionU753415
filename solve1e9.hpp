// 一维子问题 O(1) 求解器：《棋盘上的虫子》1e9 版本
// f(L,a,b) = 长度 L 的数轴 1..L，从 a 到 b，每步长度 d∈[t,t+k]，不越界，最少步数
//
// 分段：
//   L <= 2t        窄棋盘（两区往返漂移）
//   L >= 2t+k      宽棋盘（边界无关，等价无边界）
//   2t < L < 2t+k  中间区（二层"攀爬 + 换层"模型）
#pragma once
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF1 = (ll)4e18;

static inline ll cd_(ll a, ll b) { // ceil(a/b), b>0
    return a >= 0 ? (a + b - 1) / b : -((-a) / b);
}

namespace one_d {

// ---------------- 窄棋盘公式 L <= 2t ----------------
ll narrow(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF1;
    ll M = L - t;
    ll q = min(k, M - 1);
    // side: 0=左区(u=p), 1=右区(u=p-t), -1=死区
    auto side = [&](ll p, ll &u) {
        if (p <= M) { u = p; return 0; }
        if (p >= t + 1) { u = p - t; return 1; }
        return -1;
    };
    ll ua, ub;
    int sa = side(a, ua), sb = side(b, ub);
    if (sa < 0 || sb < 0) return INF1;
    if (sa == sb) {
        if (ua == ub) return 0;
        if (q == 0) return INF1;
        return 2 * cd_(llabs(ua - ub), q);
    }
    if (q == 0) return ua == ub ? 1 : INF1;
    ll d = 0;
    if (sa == 0) { // 左 -> 右
        if (ub > ua) d = max(d, cd_(ub - ua, q) - 1);
        else         d = max(d, cd_(ua - ub, q));
    } else {       // 右 -> 左
        if (ua > ub) d = max(d, cd_(ua - ub, q) - 1);
        else         d = max(d, cd_(ub - ua, q));
    }
    return 2 * max(d, 0LL) + 1;
}

// ---------------- 宽棋盘公式 L >= 2t+k ----------------
ll wide(ll D, ll t, ll k) {
    D = llabs(D);
    if (D == 0) return 0;
    if (k == 0) return D % t == 0 ? D / t : INF1;
    ll c[6]; int n = 0;
    c[n++] = 0;
    c[n++] = D / t; c[n++] = D / t + 1;
    ll w = cd_(D, t + k);
    c[n++] = w - 1; c[n++] = w; c[n++] = w + 1;
    ll ans = INF1;
    for (int i = 0; i < n; i++) {
        ll m = c[i];
        if (m < 0) continue;
        ll q = max(0LL, max(cd_(m * t - D, k), cd_(D - m * (t + k), k)));
        ans = min(ans, m + 2 * q);
    }
    return ans;
}

// ---------------- 中间区 ----------------
// 记 c = L-t, s = L-2t (1 <= s <= k-1)。
// 物理位置 p<=c 可上跳: p -> [p+t, p+t+k]；p>=t+1 可下跳: p -> [p-t-k, p-t]。
// "攀爬"（一次上跳+一次下跳）在坐标 u∈[1,c] 上等价于 u -> [u-k, u+k]，代价 2。
// 换层发生在重叠区：L0 u (u∈[t+1,t+s]) == L1 (u-t)。

static inline ll climb(ll x, ll y, ll k) {
    if (x == y) return 0;
    return 2 * cd_(llabs(x - y), k);
}

// 严格交替走法，第一步必须向上（要求起点 a <= c）
ll alt_up(ll L, ll a, ll b, ll t, ll k) {
    ll c = L - t;
    if (a > c) return INF1;
    ll best = INF1;
    if (b <= c) best = min(best, a == b ? 0 : 2 * cd_(llabs(a - b), k));
    if (b >= t + 1) {
        ll lo = max(1LL, b - t - k), hi = min(c, b - t);
        if (lo <= hi) {
            ll j;
            if (a < lo) j = cd_(lo - a, k);
            else if (a > hi) j = cd_(a - hi, k);
            else j = 0;
            best = min(best, 2 * j + 1);
        }
    }
    return best;
}

static inline ll clampv(ll x, ll lo, ll hi) { return x < lo ? lo : (x > hi ? hi : x); }

struct Cand {
    ll v[64]; int n = 0;
    void push(ll x) { if (n < 64) v[n++] = x; }
};

// 把 [lo,hi] 中模 k 余 r 的最小/最大代表加入 out
static void add_residue(Cand& out, ll lo, ll hi, ll r, ll k) {
    if (lo > hi || k <= 0) return;
    ll rr = ((r % k) + k) % k;
    ll first = lo + (((rr - lo) % k) + k) % k;
    if (first <= hi) out.push(first);
    ll last = hi - ((((hi % k) - rr) % k + k) % k);
    if (last >= lo) out.push(last);
}

// 只考虑起点在 L0 一侧 (a<=c) 的走法：纯交替，或在重叠区换层一次
ll dir_solve(ll L, ll a, ll b, ll t, ll k) {
    ll c = L - t, s = L - 2 * t;
    if (a > c) return INF1;
    ll best = alt_up(L, a, b, t, k);

    const ll baseY[9] = {a - t, b, b - t, b - t - k, b + k, a, a + t, 1, s};
    const ll baseZ[9] = {a + t, b, b - t, b - t - k, b + k, a, a - t, t + 1, t + s};

    // ---- 换层向下：L0 u∈[t+1,t+s] --下跳--> L0 y∈[1,s]
    {
        Cand ys;
        for (int i = 0; i < 9; i++) for (ll d = -1; d <= 1; d++) ys.push(clampv(baseY[i] + d, 1, s));
        for (int i = 0; i < 9; i++) add_residue(ys, 1, s, baseY[i], k);
        for (int i = 0; i < ys.n; i++) {
            ll y = ys.v[i];
            if (y < 1 || y > s) continue;
            ll lo = y + t, hi = min(t + s, y + t + k);
            if (lo > hi) continue;
            ll v = alt_up(L, y, b, t, k);
            if (v >= INF1) continue;
            Cand us;
            us.push(clampv(a, lo, hi));
            us.push(lo); us.push(hi);
            add_residue(us, lo, hi, a, k);
            add_residue(us, lo, hi, b, k);
            add_residue(us, lo, hi, b - t, k);
            for (int j = 0; j < us.n; j++) best = min(best, climb(a, us.v[j], k) + 1 + v);
        }
    }
    // ---- 换层向上：L0 u∈[1,s] --上跳+换层--> L0 z∈[t+1,t+s]
    {
        Cand zs;
        for (int i = 0; i < 9; i++) for (ll d = -1; d <= 1; d++) zs.push(clampv(baseZ[i] + d, t + 1, t + s));
        for (int i = 0; i < 9; i++) add_residue(zs, t + 1, t + s, baseZ[i], k);
        for (int i = 0; i < zs.n; i++) {
            ll z = zs.v[i];
            if (z < t + 1 || z > t + s) continue;
            ll lo = max(1LL, z - t - k), hi = min(s, z - t);
            if (lo > hi) continue;
            ll v = alt_up(L, z, b, t, k);
            if (v >= INF1) continue;
            Cand us;
            us.push(clampv(a, lo, hi));
            us.push(lo); us.push(hi);
            add_residue(us, lo, hi, a, k);
            add_residue(us, lo, hi, z - t, k);
            for (int j = 0; j < us.n; j++) best = min(best, climb(a, us.v[j], k) + 1 + v);
        }
    }
    return best;
}

ll middle(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    ll m1 = L + 1 - a, m2 = L + 1 - b;
    ll best = INF1;
    best = min(best, dir_solve(L, a, b, t, k));
    best = min(best, dir_solve(L, b, a, t, k));
    best = min(best, dir_solve(L, m1, m2, t, k));
    best = min(best, dir_solve(L, m2, m1, t, k));
    return best;
}

// ---------------- 总入口 ----------------
ll solve(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF1;
    if (L <= 2 * t) return narrow(L, a, b, t, k);
    if (L >= 2 * t + k) return wide(b - a, t, k);
    return middle(L, a, b, t, k);
}

} // namespace one_d
