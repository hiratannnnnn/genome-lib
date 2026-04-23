# genome-lib

Labarre (2018) のブロック交換ソートアルゴリズム (ApproximateSbpbi) の実装ライブラリです。

## ビルド

```bash
# ライブラリをビルド
make

# sbpbi の全ツールを一括ビルド
make sbpbi
```

## ツール一覧

### check
1つの順列に対してアルゴリズムを実行し、ログと統計情報を出力します。

```bash
make check
make run-check ARGS="5 3 2 4 1"
```

### all_check
サイズ `n` の全順列を網羅し、最大操作数とその順列を出力します（小さい `n` 向け）。

```bash
make all_check
make run-all_check N=7
```

### make_input
サイズ `n` のランダムな順列を生成し、ファイル `input` に書き出します。

```bash
make make_input
make gen N=10
```

### check-input
`make gen` で生成した `input` ファイルの内容を `check` で実行します。

```bash
make check-input
```

### benchmark
ランダムな順列を `trials` 回生成してアルゴリズムを実行し、実行時間と操作数の統計を出力します。

```bash
make benchmark
make run-benchmark N=100 T=1000
```

## 典型的なワークフロー

```bash
# ランダムな順列を1つ生成して確認する
make gen N=8
make check-input

# 小さい n で最悪ケースを全探索する
make run-all_check N=7

# 実行時間を統計的に計測する
make run-benchmark N=100 T=1000
```
