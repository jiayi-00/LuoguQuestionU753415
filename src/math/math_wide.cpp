#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (ll)4e18;

/*
 * 加强版（宽棋盘）：
 * 保证 min(n,m) >= 2t+k。
 *
 * 结论：此时棋盘边界不影响一维最短步数，
 * 可以直接用无边界模型。
 */

ll ceilDiv(ll a, ll b) {  // b > 0
    if (a >= 0) return (a + b - 1) / b;
    return -((-a) / b);
}

/*
 * 一维无边界问题：
 * 从 0 到 D，每步长度在 [t,t+k]，方向任意，最少步数。
 *
 * 固定“向右比向左多走 m 步”，再枚举 q 组往返对。
 * 每组往返对最多把净位移调整 k。
 */
ll solveUnbounded(ll D, ll t, ll k) {
    D = llabs(D);
    if (D == 0) return 0;

    if (k == 0) {
        return (D % t == 0) ? D / t : INF;
    }

    vector<ll> cand;
    cand.push_back(0);

    ll a = D / t;
    ll b = D / (t + k);
    for (ll base : {a, b}) {
        for (ll m = base - 2; m <= base + 2; ++m) {
            if (m >= 0) cand.push_back(m);
        }
    }

    ll ans = INF;
    for (ll m : cand) {
        // q 组往返，组内正负步长都在 [t,t+k]
        ll q = 0;
        q = max(q, ceilDiv(m * t - D, k));
        q = max(q, ceilDiv(D - m * (t + k), k));
        q = max(q, 0LL);

        ans = min(ans, m + 2 * q);
    }
    return ans;
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

    // 本文件的前提：min(n,m) >= 2t+k
    ll ans = solveUnbounded(x2 - x1, t, k)
           + solveUnbounded(y2 - y1, t, k);

    if (ans >= INF / 2 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
