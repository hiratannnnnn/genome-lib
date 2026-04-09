# ApproximateSbpbi 実装計画

## 1. 論文記法 → コード記法の対応

### 添字の基準
論文は **1-indexed** (値・位置ともに {1,...,n})。コードは **0-indexed** ({0,...,n-1})。

### 操作 β(1, i, j, k) の意味

論文の β(1, i, j, k)（1 ≤ i ≤ j ≤ k ≤ n）は次の prefix block interchange：

```
前: [ 1..i ] [ i+1..j ] [ j+1..k ] [ k+1..n ]
後: [ j+1..k ] [ i+1..j ] [ 1..i ] [ k+1..n ]
     ↑第2ブロック  ↑中間(不動)   ↑第1ブロック
```

0-indexed に変換すると：
```c
prefix_block_interchange(arr, n, i-1, j, k-1);
// = block_interchange(arr, n, 0, i-1, j, k-1)
// arr[0..i-1] と arr[j..k-1] を入れ替え、arr[i..j-1] は不動
```

特殊ケース β(1, i, i, j)（アルゴリズム line 13）：中間部分が空（arr[i..i-1]）。
```c
prefix_block_interchange(arr, n, i-1, i, j-1);
// arr[0..i-1] と arr[i..j-1] を入れ替え（隣接ブロックの交換）
```

### π ← πσ の意味
`compose_permutation` を使う（σ が block interchange そのものなので、
実装上は直接 `block_interchange` を π に適用すればよい）。

---

## 2. Breakpoint Graph G(π)

### 構造

unsigned linear permutation π = (π₁,...,πₙ) の拡張：
```
π̄ = (π₀=0, π₁, π₂, ..., πₙ, πₙ₊₁=n+1)
```

各値 v ∈ {0,...,n+1} を **左端 vₗ** と **右端 vᵣ** の2頂点に分割する（計 2(n+2) 頂点）。

```
頂点番号: v の左端 = 2v,  v の右端 = 2v+1
総頂点数: 2(n+2)
```

**黒辺（black arc）**：identity の隣接を表す。固定。
```
(i_r, (i+1)_l)  for i = 0..n
= 辺 (2i+1, 2(i+1))
```

**灰辺（grey arc）**：現在の順列 π̄ の隣接を表す。π が変わるたびに更新。
```
(π̄ᵢ_r, π̄ᵢ₊₁_l)  for i = 0..n
= 辺 (2*π̄[i]+1, 2*π̄[i+1])
```

### 性質
各頂点の次数はちょうど 1（黒辺で 1、灰辺で 1）なので、グラフは **頂点素なサイクルの集合** に分解される（alternating cycle decomposition）。

### 実装するデータ構造
```c
typedef struct {
    int  n;          // 元の順列の長さ
    int *grey;       // grey[v] = v の灰辺の相手頂点 (size: 2*(n+2))
    int *black;      // black[v] = v の黒辺の相手頂点 (size: 2*(n+2))
    int *cycle_id;   // cycle_id[v] = v が属するサイクルの番号 (size: 2*(n+2))
    int  num_cycles; // サイクル総数
} BreakpointGraph;
```

---

## 3. Leftmost Cycle（最左サイクル）

アルゴリズムで「leftmost cycle of G(π)」とは、**灰辺 (0_r, π₁_l) を含むサイクル**、
すなわち拡張順列の最初の位置 (i=0) に対応するサイクル。

位置で言えば「π̄[0]=0 の右端頂点 2*0+1=1 が属するサイクル」。

---

## 4. アルゴリズムの各行に必要な操作

```
Input: π  (0-indexed, values 0..n-1)
       ι  (identity = [0,1,...,n-1])
```

| 行 | 操作 | 必要な関数 |
|----|------|-----------|
| `π ≠ ι` | 恒等置換チェック | `is_identity_permutation` (既存) |
| `π[0] ≠ 0` (論文 π₁≠1) | 先頭要素チェック | 直接参照 |
| `j ← position of π[0]-1` | 値 (π[0]-1) の位置を探す | `find_value(perm, n, val)` **(新規)** |
| 灰辺が leftmost cycle に属するか | breakpoint graph を参照 | `bg_build`, `bg_decompose_cycles` **(新規)** |
| `(πᵢ, πₖ = πᵢ+1)` を見つける | i ≤ j ≤ k かつ π[k]=π[i]+1 となる対を探索 | `find_grey_arc_in_leftmost_cycle` **(新規)** |
| `β(1, i, j, k)` の適用 | prefix block interchange | `prefix_block_interchange` (既存) |
| `i` = 最小の breakpoint 位置 (line 11) | π[i+1] ≠ π[i]+1 となる最小 i | `first_breakpoint_position` **(新規)** |
| `j ← position of π[i]+1` | 値 (π[i]+1) の位置 | `find_value` **(新規)** |

---

## 5. 実装する関数一覧

### 5.1 permutation.c に追加

```c
// 値 val の位置（0-indexed）を返す。見つからなければ -1。
int find_value(int *perm, int n, int val);

// π[i+1] ≠ π[i]+1 となる最小 i を返す（なければ -1）。
// line 11 の "smallest index such that π_{i+1} ≠ π_i + 1"
int first_breakpoint_position(int *perm, int n);
```

### 5.2 breakpoint_graph.c（新規ファイル）

```c
// breakpoint graph を構築して返す
BreakpointGraph *bg_build(int *perm, int n);

// サイクル分解を行い cycle_id と num_cycles をセット
void             bg_decompose_cycles(BreakpointGraph *bg);

// 灰辺 (2*a+1, 2*b) が leftmost cycle に属するか
// a, b は元の値（0-indexed）
int              bg_arc_in_leftmost_cycle(BreakpointGraph *bg, int a, int b);

// bg を解放
void             bg_free(BreakpointGraph *bg);
```

### 5.3 approx_sbpbi.c（新規ファイル）

```c
// アルゴリズム本体
// perm: 入力順列（破壊的変更あり）
// ops: 適用した操作を記録する配列（各要素は {j, k, l} の3整数）
// ops は呼び出し元で十分な大きさを確保すること（最大 n 操作）
// 戻り値: 適用した操作数
int approx_sbpbi(int *perm, int n, int (*ops)[3]);
```

---

## 6. 実装の順序

1. `find_value` / `first_breakpoint_position` → `permutation.c` に追加
2. `BreakpointGraph` 構造体 → `types.h` に追加
3. `bg_build` / `bg_decompose_cycles` / `bg_arc_in_leftmost_cycle` / `bg_free` → `breakpoint_graph.c`
4. Case A（Lemma 4 cases 1–3）の探索ロジック → `approx_sbpbi.c`
5. Case B（Lemma 4 case 4）の探索ロジック → `approx_sbpbi.c`
6. アルゴリズム全体 `approx_sbpbi` → `approx_sbpbi.c`
7. テスト → `tests/test_approx_sbpbi.c`

---

## 7. テスト方針

- 小さい例（n=4, 5）で手計算と照合
- `is_identity_permutation` でソート完了を検証
- 操作数が 2·OPT 以内であることを確認（OPT = `count_breakpoints(π,n) / 3` が下界の目安）
- ランダム順列で `approx_sbpbi` → `is_identity_permutation` を多数試行

---

## 8. 注意点・未確認事項

- 論文の "leftmost cycle" の定義を要確認（上記は推定）
- Lemma 4 の case 1〜3 と case 4 の具体的な条件を論文で確認してから実装する
- β の適用後に breakpoint graph を **再構築** するか **差分更新** するかは性能に影響（最初は再構築で実装し、後で最適化）
- Case A の探索は O(n)、全体は O(n²) になる見込み
