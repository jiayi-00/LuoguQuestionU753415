#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 4e18;
const int MAXN = 10000005;

int T, K;  // t, k
int nextR[MAXN], nextL[MAXN], que[MAXN];

int findR(int x) {
    int root = x;
    while (nextR[root] != root) root = nextR[root];
    while (nextR[x] != root) { int nx = nextR[x]; nextR[x] = root; x = nx; }
    return root;
}
int findL(int x) {
    int root = x;
    while (nextL[root] != root) root = nextL[root];
    while (nextL[x] != root) { int nx = nextL[x]; nextL[x] = root; x = nx; }
    return root;
}

// 一维 BFS：位置 1..N，起点 s，终点 e，最多 maxSteps 步
ll solve1D(int N, int s, int e, ll maxSteps) {
    if (s == e) return 0;
    if (T >= N) return INF;   // 一步都走不了

    for (int i = 0; i <= N + 1; i++) { nextR[i] = i; nextL[i] = i; }

    auto erase = [&](int x) {
        nextR[x] = findR(x + 1);
        nextL[x] = findL(x - 1);
    };

    int head = 0, tail = 0;
    que[tail++] = s;
    erase(s);

    ll steps = 0;
    while (head < tail) {
        int layerEnd = tail;
        steps++;
        if (steps > maxSteps) return INF;
        while (head < layerEnd) {
            int pos = que[head++];

            // 正方向 [pos+t, pos+t+k]
            {
                ll l = (ll)pos + T, r = min((ll)N, (ll)pos + T + K);
                if (l <= r) {
                    int cur = findR((int)l);
                    while (cur <= r) {
                        if (cur == e) return steps;
                        que[tail++] = cur;
                        erase(cur);
                        cur = findR(cur + 1);
                    }
                }
            }
            // 负方向 [pos-t-k, pos-t]
            {
                ll l = max(1LL, (ll)pos - T - K), r = (ll)pos - T;
                if (l <= r) {
                    int cur = findL((int)r);
                    while (cur >= l) {
                        if (cur == e) return steps;
                        que[tail++] = cur;
                        erase(cur);
                        cur = findL(cur - 1);
                    }
                }
            }
        }
    }
    return INF;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, x1, y1, x2, y2;
    ll s;
    cin >> n >> m;
    cin >> x1 >> y1 >> x2 >> y2;
    cin >> s >> K >> T;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    ll fx = solve1D(n, x1, x2, s);
    ll fy = solve1D(m, y1, y2, s);

    if (fx == INF || fy == INF || fx + fy > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << fx + fy << '\n';
    }
    return 0;
}
