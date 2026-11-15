#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (ll)4e18;

/*
 * 窄棋盘 O(1) 公式：L <= 2t 时使用。
 */
ll solveNarrow(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;

    ll M = L - t;
    ll q = min(k, M - 1);

    auto getSide = [&](ll p) -> pair<int, ll> {
        if (p <= M) return {0, p};
        if (p >= t + 1) return {1, p - t};
        return {-1, 0};
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
    if (sa == 0) {
        if (v > u) d = max(d, (v - u + q - 1) / q - 1);
        else       d = max(d, (u - v + q - 1) / q);
    } else {
        if (u > v) d = max(d, (u - v + q - 1) / q - 1);
        else       d = max(d, (v - u + q - 1) / q);
    }
    return 2 * max(d, 0LL) + 1;
}

/*
 * 宽棋盘兜底：一维 BFS。
 * 仅适合 L 不太大（例如基础版 L <= 100）的数据。
 */
ll solveWideByBFS(ll L, ll a, ll b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;

    int n = (int)L;
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[(int)a] = 0;
    q.push((int)a);

    ll maxD = min(t + k, L - 1);
    while (!q.empty()) {
        int x = q.front();
        q.pop();

        for (ll dd = t; dd <= maxD; ++dd) {
            int d = (int)dd;
            if (x - d >= 1 && dist[x - d] == -1) {
                dist[x - d] = dist[x] + 1;
                q.push(x - d);
            }
            if (x + d <= n && dist[x + d] == -1) {
                dist[x + d] = dist[x] + 1;
                q.push(x + d);
            }
        }
    }

    return dist[(int)b] == -1 ? INF : dist[(int)b];
}

ll solve1D(ll L, ll a, ll b, ll t, ll k) {
    if (L <= 2 * t) return solveNarrow(L, a, b, t, k);
    return solveWideByBFS(L, a, b, t, k);
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

    ll ans = solve1D(n, x1, x2, t, k) + solve1D(m, y1, y2, t, k);
    if (ans >= INF / 2 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
