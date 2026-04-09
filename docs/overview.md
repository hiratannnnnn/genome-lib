# graph-lib — プロジェクト概要

## このライブラリについて

グラフ理論アルゴリズムをまとめた C ライブラリ。
現在は汎用グラフライブラリとして整備中だが、**将来的にゲノム解析ライブラリへ転用する予定**。

---

## ディレクトリ構成

```
graph-lib/
├── includes/       ヘッダファイル（lib.h, types.h, get_next_line.h）
├── srcs/           ライブラリ本体（.c ファイル）
├── tests/          テストスイート（C 言語）
├── docs/           このドキュメント群
└── Makefile        → dist/lib.a を生成
```

## ビルド方法

```sh
# ライブラリのアーカイブを作成
make            # → dist/lib.a

# テスト
cd tests && make run
```

---

## 主要モジュール一覧

| モジュール | ファイル | 内容 |
|-----------|---------|------|
| 行列操作 | `generate_matrix_int.c`, `generate_matrix_double.c`, `matrix_basic.c` | 行列生成・コピー・乗算・頂点入れ替え |
| グラフ生成 | `random_undigraph.c`, `random_digraph.c`, `random_tree.c`, `random_bigraph.c`, `*_cost.c` | 各種ランダムグラフ生成 |
| グラフ解析 | `graph_analysis.c` | 次数計算・辺数・木判定・奇頂点探索 |
| グラフ変換 | `graph_conversion.c`, `utils_edge_list.c`, `line_graph.c` | 隣接行列 ↔ 隣接リスト変換 |
| データ構造 | `utils_edges.c`, `utils_vertex.c`, `utils_edgenode.c`, `utils_pathnode.c`, `utils_node.c` | Edge / Vertex / EdgeNode / PathNode / Node 操作 |
| ソート | `cmp.c`, `sort_edge_list.c` | 比較関数・各種リストのソート |
| 入出力 | `write_to_file.c`, `read_matrix.c`, `get_next_line.c` | ファイル読み書き |
| ユーティリティ | `utils_basic.c`, `utils_math.c`, `utils_array.c`, `utils_char.c`, `utils_print.c`, `utils_free.c` | 数値・配列・文字列・表示・解放 |
| メモリ管理 | `xmalloc.c`, `memory_dump.c` | 追跡付きアロケータ |
| 文字列 | `ft_split.c`, `ft_strtrim.c` | split / trim |

---

## 設計上の約束事

- 出力は `printf` のみ使用（`puts` / `write` 等は使わない）
- 独自アロケータ（`xmalloc` / `xcalloc` / `xfree` / `xrealloc`）を使い、`mem` / `mem_peak` でメモリ使用量を追跡する
- 標準ライブラリで代替できる関数は持たない（`strdup`, `strlen` 等は `<string.h>` を使う）
- `get_next_line` だけは構文解析の自前実装のために保持

---

## グローバル変数（lib.h で extern 宣言）

| 変数 | 型 | 用途 |
|------|-----|------|
| `mem` | `size_t` | 現在のアロケート合計 |
| `mem_peak` | `size_t` | ピーク使用量 |
| `proc_start` | `clock_t` | 処理開始時刻 |
| `proc_end` | `clock_t` | 処理終了時刻 |

> **注意**: これらは `srcs/` に定義がない。ライブラリを使う実行ファイル側（例: `tests/main.c`）で必ず定義すること。
