# Abbreviation Reference — work/

## 基本略称

| 略称   | 元の語              | 意味                              |
|--------|---------------------|-----------------------------------|
| `rev`  | reversal            | 区間反転                          |
| `srev` | signed reversal     | 符号付き反転（反転＋符号反転）    |
| `tpos` | transposition       | 転置（2ブロック入れ替え）         |
| `inv`  | inversion(s)        | 転倒数                            |
| `ent`  | entropy             | エントロピー（位置ズレの和）      |
| `bp`   | breakpoint(s)       | ブレークポイント数                |
| `lam`  | lambda              | λ（操作サイズ上限）               |
| `dec`  | decreasing          | 単調減少（ストリップの種類）      |
| `elem` | element             | 要素                              |
| `cnt`  | count               | カウント（戻り値が整数の関数）    |
| `soop` | smallest out-of-place | 最小の位置ずれ要素             |

## 関数名対応表

### foundations/

| 新（略称） | 旧（元の名前） |
|---|---|
| `apply_rev` | `apply_reversal` |
| `apply_srev` | `apply_signed_reversal` |
| `apply_tpos` | `apply_transposition` |
| `cnt_inv` | `count_inversions` |
| `delta_inv_rev` | `delta_inv_reversal` |
| `delta_inv_tpos` | `delta_inv_transposition` |
| `ent_elem` | `entropy_element` |
| `ent_perm` | `entropy_perm` |
| `cnt_Eminus` | `count_E_minus_even` |
| `cnt_Eplus` | `count_E_plus_odd` |
| `cnt_bp_rev` | `count_breakpoints_unsigned_rev` |
| `cnt_bp_tpos` | `count_breakpoints_transposition` |
| `is_lam_perm` | `is_lambda_perm` |
| `find_soop` | `find_smallest_out_of_place` |
| `find_strips` | — (変更なし) |

### lp_inversions/

| 新 | 旧 |
|---|---|
| `lp_inv_greedy` | `lp_inversions_greedy` |
| `LP_MODE_TPOS` | `LP_MODE_TRANS` |

### lp_breakpoints/

| 新 | 旧 |
|---|---|
| `lp_bp_rev` | `lp_bp_reversals` |
| `lp_bp_tpos` | `lp_bp_transpositions` |
| `lp_bp_both` | — (変更なし) |
| `move_strip_rev` | `move_strip_reversals` |
| `move_strip_tpos` | `move_strip_transpositions` |
| `move_strip_dec` | `move_strip_decreasing` |
| `rev_dec_strip` | `reverse_decreasing_strip` |

### lw_inversion/

| 新 | 旧 |
|---|---|
| `lw_inv_greedy` | `lw_inversion_greedy` |
| `LW_MODE_TPOS` | `LW_MODE_TRANS` |

### lw_entropy/

| 新 | 旧 |
|---|---|
| `lw_ent_greedy` | `lw_entropy_greedy` |
| `LW_ENT_MODE_TPOS` | `LW_ENT_MODE_TRANS` |

### 変更なし

- `lp_score_greedy`, `lp_bp_both`
- `lw_phi_greedy`, `lw_psi_greedy_short`, `lw_exact`
- `find_strips`, `Strip`

## 命名規則

- **動詞_目的語**: `apply_rev`, `apply_tpos` — 操作の適用
- **delta_inv_xxx**: Inv の変化量 ΔInv(π, β)
- **cnt_xxx**: カウント（整数を返す）
- **ent_xxx**: エントロピー関連
- **is_xxx**: 述語（bool を返す）
- **find_xxx**: 探索（インデックスを返す）
- **lp_** / **lw_**: アルゴリズム名前空間（lp = λ-perm, lw = length-weighted）
