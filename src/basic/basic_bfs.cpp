#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    long long s, k, t;
    cin >> s >> k >> t;

    if (x1 == x2 && y1 == y2) {
        cout << "YES 0\n";
        return 0;
    }

    vector<vector<int>> dist(n + 1, vector<int>(m + 1, -1));
    queue<pair<int, int>> q;
    dist[x1][y1] = 0;
    q.push({x1, y1});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        int cur = dist[x][y];
        if (cur >= s) continue;

        long long maxD = min<long long>(t + k, max(n, m));
        for (long long dd = t; dd <= maxD; ++dd) {
            int d = (int)dd;
            // up / down
            if (x - d >= 1 && dist[x - d][y] == -1) {
                dist[x - d][y] = cur + 1;
                q.push({x - d, y});
            }
            if (x + d <= n && dist[x + d][y] == -1) {
                dist[x + d][y] = cur + 1;
                q.push({x + d, y});
            }
            // left / right
            if (y - d >= 1 && dist[x][y - d] == -1) {
                dist[x][y - d] = cur + 1;
                q.push({x, y - d});
            }
            if (y + d <= m && dist[x][y + d] == -1) {
                dist[x][y + d] = cur + 1;
                q.push({x, y + d});
            }
        }
    }

    long long ans = dist[x2][y2];
    if (ans == -1 || ans > s) {
        cout << "NO\n";
    } else {
        cout << "YES " << ans << '\n';
    }
    return 0;
}
