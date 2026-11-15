#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int INF = 1e9;

/*
 * 一维问题：
 * 1..L 上从 a 到 b，每步走 [t,t+k]，不能越界，求最少步数。
 *
 * 区间 BFS + 并查集维护“下一个未访问位置”。
 * 每个位置只会入队一次，复杂度 O(L α(L))。
 */
int solve1D(int L, int a, int b, ll t, ll k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;

    int U = (int)min(t + k, (ll)L - 1);

    // nxt[i]：从 i 开始，下一个未访问的位置
    vector<int> nxt(L + 2);
    iota(nxt.begin(), nxt.end(), 0);

    vector<int> dist(L + 1, -1);
    vector<int> q;
    q.reserve(L);

    auto find = [&](int x) -> int {
        int r = x;
        while (nxt[r] != r) r = nxt[r];
        while (nxt[x] != x) {
            int y = nxt[x];
            nxt[x] = r;
            x = y;
        }
        return r;
    };

    auto erase = [&](int x) {
        nxt[x] = find(x + 1);
    };

    auto cover = [&](int l, int r, int nd) {
        l = max(l, 1);
        r = min(r, L);
        if (l > r) return;
        for (int y = find(l); y <= r; y = find(y)) {
            dist[y] = nd;
            erase(y);
            q.push_back(y);
        }
    };

    dist[a] = 0;
    erase(a);
    q.push_back(a);

    for (size_t head = 0; head < q.size(); ++head) {
        int x = q[head];
        int nd = dist[x] + 1;

        // 向左走 [t,U]
        cover(x - U, x - t, nd);
        // 向右走 [t,U]
        cover(x + t, x + U, nd);
    }

    return dist[b] == -1 ? INF : dist[b];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    int x1, y1, x2, y2;
    ll s, k, t;

    cin >> n >> m;
    cin >> x1 >> y1 >> x2 >> y2;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    ll ans = (ll)solve1D(n, x1, x2, t, k)
           + (ll)solve1D(m, y1, y2, t, k);

    if (ans >= INF / 2 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
