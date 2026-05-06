# Miranda (2020) 読解ノート — インデックス
**論文**: Length-weighted λ-rearrangement distance  
**著者**: Alexandrino, Miranda, Lintzmayer, Dias  
**掲載**: Journal of Combinatorial Optimization (2021) 41:579–602

## ファイル一覧

| ファイル | 内容 |
|---------|------|
| [2020_intro.md](2020_intro.md) | Sec 1: イントロダクション・問題設定 |
| [2020_definition.md](2020_definition.md) | Sec 2: 共通定義（順列・操作・コスト・グラフ等） |
| [2020_sec3_inversions.md](2020_sec3_inversions.md) | Sec 3: 転倒数ベースGreedy（符号なし） |
| [2020_sec4_entropy.md](2020_sec4_entropy.md) | Sec 4: エントロピーベースGreedy（符号なし） |
| [2020_sec5_signed.md](2020_sec5_signed.md) | Sec 5: スコア関数 φ（符号付き） |
| [2020_sec6_large_lambda.md](2020_sec6_large_lambda.md) | Sec 6: λ = Θ(n) の場合 |
| [2020_sec7_short.md](2020_sec7_short.md) | Sec 7: 短い再配置 λ = 3 の場合 |
| [2020_sec8_alpha.md](2020_sec8_alpha.md) | Sec 8: α > 1 の場合 |

## 主要成果一覧（Table 1）

| パラメータ | SbR | SbT | SbRT | SbR̄ | SbR̄T |
|-----------|-----|-----|------|------|-------|
| λ > 1, α = 1 | λ−1 | λ/2 | λ−1 | λ | λ |
| λ = Θ(n), α = 1 | O(lg n) | O(lg² n) | O(lg² n) | O(lg n) | O(lg² n) |
| λ = 3, α = 1 | 2 | 4/3 | 2 | 3 | 7/3 |
| α ≥ 2 | 2 | 2 | 2 | 3 | 3 |
| α ≥ 3 | 厳密解 | 厳密解 | 厳密解 | 厳密解 | 厳密解 |
