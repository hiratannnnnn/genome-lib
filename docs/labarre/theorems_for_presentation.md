# 発表参照用：重要定理まとめ

Labarre (2018) "Sorting by Prefix Block-Interchanges" より。

---

## Lemma 4（境界条件の根拠）

**主張**: π₁ ≠ 1 ならば、∆c = 2、∆c₁ ≥ 2、∆g ≤ -1 を満たす prefix BI が必ず存在する。

証明は4つのケースに分かれ、実装のパターン A/B に直接対応する。

### Algorithm 1 との不正確さ

Algorithm 1 の line 5 は次のように書かれている：

```
if ∃(πi, πk = πi+1) such that i ≤ j ≤ k and grey arc belongs to leftmost cycle
    σ ← β(1, i, j, k)
```

**不正確な点 1：j = k のケース（case 3）が暗黙的に省略されている**

Lemma 4 の case 3 では β(1, i, j, j)（第2ブロックが空のブロック転位）が使われる。
コードでは `j+1 == k` のとき `set_pos(pos, 1, i/2+1, i/2+1, k/2+1)` と
第2・第3引数を同じにすることで正しく対応している。

**不正確な点 2：cycle_id の条件が明記されていない**

「grey arc belongs to leftmost cycle」はコードでは `ctx->bp[ctx->tmp[i]].cycle_id == 0`
に対応する。パターン B（find_pattern_b）ではこの条件を外すことで
case 4（leftmost cycle 以外を操作する）を扱う。

---

## Theorem 5（アルゴリズムの正当性・上界）

**主張**: 任意の π ∈ Sₙ に対して pbid(π) ≤ g(π)

g(π) の定義：

```
g(π) = (n + 1 + c(G(π))) / 2  - c₁(G(π)) - f(π)
f(π) = 0  (π₁ = 1 のとき)
f(π) = 1  (それ以外)
```

### π₁ = 1 のケース（パターン C の根拠）

証明中に明示されている：

> "we select β(1, i, i, j) then makes πᵢ and πᵢ₊₁ contiguous"

これがコードの `find_pattern_c` の根拠であり、
`set_pos(pos, 1, i+2, i+2, j+1)` という j=k 形式で実装されている。
Algorithm 1 の else 節は「find i s.t. πᵢ₊₁ ≠ πᵢ+1」とのみ記述されており、
境界の詳細は証明の文中にしかない。

---

## Theorem 25（⌊2n/3⌋ がタイトな直径）

**主張**: Sₙ における prefix block-interchange の直径は ⌊2n/3⌋ である。

### 上界（Corollary 24 より）

Lemma 21「任意の prefix BI は breakpoint を最大3つ除去する」から：

```
pbid(π) ≤ 2 × ⌈(b(π) - 1) / 3⌉ ≤ 2n/3
```

### 下界（tight であること）

基底ケースとして以下の3つを明示的に確認：

| 順列 | n | pbid | ⌊2n/3⌋ |
|---|---|---|---|
| ⟨1 3 2⟩ | 3 | 2 | 2 |
| ⟨1 4 3 2⟩ | 4 | 2 | 2 |
| ⟨1 3 2 5 4⟩ | 5 | 3 | 3 |

`⟨n+1 n+3 n+2⟩` を末尾に連結することで任意の n に帰納的に構成。
各連結で Theorem 19 と Corollary 24 により下界・上界ともに 2 ずつ増加。

n ≡ 0 (mod 3) のとき ⌊2n/3⌋ に達する順列はおそらく
`⟨1 3 2 4 6 5 … n-2 n n-1⟩` のみ（論文末尾より）。
