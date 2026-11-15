# 数据说明

三个版本的测试数据分别放在：

```text
data/basic/           基础版，30 组
data/enhanced_1e7/    加强版，10 组
data/1e9/             数学版，5 组
```

每组数据都是：

```text
XX.in
XX.out
```

输出格式：

```text
YES ans
NO
```

每个目录下还有：

- `manifest.tsv`：测试点说明与预期答案；
- `README.txt`：该版本的数据范围、构造说明和标程路径。

## 快速检查

在仓库根目录编译后，可以用：

```powershell
Get-Content data/basic/01.in | .\build\basic.exe
Get-Content data/enhanced_1e7/07.in | .\build\enhanced_1e7.exe
Get-Content data/1e9/01.in | .\build\math_1e9.exe
```

数据目录中只有文本文件，不包含可执行文件或压缩包。
