# ロードマップ・今後の方針

## 大方針：ゲノム解析ライブラリへの転用

現在の `graph-lib` はグラフ理論汎用ライブラリだが、将来的に **ゲノム編集・ゲノム解析に特化したライブラリ**へ発展させる予定。

- ライブラリ名（`graph-lib` → ゲノム向け名称）は後で変更する
- モノレポ構成への移行も検討中
- docs はその移行を見据えて整備する

---

## バグ修正（優先度：高）

### #1 gen_rand_tree: n < 3 でクラッシュ
- **原因**: Prufer 符号長 = n-2 が `size_t` にキャストされると n=1 のとき巨大値になりアンダーフロー
- **対処**: `gen_rand_tree` に n >= 3 のガード節を追加する

### #2 gen_rand_bigraph: a ≠ b で heap-buffer-overflow
- **原因**: 実装内で `matrix[j][i] = 1` と書いているが j は b 側のインデックスで a 行の行列に書き込む
- **対処**: 真の二部グラフ（a×b の biadjacency matrix）として実装し直す

### #7 malloc/free と xmalloc/xfree の混在
- メモリ追跡が壊れるため、全ソースで `xmalloc` / `xfree` / `xcalloc` / `xrealloc` に統一する

### #8 xcalloc の第1引数が int（size_t が正しい）
- シグネチャ: `void *xcalloc(int n, size_t size_each)` → `void *xcalloc(size_t n, size_t size_each)`

### #12 NodeType enum に ROUND / SQUARE が混在
- `NodeType` に `NODE_TYPE_EDGE`, `NODE_TYPE_VERTEX` しか使っていないのに `ROUND`, `SQUARE` が残っている
- 削除または別 enum に分離する

### #15 dump_memory_hex の size / offset 引数が int
- `int size, int offset` → `size_t size, size_t offset`

---

## リファクタリング（優先度：中）

### #6 PathNode / EdgeNode / Node の統合
- `PathNode`（頂点のリスト）と `EdgeNode`（辺のリスト）が先に存在し、後から汎用 `Node` を追加した経緯
- 方向性: `Node` の `void *ptr` + `NodeType` で統合できるか検討。ただし型安全性とのトレードオフあり
- ゲノム解析ライブラリへの移行タイミングで設計し直すのがベター

### #9 cmp_double_* の戻り値規約が cmp_int_* と不一致
- `cmp_int_asc(a, b)`: a > b なら +1, a < b なら -1, a == b なら 0
- `cmp_double_asc(a, b)`: 現状は 1 / -1 / 0 ではなく直接差を返している可能性あり
- `qsort` 互換にするか、ライブラリ独自規約に統一する

### #10 リスト長取得関数の命名が分散
- `len_edge_list`, `count_edgenodes`, `count_pathnodes`, `get_node_length` がバラバラ
- `count_X` に統一する

### #11 pop 操作の命名順序が不統一
- `pathnode_pop_first` vs `edgenode_pop_first` vs `pop_first_node`
- `pop_first_X` / `pop_last_X` に統一する

### #13 dbcost_to_adj の命名ミスリード
- `db` は double の略のはずが、引数が `int **cost` になっている
- 実態に合った名前に変更する（または引数の型を直す）

### #14 ft_ プレフィックスの整理
- `ft_split`, `ft_strtrim` は 42 スクール由来の命名
- ゲノムライブラリへ移行する際に `split` / `strtrim`（またはドメイン名プレフィックス）に変える

### #16 need_double_edge はオイラー固有ロジック
- オイラー閉路アルゴリズムのためのヘルパーが `graph_analysis.c` に残っている
- オイラー関連コードを全削除した今、この関数も削除を検討

### #17 ft_split の sep 引数が const でない
- `char **ft_split(char const *str, char *sep)` → `sep` も `const char *` にする

---

## 命名規則・コーディング規約の方針

- 関数名: `動詞_対象_修飾語` の順（例: `sort_list_by_degree`）
- プレフィックス: 汎用ユーティリティには付けない。ドメイン固有になったらプロジェクト名プレフィックスを検討
- `ft_` は廃止方向
- エラー時は `NULL` を返す（エラーコードは返さない）
- 出力は `printf` のみ

---

## ドキュメント化TODO

- [ ] 各関数のシグネチャと挙動を `docs/api.md` にまとめる
- [ ] ゲノム解析での具体的な利用シナリオを `docs/use_cases.md` に書く
- [ ] モノレポ移行時の構成案を `docs/architecture.md` に書く
