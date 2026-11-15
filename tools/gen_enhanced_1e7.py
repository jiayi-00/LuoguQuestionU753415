# -*- coding: utf-8 -*-
"""生成 n,m <= 1e7 的加强版数据（10 个测试点）。

约束：
  1 <= n,m <= 1e7
  1 <= s <= 2e7
  1 <= t <= 1e9
  0 <= k <= 1e9
"""

import subprocess
from pathlib import Path

OUT = Path("data/enhanced_1e7")
OUT.mkdir(exist_ok=True)

# (n, m, x1, y1, x2, y2, s, k, t, note)
CASES = [
    # 1~3：n,m <= 100, s <= 100, t,k <= 100
    (1, 1, 1, 1, 1, 1, 1, 0, 1, "A=B，最小棋盘"),
    (10, 10, 1, 1, 6, 6, 5, 1, 2, "样例 1"),
    (1, 100, 1, 1, 1, 100, 100, 50, 60, "一维，一步跨到"),

    # 4~6：n,m <= 1e5, s <= 2e5, t,k <= 1e9
    (100000, 100000, 1, 1, 100000, 100000, 200000, 0, 1,
     "最大曼哈顿距离，s 刚好够"),
    (1, 100000, 1, 1, 1, 20000, 2, 50000, 30000,
     "中间区，两跳返回"),
    (100000, 100000, 50000, 50000, 1, 1, 200000, 1000000000, 1000000000,
     "t 大于棋盘最大位移，无路可走"),

    # 7~10：n,m <= 1e7, s <= 2e7, t,k <= 1e9
    (10000000, 10000000, 1, 1, 10000000, 10000000, 20000000, 0, 1,
     "最大棋盘，t=1, k=0"),
    (10000000, 10000000, 1, 1, 5000001, 5000001, 2, 0, 5000000,
     "大棋盘，一步 5e6"),
    (10000000, 10000000, 1, 1, 2000001, 2000001, 3, 5000000, 3000000,
     "大棋盘中间区，s 比答案少 1"),
    (10000000, 10000000, 1, 1, 9999999, 9999999, 2, 1000000000, 1,
     "k 极大，一步跨到"),
]


def main():
    solver = Path("build/enhanced_1e7.exe")
    if not solver.exists():
        solver = Path("build/enhanced_1e7")
    if not solver.exists():
        raise SystemExit("请先编译: build/enhanced_1e7.exe")

    manifest = ["id\tnote\tanswer"]
    for i, (n, m, x1, y1, x2, y2, s, k, t, note) in enumerate(CASES, 1):
        assert 1 <= n <= 10**7 and 1 <= m <= 10**7
        assert 1 <= s <= 2 * 10**7
        assert 1 <= t <= 10**9 and 0 <= k <= 10**9
        assert 1 <= x1 <= n and 1 <= y1 <= m
        assert 1 <= x2 <= n and 1 <= y2 <= m

        inp = f"{n} {m}\n{x1} {y1} {x2} {y2}\n{s} {k} {t}\n"
        (OUT / f"{i:02d}.in").write_text(inp, encoding="utf-8")

        res = subprocess.run(
            [str(solver)],
            input=inp,
            text=True,
            capture_output=True,
            check=True,
        )
        out = res.stdout.strip()
        if not out:
            raise RuntimeError(f"case {i}: empty output")
        (OUT / f"{i:02d}.out").write_text(out + "\n", encoding="utf-8")
        manifest.append(f"{i:02d}\t{note}\t{out}")

    (OUT / "manifest.tsv").write_text("\n".join(manifest) + "\n", encoding="utf-8")
    print(f"generated {len(CASES)} cases")


if __name__ == "__main__":
    main()
