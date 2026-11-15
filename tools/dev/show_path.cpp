// 打印某个一维实例的最短路（用于分析失败样例）
#include "solve1e9.hpp"
#include <cstdio>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
    int L = atoi(argv[1]), a = atoi(argv[2]), b = atoi(argv[3]), t = atoi(argv[4]), k = atoi(argv[5]);
    // BFS from b (graph is undirected)
    vector<int> d(L + 1, -1);
    vector<int> q(L);
    int h = 0, tl = 0;
    d[b] = 0; q[tl++] = b;
    long long U = (long long)t + k;
    while (h < tl) {
        int x = q[h++];
        for (int y = max(1LL, (long long)x - U); y <= x - t; y++) if (d[y] < 0) { d[y] = d[x] + 1; q[tl++] = y; }
        for (int y = x + t; y <= min((long long)L, (long long)x + U); y++) if (d[y] < 0) { d[y] = d[x] + 1; q[tl++] = y; }
    }
    printf("dist=%d  formula=%lld\n", d[a], one_d::solve(L, a, b, t, k));
    int x = a;
    vector<int> path;
    while (x != b) {
        path.push_back(x);
        int nx = -1;
        for (int y = max(1LL, (long long)x - U); y <= x - t; y++) if (d[y] == d[x] - 1) { nx = y; break; }
        if (nx < 0) for (int y = x + t; y <= min((long long)L, (long long)x + U); y++) if (d[y] == d[x] - 1) { nx = y; break; }
        if (nx < 0) { printf("broken\n"); return 1; }
        x = nx;
    }
    path.push_back(b);
    printf("steps=%d\n", (int)path.size() - 1);
    // 压缩连续相同的步长
    vector<pair<long long,int>> runs; // (step, position before run)
    for (size_t i = 0; i + 1 < path.size(); i++) {
        long long st = path[i + 1] - path[i];
        if (!runs.empty() && runs.back().first == st) continue;
        runs.push_back({st, path[i]});
    }
    printf("runs=%d\n", (int)runs.size());
    for (auto& r : runs) printf("  step %lld from %lld\n", r.first, r.second);
    return 0;
}
