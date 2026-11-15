# 工具说明

## 数据生成

```powershell
python tools/gen_basic.py
python tools/gen_enhanced_1e7.py
```

两个脚本都应在仓库根目录运行。`gen_enhanced_1e7.py` 默认调用：

```text
build/enhanced_1e7.exe
```

请先按 README 编译。

## 验证

- `tools/verify_wide.cpp`：验证 `L >= 2t+k` 时无边界公式正确；
- `tools/verify_wide2.cpp`：另一个小范围宽棋盘对拍；
- `src/1e9/verify_1e9.cpp`：并查集区间 BFS 对拍 1e9 一维求解器；
- `src/1e9/verify_single.cpp`：验证单文件版 `math_1e9_single.cpp`。

## dev/

研究阶段的实验代码，包括：

- `proto_middle.py` / `proto_left.py` / `proto_full.py`：固定 `p,q` 区间的探索；
- `search_intermediate.cpp` / `search_pen.cpp`：搜索边界惩罚和反例；
- `show_path.cpp`：输出最短路径；
- `find_diff2_u1.cpp`：早期差分测试。

这些文件不是最终标程，保留用于复现研究过程。
