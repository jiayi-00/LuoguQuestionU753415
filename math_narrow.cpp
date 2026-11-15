#include <bits/stdc++.h>
using namespace std;

/*
 * 注意：
 * 本文件只解决“窄棋盘”版本，即必须满足 max(n,m) <= 2t。
 * 基础版数据中存在 max(n,m) > 2t 的情况，不能拿本文件去跑基础版数据。
 * 基础版请使用 basic_bfs.cpp；
 * 若想让数学代码兼容小范围宽棋盘，请使用 math_hybrid_basic.cpp。
 */

using ll = long long;
const ll INF = (ll)4e18;

/*
 * 一维问题：
 * 数轴上有 L 个格子 1..L，从 a 到 b，每步走 d ∈ [t, t+k]，
 * 不能越界，求最少步数。
 *
 * 仅在 L <= 2t 时使用本函数。
 */
ll solve1D(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;  // 一步都走不了

    ll M = L - t;
    ll q = min(k, M - 1);

    auto getSide = [&](ll p) -> pair<int, ll> {
        if (p <= M) return {0, p};          // 左区，映射后为 p
        if (p >= t + 1) return {1, p - t};  // 右区，映射后为 p-t
        return {-1, 0};                     // 中间死区
    };

    auto [sa, u] = getSide(a);
    auto [sb, v] = getSide(b);
    if (sa == -1 || sb == -1) return INF;

    if (sa == sb) {
        if (u == v) return 0;
        if (q == 0) return INF;
        return 2 * ((llabs(u - v) + q - 1) / q);
    }

    if (q == 0) return (u == v ? 1 : INF);

    ll d = 0;
    if (sa == 0) {  // 左区 -> 右区
        if (v > u) d = max(d, (v - u + q - 1) / q - 1);
        else       d = max(d, (u - v + q - 1) / q);
    } else {        // 右区 -> 左区
        if (u > v) d = max(d, (u - v + q - 1) / q - 1);
        else       d = max(d, (v - u + q - 1) / q);
    }
    d = max(d, 0LL);
    return 2 * d + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, m, x1, y1, x2, y2, s, k, t;
    cin >> n >> m;
    cin >> x1 >> y1 >> x2 >> y2;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    // 本版本要求 max(n,m) <= 2t
    ll ans = solve1D(n, x1, x2, t, k) + solve1D(m, y1, y2, t, k);
    if (ans >= INF / 2 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
