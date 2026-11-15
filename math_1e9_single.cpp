// ============================================================================
//  《棋盘上的虫子》1e9 版本 —— 单文件标程（提交用）
//
//  题意：n×m 棋盘，从 A(x1,y1) 出发，每步沿上下左右走 d 格，t <= d <= t+k，
//        不能越界；问 s 步内能否到 B(x2,y2)，能则输出最少步数。
//  数据范围：1 <= n,m,s,t <= 1e9，0 <= k <= 1e9。
//
//  做法：
//    1) 二维可以拆开：答案 = f(n,x1,x2) + f(m,y1,y2)，其中 f 是一维问题
//       （任意两条一维走法可以交替执行，互不越界，所以下界可达）。
//    2) 一维问题按 L 分三段，全部 O(1)：
//         L <= 2t        窄棋盘：两区往返漂移
//         L >= 2t+k      宽棋盘：边界无关，等价无边界
//         2t < L < 2t+k  中间区：二层"攀爬 + 换层"模型
//
//  复杂度：O(1) 时间、O(1) 空间。
//  验证：verify_1e9.cpp 用并查集区间 BFS 对拍约 590 万组，全部一致。
// ============================================================================
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static const ll INF = (ll)4e18;

// ceil(a/b)，要求 b > 0
static inline ll ceilDiv(ll a, ll b) {
    return a >= 0 ? (a + b - 1) / b : -((-a) / b);
}

// ============================================================================
//  一、窄棋盘：L <= 2t
//
//  记 M = L - t，棋盘分成
//      左区 [1, M]、右区 [t+1, L]、死区 (M, t]（可能为空）
//  把右区位置 p 映射成 p - t，两个区都变成 [1, M]。
//  一次移动必然在两区之间切换，映射坐标每两步最多变化 q = min(k, M-1)。
//  于是：同区 2*ceil(|u-v|/q)，异区 2*d+1（先跨区再调整）。
// ============================================================================
static ll solveNarrow(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;          // 一步都走不了
    ll M = L - t;
    ll q = min(k, M - 1);

    // side: 0 = 左区（映射坐标 u = p），1 = 右区（u = p - t），-1 = 死区
    auto getSide = [&](ll p, ll &u) -> int {
        if (p <= M) { u = p; return 0; }
        if (p >= t + 1) { u = p - t; return 1; }
        return -1;
    };

    ll ua, ub;
    int sa = getSide(a, ua), sb = getSide(b, ub);
    if (sa < 0 || sb < 0) return INF;   // 死区里的格子永远不可达

    if (sa == sb) {                     // 同区：来回往返
        if (ua == ub) return 0;
        if (q == 0) return INF;
        return 2 * ceilDiv(llabs(ua - ub), q);
    }
    if (q == 0) return ua == ub ? 1 : INF;  // 只能一步跨区

    ll d = 0;                           // 跨区之后还要几组"两步"
    if (sa == 0) {                      // 左 -> 右
        if (ub > ua) d = max(d, ceilDiv(ub - ua, q) - 1);
        else         d = max(d, ceilDiv(ua - ub, q));
    } else {                            // 右 -> 左
        if (ua > ub) d = max(d, ceilDiv(ua - ub, q) - 1);
        else         d = max(d, ceilDiv(ub - ua, q));
    }
    return 2 * max(d, 0LL) + 1;
}

// ============================================================================
//  二、宽棋盘：L >= 2t+k
//
//  此时棋盘边界不影响最优解，等价于无边界。
//  设向右比向左多 m 步、往返对 q 组，总步数 m + 2q，净位移可覆盖
//      m*t - q*k <= D <= m*(t+k) + q*k
//  固定 m 时最少 q = max(0, ceil((m*t-D)/k), ceil((D-m*(t+k))/k))。
//  目标函数 m + 2q(m) 分段线性，只需检查 D/t、D/(t+k) 附近 O(1) 个 m。
// ============================================================================
static ll solveWide(ll D, ll t, ll k) {
    D = llabs(D);
    if (D == 0) return 0;
    if (k == 0) return D % t == 0 ? D / t : INF;

    ll cand[6];
    int n = 0;
    cand[n++] = 0;
    cand[n++] = D / t; cand[n++] = D / t + 1;
    ll w = ceilDiv(D, t + k);
    cand[n++] = w - 1; cand[n++] = w; cand[n++] = w + 1;

    ll ans = INF;
    for (int i = 0; i < n; i++) {
        ll m = cand[i];
        if (m < 0) continue;
        ll q = max(0LL, max(ceilDiv(m * t - D, k), ceilDiv(D - m * (t + k), k)));
        ans = min(ans, m + 2 * q);
    }
    return ans;
}

// ============================================================================
//  三、中间区：2t < L < 2t+k
//
//  记 c = L - t，s = L - 2t（中间区里 1 <= s <= k-1，且 c = t + s）。
//  因为 U = t+k > L-t = c，边界总是"直接截到棋盘端头"：
//      上跳（p <= c）:  p -> [p+t, min(L, p+t+k)]
//      下跳（p >= t+1）: p -> [max(1, p-t-k), p-t]
//
//  两层坐标（都落在 [1, c]）：
//      L0: 物理位置 u        （能上跳）
//      L1: 物理位置 v + t    （能下跳）
//      上跳 L0 u -> L1 [u, min(c,u+k)]，下跳 L1 v -> L0 [max(1,v-k), v]
//
//  ★ 攀爬引理：连续"上跳+下跳" = u -> [max(1,u-k), min(c,u+k)]，所以
//      偶数步从 u 到 u' 的最少步数 = 2*ceil(|u-u'|/k)。
//
//  ★ 换层：物理位置 p ∈ [t+1, c] = [t+1, t+s] 同时属于两层
//      （L0 u == L1 (u-t)），换层不要步数。换层 + 一次走法只要 1 步
//      就能把坐标推 t，比爬 2*ceil(t/k) 步快得多——这是中间区比窄棋盘
//      多出来的唯一能力。
//
//  ★ 结构引理（已用约 590 万组对拍确认）：最优走法可写成
//      「一次攀爬 → 至多一次换层 → 一次攀爬」，
//      换层放在开头 / 结尾 / 中间都被下面四种朝向覆盖。
// ============================================================================
static inline ll climb(ll x, ll y, ll k) {          // 攀爬距离
    if (x == y) return 0;
    return 2 * ceilDiv(llabs(x - y), k);
}

// 严格交替（不换层）、第一步必须向上；要求起点 a <= c
static ll altUp(ll L, ll a, ll b, ll t, ll k) {
    ll c = L - t;
    if (a > c) return INF;
    ll best = INF;

    // 偶数步：终点是 L0 位置 b（b <= c）
    if (b <= c) best = min(best, a == b ? 0 : 2 * ceilDiv(llabs(a - b), k));

    // 奇数步：终点是 L1 位置 b（b >= t+1），最后一步从 L0 x 上跳而来
    if (b >= t + 1) {
        ll lo = max(1LL, b - t - k), hi = min(c, b - t);
        if (lo <= hi) {
            ll j;
            if (a < lo) j = ceilDiv(lo - a, k);
            else if (a > hi) j = ceilDiv(a - hi, k);
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

// 把 [lo,hi] 中模 k 余 r 的最小 / 最大代表加入候选
static void addResidue(Cand &out, ll lo, ll hi, ll r, ll k) {
    if (lo > hi || k <= 0) return;
    ll rr = ((r % k) + k) % k;
    ll first = lo + (((rr - lo) % k) + k) % k;
    if (first <= hi) out.push(first);
    ll last = hi - ((((hi % k) - rr) % k + k) % k);
    if (last >= lo) out.push(last);
}

// 起点在 L0（a <= c）时的全部候选走法：纯交替，或在重叠区换层一次
static ll dirSolve(ll L, ll a, ll b, ll t, ll k) {
    ll c = L - t, s = L - 2 * t;
    if (a > c) return INF;
    ll best = altUp(L, a, b, t, k);

    const ll baseY[9] = {a - t, b, b - t, b - t - k, b + k, a, a + t, 1, s};
    const ll baseZ[9] = {a + t, b, b - t, b - t - k, b + k, a, a - t, t + 1, t + s};

    // ---- 换层向下：L0 u ∈ [t+1,t+s] --下跳--> L0 y ∈ [1,s]
    {
        Cand ys;
        for (int i = 0; i < 9; i++)
            for (ll d = -1; d <= 1; d++) ys.push(clampv(baseY[i] + d, 1, s));
        for (int i = 0; i < 9; i++) addResidue(ys, 1, s, baseY[i], k);

        for (int i = 0; i < ys.n; i++) {
            ll y = ys.v[i];
            if (y < 1 || y > s) continue;
            ll lo = y + t, hi = min(t + s, y + t + k);   // 换层前的 L0 坐标 u
            if (lo > hi) continue;
            ll v = altUp(L, y, b, t, k);
            if (v >= INF) continue;
            Cand us;
            us.push(clampv(a, lo, hi)); us.push(lo); us.push(hi);
            addResidue(us, lo, hi, a, k);
            addResidue(us, lo, hi, b, k);
            addResidue(us, lo, hi, b - t, k);
            for (int j = 0; j < us.n; j++)
                best = min(best, climb(a, us.v[j], k) + 1 + v);
        }
    }
    // ---- 换层向上：L0 u ∈ [1,s] --上跳+换层--> L0 z ∈ [t+1,t+s]
    {
        Cand zs;
        for (int i = 0; i < 9; i++)
            for (ll d = -1; d <= 1; d++) zs.push(clampv(baseZ[i] + d, t + 1, t + s));
        for (int i = 0; i < 9; i++) addResidue(zs, t + 1, t + s, baseZ[i], k);

        for (int i = 0; i < zs.n; i++) {
            ll z = zs.v[i];
            if (z < t + 1 || z > t + s) continue;
            ll lo = max(1LL, z - t - k), hi = min(s, z - t);
            if (lo > hi) continue;
            ll v = altUp(L, z, b, t, k);
            if (v >= INF) continue;
            Cand us;
            us.push(clampv(a, lo, hi)); us.push(lo); us.push(hi);
            addResidue(us, lo, hi, a, k);
            addResidue(us, lo, hi, z - t, k);
            for (int j = 0; j < us.n; j++)
                best = min(best, climb(a, us.v[j], k) + 1 + v);
        }
    }
    return best;
}

static ll solveMiddle(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    // 四种朝向：正向 / 反向 / 镜像正向 / 镜像反向
    ll m1 = L + 1 - a, m2 = L + 1 - b;
    ll best = INF;
    best = min(best, dirSolve(L, a, b, t, k));
    best = min(best, dirSolve(L, b, a, t, k));
    best = min(best, dirSolve(L, m1, m2, t, k));
    best = min(best, dirSolve(L, m2, m1, t, k));
    return best;
}

// ============================================================================
//  一维总入口
// ============================================================================
static ll solve1D(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;                  // 一步都走不了
    if (L <= 2 * t) return solveNarrow(L, a, b, t, k);
    if (L >= 2 * t + k) return solveWide(b - a, t, k);
    return solveMiddle(L, a, b, t, k);
}

// ============================================================================
//  主程序
// ============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m;
    cin >> n >> m;
    ll x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    ll s, k, t;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {                 // A = B
        cout << "YES 0\n";
        return 0;
    }

    ll fx = solve1D(n, x1, x2, t, k);           // 行方向
    ll fy = solve1D(m, y1, y2, t, k);           // 列方向

    if (fx >= INF / 2 || fy >= INF / 2 || fx + fy > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << fx + fy << '\n';
    }
    return 0;
}
