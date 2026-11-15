#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int INF = 1e9;

/*
 * 涓€缁村尯闂?BFS锛? * L 涓牸瀛愶紝浠?a 鍒?b锛屾瘡姝ヨ蛋 [t, t+k]锛屼笉鑳借秺鐣屻€? * 鐢ㄥ苟鏌ラ泦缁存姢鈥滀笅涓€涓湭璁块棶浣嶇疆鈥濓紝澶嶆潅搴?O(L 伪(L))銆? *
 * 閫傚悎 L <= 2e6 宸﹀彸鐨勬暟鎹紝鍙綔涓轰腑闂村尯鐨勭簿纭鎷?鏁版嵁鐢熸垚鍣ㄣ€? */
int solve1D(int L, int a, int b, int t, int k) {
    if (a == b) return 0;
    if (t > L - 1) return INF;

    int U = min(t + k, L - 1);
    vector<int> nxt(L + 2);
    for (int i = 1; i <= L + 1; ++i) nxt[i] = i;

    function<int(int)> find = [&](int x) -> int {
        if (nxt[x] == x) return x;
        return nxt[x] = find(nxt[x]);
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

    // 闄愬埗锛歯,m 涓嶈兘瓒呰繃 BFS 鑳芥壙鍙楃殑鑼冨洿
    const int LIMIT = 10000000;
    if (max(n, m) > LIMIT) {
        // 杩欎釜鏂囦欢鍙礋璐ｄ腑灏忚寖鍥达紱澶ц寖鍥磋鐢?math_wide / math_narrow
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

