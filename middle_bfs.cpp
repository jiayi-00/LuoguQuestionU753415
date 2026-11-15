#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int INF = 1e9;

/*
 * 一维区间 BFS：
 * L 个格子，从 a 到 b，每步走 [t, t+k]，不能越界。
 * 用并查集维护“下一个未访问位置”，复杂度 O(L α(L))。
 *
 * 适合 L <= 2e6 左右的数据，可作为中间区的精确对拍/数据生成器。
 */
int solve1D(int L, int a, int b, int t, int k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;

    int U = min(t + k, L - 1);
    vector<int> nxt(L + 2);
    for (int i = 1; i <= L + 1; ++i) nxt[i] = i;

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

    vector<int> dist(L + 1, -1);
    queue<int> q;
    dist[a] = 0;
    erase(a);
    q.push(a);

    auto visitRange = [&](int l, int r, int nd) {
        l = max(l, 1);
        r = min(r, L);
        if (l > r) return;
        for (int y = find(l); y <= r; y = find(y)) {
            dist[y] = nd;
            erase(y);
            q.push(y);
        }
    };

    while (!q.empty()) {
        int x = q.front();
        q.pop();
        int nd = dist[x] + 1;

        // [x-U, x-t]
        visitRange(x - U, x - t, nd);
        // [x+t, x+U]
        visitRange(x + t, x + U, nd);
    }

    return dist[b] == -1 ? INF : dist[b];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, x1, y1, x2, y2;
    ll s, k, t;
    cin >> n >> m;
    cin >> x1 >> y1 >> x2 >> y2;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    // 限制：n,m 不能超过 BFS 能承受的范围
    const int LIMIT = 10000000;
    if (max(n, m) > LIMIT) {
        // 这个文件只负责中小范围；大范围请用 math_wide / math_narrow
        return 0;
    }

    int ans = solve1D(n, x1, x2, (int)t, (int)k)
            + solve1D(m, y1, y2, (int)t, (int)k);

    if (ans >= INF / 2 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
