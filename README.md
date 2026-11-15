# U753357 棋盘上的虫子

一道棋盘上的最少步数题，包含三个逐步加强的版本：

| 版本 | 数据范围 | 标程 | 复杂度 |
|---|---|---|---|
| 基础版 | `n,m ≤ 100`，`s ≤ 100`，`t,k ≤ 100` | `src/basic/basic_bfs.cpp` | `O(nm(k+1))` |
| 加强版 1e7 | `n,m ≤ 1e7`，`s ≤ 2e7`，`t,k ≤ 1e9` | `src/enhanced_1e7/enhanced_1e7.cpp` | `O((n+m) α)` |
| 数学版 1e9 | `n,m,s,t,k ≤ 1e9` | `src/1e9/math_1e9_single.cpp` | `O(1)` |

三个版本共用题面，区别只在数据范围。

## 核心观察

行、列两个维度可以拆开：

```text
answer = f(n, x1, x2) + f(m, y1, y2)
```

其中 `f(L,a,b)` 是一维问题：

> 数轴 `1..L`，从 `a` 到 `b`，每步走 `d ∈ [t,t+k]`，不能越界，求最少步数。

任意二维走法投影到行、列上分别是一维合法走法，所以总步数不少于两维之和；两个一维走法可以任意交错执行，所以下界可以达到。

## 目录

```text
src/
  basic/                 基础版二维 BFS
  enhanced_1e7/          1e7 版的一维区间 BFS
  1e9/                   全范围 O(1) 数学标程与对拍
  math/                  窄棋盘 / 宽棋盘 / 小范围数学版本
  experimental/          双向区间 BFS、1D 区间 BFS、历史实现
data/
  basic/                 30 组基础版数据
  enhanced_1e7/          10 组加强版数据
  1e9/                   5 组 1e9 数学版数据
docs/                    版本设计、分段研究和 1e9 算法说明
tools/                   数据生成器、对拍程序和实验脚本
```

## 编译

在仓库根目录执行：

```powershell
mkdir build -Force

g++ -std=c++17 -O2 src/basic/basic_bfs.cpp -o build/basic.exe
g++ -std=c++17 -O2 src/enhanced_1e7/enhanced_1e7.cpp -o build/enhanced_1e7.exe
g++ -std=c++17 -O2 src/1e9/math_1e9_single.cpp -o build/math_1e9.exe
g++ -std=c++17 -O2 src/1e9/math_1e9.cpp -o build/math_1e9_split.exe
```

运行：

```powershell
Get-Content data/basic/01.in | .\build\basic.exe
Get-Content data/enhanced_1e7/07.in | .\build\enhanced_1e7.exe
Get-Content data/1e9/01.in | .\build\math_1e9.exe
```

## 数据

- 基础版：`data/basic/`，30 组，含 `.in/.out`、`manifest.tsv` 和说明；
- 加强版 1e7：`data/enhanced_1e7/`，10 组，三个子任务档位；
- 数学版 1e9：`data/1e9/`，5 组，覆盖窄、宽、中间区和不可达情况。

生成器：

```powershell
python tools/gen_basic.py
python tools/gen_enhanced_1e7.py
```

`gen_enhanced_1e7.py` 默认调用 `build/enhanced_1e7.exe`，请先编译。

## 文档

- [版本设计](docs/01-版本设计.md)
- [加强版 1e7 研究](docs/02-加强版-1e7.md)
- [中间区研究](docs/03-中间区研究.md)
- [1e9 算法研究](docs/04-1e9-算法.md)

`docs/03` 是历史研究记录，最终方案以 `docs/04` 和 `src/1e9/` 为准。

## 验证状态

- 基础版：30 组数据全部通过；
- 加强版：10 组数据全部通过；
- 1e9 版：5 组数据全部通过，且已用 `src/1e9/verify_1e9.cpp` 的并查集区间 BFS 做过小范围对拍。

`src/experimental/` 中保留了一些研究阶段的实现，例如双向区间 BFS。它们不一定适合作为最终标程，主要用于复现研究过程。

## 说明

基础版与加强版分别为 `U753357` `U753415`
