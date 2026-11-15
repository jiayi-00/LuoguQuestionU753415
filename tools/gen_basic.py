# -*- coding: utf-8 -*-
"""生成《棋盘上的虫子》基础版数据。

基础版约束：
  1 <= n,m <= 100
  1 <= s <= 100
  1 <= t <= 100
  0 <= k <= 100
"""

from collections import deque
from pathlib import Path

OUT_DIR = Path("data/basic")
OUT_DIR.mkdir(exist_ok=True)

# (n, m, x1, y1, x2, y2, s, k, t, note)
CASES = [
    # 1~10：手工边界与特殊情况
    (1, 1, 1, 1, 1, 1, 1, 0, 1, "1x1，A=B"),
    (1, 5, 1, 1, 1, 5, 100, 0, 1, "一维直线，直接走到"),
    (5, 1, 1, 1, 5, 1, 100, 1, 1, "一维竖直，步长 1~2"),
    (1, 5, 1, 1, 1, 5, 1, 1, 2, "s 不够，走不到"),
    (1, 3, 1, 1, 1, 2, 100, 0, 2, "t 太大，只能 1->3，到不了 2"),
    (1, 4, 1, 1, 1, 4, 100, 0, 2, "距离 3 不是 2 的倍数"),
    (3, 3, 1, 1, 3, 3, 2, 0, 2, "行、列各一步"),
    (10, 10, 1, 1, 10, 10, 17, 0, 1, "曼哈顿距离 18，s=17"),
    (10, 10, 1, 1, 10, 10, 18, 0, 1, "曼哈顿距离 18，s=18"),
    (100, 100, 1, 1, 100, 100, 1, 0, 100, "t 大于棋盘最大位移，无路可走"),

    # 11~20：k=0、贴边、窄棋盘死区等
    (100, 100, 1, 1, 100, 100, 2, 0, 99, "行、列各一步，t=99"),
    (10, 10, 1, 1, 6, 6, 10, 0, 2, "位移 5 不是 2 的倍数"),
    (10, 10, 1, 1, 7, 7, 6, 0, 2, "行、列各 3 步"),
    (10, 10, 1, 1, 1, 7, 3, 0, 2, "只有一维需要走"),
    (10, 10, 1, 1, 10, 10, 6, 1, 1, "每维至少 5 步，总步数不够"),
    (10, 10, 1, 1, 10, 10, 10, 1, 1, "每维 5 步，恰好 10 步"),
    (10, 10, 1, 1, 10, 10, 4, 2, 3, "每维 9=4+5，共 4 步"),
    (1, 7, 1, 1, 1, 4, 100, 1, 4, "窄棋盘中间死区，目标不可达"),
    (1, 7, 1, 1, 1, 3, 100, 1, 4, "窄棋盘左区可达"),
    (100, 100, 1, 1, 1, 1, 100, 100, 100, "A=B，且参数很大"),

    # 21~30：随机/综合
    (57, 86, 57, 45, 1, 1, 15, 7, 2, "随机：s 恰好等于答案"),
    (44, 84, 23, 1, 1, 40, 38, 9, 5, "随机：贴边"),
    (52, 20, 24, 10, 1, 20, 78, 7, 9, "随机：贴右边"),
    (48, 48, 48, 1, 1, 48, 2, 5, 3, "随机：s 太小"),
    (13, 94, 4, 1, 7, 1, 12, 4, 9, "随机：短距离但 t 很大"),
    (92, 12, 92, 12, 20, 3, 44, 7, 9, "随机：大棋盘，小步长"),
    (64, 44, 1, 35, 1, 44, 90, 1, 5, "随机：同一列"),
    (35, 62, 10, 27, 35, 62, 15, 9, 7, "随机：终点贴角"),
    (23, 62, 1, 62, 1, 7, 35, 8, 8, "随机：同一列，纵向走"),
    (17, 62, 1, 1, 9, 1, 36, 10, 5, "随机：同一行，一步到达"),
]


def solve(n, m, x1, y1, x2, y2, s, k, t):
    if (x1, y1) == (x2, y2):
        return 0
    dist = [[-1] * (m + 1) for _ in range(n + 1)]
    dist[x1][y1] = 0
    q = deque([(x1, y1)])
    max_d = min(t + k, max(n, m))
    while q:
        x, y = q.popleft()
        if dist[x][y] >= s:
            continue
        for d in range(t, max_d + 1):
            for nx, ny in ((x - d, y), (x + d, y), (x, y - d), (x, y + d)):
                if 1 <= nx <= n and 1 <= ny <= m and dist[nx][ny] == -1:
                    dist[nx][ny] = dist[x][y] + 1
                    q.append((nx, ny))
    return dist[x2][y2]


def main():
    lines = []
    for i, (n, m, x1, y1, x2, y2, s, k, t, note) in enumerate(CASES, 1):
        assert 1 <= n <= 100 and 1 <= m <= 100
        assert 1 <= s <= 100 and 1 <= t <= 100 and 0 <= k <= 100
        assert 1 <= x1 <= n and 1 <= y1 <= m
        assert 1 <= x2 <= n and 1 <= y2 <= m

        ans = solve(n, m, x1, y1, x2, y2, s, k, t)
        if ans == -1 or ans > s:
            out = "NO"
            shown = "NO"
        else:
            out = f"YES {ans}"
            shown = out

        inp = f"{n} {m}\n{x1} {y1} {x2} {y2}\n{s} {k} {t}\n"
        (OUT_DIR / f"{i:02d}.in").write_text(inp, encoding="utf-8")
        (OUT_DIR / f"{i:02d}.out").write_text(out + "\n", encoding="utf-8")
        lines.append(f"{i:02d}\t{note}\t{shown}")

    (OUT_DIR / "manifest.tsv").write_text(
        "id\tnote\tanswer\n" + "\n".join(lines) + "\n", encoding="utf-8"
    )
    print(f"generated {len(CASES)} cases in {OUT_DIR.resolve()}")


if __name__ == "__main__":
    main()
