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

0-indexed 変換：
```c
prefix_block_interchange(arr, n, i-1, j, k-1);
// = block_interchange(arr, n, 0, i-1, j, k-1)
// arr[0..i-1] と arr[j..k-1] を入れ替え、arr[i..j-1] は不動
```

特殊ケース β(1, i, i, j)（アルゴリズム line 13）：中間が空。
```c
prefix_block_interchange(arr, n, i-1, i, j-1);
// arr[0..i-1] と arr[i..j-1] を入れ替え（隣接ブロック交換）
```

### π ← πσ の意味
σ が block interchange そのものなので、直接 `block_interchange` を π に適用すればよい。

---

## 2. Breakpoint Graph G(π)

### 構造

unsigned linear permutation π = (π₁,...,πₙ) の拡張：
```
π̄ = (π₀=0, π₁, π₂, ..., πₙ, πₙ₊₁=n+1)
```

各値 v ∈ {0,...,n+1} を **左端 vₗ** と **右端 vᵣ** の2頂点に分割（計 2(n+2) 頂点）。

```
頂点番号: vₗ = 2v,  vᵣ = 2v+1
総頂点数: 2(n+2)
```

**黒辺（black arc）**：identity の隣接を表す。固定。
```
iᵣ — (i+1)ₗ    for i = 0..n
= 辺 (2i+1, 2(i+1))
```

**灰辺（grey arc）**：現在の順列 π̄ の隣接を表す。π が変わるたびに更新。
```
π̄ᵢᵣ — π̄ᵢ₊₁ₗ    for i = 0..n
= 辺 (2*π̄[i]+1, 2*π̄[i+1])
```

### 性質
各頂点の次数はちょうど 2（黒辺 1 + 灰辺 1）なので、グラフは
**頂点素な alternating cycle の集合**に完全分解される。

### データ構造
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

**最左サイクル** = 最初の灰辺（位置 i=0 の灰辺、すなわち 0ᵣ と π₁ₗ を結ぶ辺）を含むサイクル。

実装上は：頂点 `2*0+1 = 1`（= 0 の右端）が属するサイクルが最左サイクル。
```c
int leftmost_cycle_id = cycle_id[1];  // 頂点 0_r = 1 のサイクル番号
```

---

## 4. Lemma 4 のケース分析

### 前提
```
j = position of (π[0]-1) in π   // 0-indexed: perm[j] == perm[0]-1
```
j は「最初の黒辺 (0, π₁) と対をなす要素の位置」。

### Case A（Lemma 4 case 1〜3）：該当する灰辺が最左サイクルにある

条件：i ≤ j ≤ k かつ π[k] = π[i]+1 かつ 黒辺 (π[i], π[k]) が最左サイクルに属する

「黒辺 (v, v+1) が最左サイクルに属する」の確認方法：
```c
// v = π[i], v+1 = π[k]
// 黒辺の端点: vᵣ = 2v+1, (v+1)ₗ = 2(v+1)
cycle_id[2*v+1] == leftmost_cycle_id
// または
cycle_id[2*(v+1)] == leftmost_cycle_id
// （同じ辺の両端は同じサイクルに属する）
```

操作：`prefix_block_interchange(arr, n, i-1, j, k-1)`

**Lemma 4 case 3 の特殊形**：k = j+1（π[j+1] = π[i]+1、つまり j と k が隣接）の場合も case A に含まれる。この場合の β(1, i, j, k) は β(1, i, j, j+1) となり中間部分が1要素。

### Case B（Lemma 4 case 4）：該当する灰辺が最左サイクルにない

条件：i ≤ j ≤ k かつ π[k] = π[i]-1（+1 ではなく -1）

証明によれば、最左サイクル以外の何らかのサイクル C が最左灰辺と交差する辺を持つことが保証されており、そのような (i, k) が必ず存在する。

操作：`prefix_block_interchange(arr, n, i-1, j, k-1)`

### Case C（π₁ = 1 のとき、Theorem 5）

π[0] = 0（0-indexed）のとき：先頭要素は正しい位置にある。

```
i = first_breakpoint_position(π, n)  // π[i+1] ≠ π[i]+1 となる最小 i
j = find_value(π, n, π[i]+1)         // 値 π[i]+1 の位置
操作: prefix_block_interchange(arr, n, i-1, i, j-1)
    = β(1, i, i, j)  // 中間が空（i=i なので）
```

---

## 5. 実装する関数一覧

### 5.1 permutation.c に追加（小規模）

```c
// 値 val の位置を返す（0-indexed）。見つからなければ -1。
int find_value(int *perm, int n, int val);

// π[i+1] ≠ π[i]+1 となる最小 i を返す。なければ -1。
int first_breakpoint_position(int *perm, int n);
```

### 5.2 breakpoint_graph.c（新規）

```c
// π（0-indexed, 値 0..n-1）から breakpoint graph を構築
BreakpointGraph *bg_init(int *perm, int n);

// alternating cycle 分解を行い cycle_id と num_cycles をセット
void             bg_decompose_cycles(BreakpointGraph *bg);

// 黒辺 (v, v+1)（値、0-indexed）が最左サイクルに属するか
int              bg_black_arc_in_leftmost(BreakpointGraph *bg, int v);

// メモリ解放
void             bg_free(BreakpointGraph *bg);
```

### 5.3 approx_sbpbi.c（新規）

```c
/*
 * ApproximateSbpbi の実装。
 * perm: 入力順列（in-place でソートされる）
 * ops:  適用した操作を記録。ops[k] = {j, k, l} （prefix_block_interchange の引数、0-indexed）
 *       NULL を渡せば記録しない。
 * 戻り値: 適用した操作数
 */
int approx_sbpbi(int *perm, int n, int (*ops)[3]);

/* Case A の (i, k) 探索: i≤j≤k かつ π[k]=π[i]+1 かつ最左サイクルに属する */
static int find_case_a(int *perm, int n, int j, BreakpointGraph *bg, int *out_i, int *out_k);

/* Case B の (i, k) 探索: i≤j≤k かつ π[k]=π[i]-1 */
static int find_case_b(int *perm, int n, int j, int *out_i, int *out_k);
```

---

## 6. 実装順序

1. `find_value` / `first_breakpoint_position` → `permutation.c`
2. `BreakpointGraph` 構造体 → `includes/types.h`
3. `bg_init` → 灰辺・黒辺の隣接表を構築
4. `bg_decompose_cycles` → DFS/BFS で cycle_id を付番
5. `bg_black_arc_in_leftmost` → cycle_id の比較
6. `find_case_a` / `find_case_b` → 線形探索 O(n)
7. `approx_sbpbi` → アルゴリズム本体
8. テスト `tests/test_approx_sbpbi.c`

---

## 7. 計算量

| 処理 | 計算量 |
|------|--------|
| `bg_init` | O(n) |
| `bg_decompose_cycles` | O(n) |
| `find_case_a` / `find_case_b` | O(n) |
| `prefix_block_interchange` | O(n) |
| **1ステップ合計** | O(n) |
| **全体（最大 n ステップ）** | O(n²) |

---

## 8. テスト方針

- n=4, 5 の小さい例で手計算と照合
- `is_identity_permutation` でソート完了を検証
- 操作数が `count_breakpoints(π, n) / 3` × 2 程度であることを確認（2-近似の保証）
- ランダム順列を大量生成して `approx_sbpbi` → `is_identity_permutation` を確認

---

## 9. 注意点

- β 適用後は breakpoint graph を **再構築**する（最初の実装では差分更新は不要）
- Case A の探索は i を 0 から j まで、k を j から n-1 まで走査すれば O(n) で見つかる
  （π[k] = π[i]+1 となる k は位置配列（`pos[v] = position of value v`）を使えば O(1) で求まる）
- Case B も同様に `pos` 配列を使えば効率化できる
- `pos` 配列（逆写像）は `inverse_permutation` で得られる（既存関数）
