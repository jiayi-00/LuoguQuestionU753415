// 《棋盘上的虫子》1e9 版本标程：O(1) 一维公式（窄 / 宽 / 中间区），二维拆成两维相加
// 编译：g++ -O2 -o math_1e9 math_1e9.cpp
#include "solve1e9.hpp"
#include <cstdio>

int main() {
    ll n, m, x1, y1, x2, y2, s, k, t;
    if (scanf("%lld %lld", &n, &m) != 2) return 0;
    if (scanf("%lld %lld %lld %lld", &x1, &y1, &x2, &y2) != 4) return 0;
    if (scanf("%lld %lld %lld", &s, &k, &t) != 3) return 0;

    if (x1 == x2 && y1 == y2) { puts("YES 0"); return 0; }

    ll fx = one_d::solve(n, x1, x2, t, k);
    ll fy = one_d::solve(m, y1, y2, t, k);

    if (fx >= INF1 / 2 || fy >= INF1 / 2 || fx + fy > s) {
        puts("NO");
    } else {
        printf("YES %lld\n", fx + fy);
    }
    return 0;
}
