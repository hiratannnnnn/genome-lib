# Markdown → PDF 変換メモ

## ツール

- **md-to-pdf**：Markdown を Puppeteer/Chromium 経由で PDF に変換する npm パッケージ
- Mermaid 図・KaTeX 数式・テーブルをそのまま PDF に出力できる

## セットアップ済み環境

```bash
sudo npm install -g md-to-pdf
sudo apt install -y chromium-browser fonts-noto-cjk
```

- `chromium-browser`：WSL 内のネイティブ Chromium（Windows の Chrome.exe は不可）
- `fonts-noto-cjk`：日本語フォント（これがないと日本語が文字化けする）

## 設定ファイル `~/.md-to-pdf.json`

```json
{
  "launch_options": {
    "executablePath": "/usr/bin/chromium-browser"
  },
  "script": [
    {"url": "https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.min.js"},
    {
      "content": "mermaid.initialize({startOnLoad:false,theme:'default'}); document.querySelectorAll('code.language-mermaid').forEach(el=>{const d=document.createElement('div');d.className='mermaid';d.textContent=el.textContent;el.parentNode.replaceWith(d);}); mermaid.run();"
    }
  ],
  "wait_until": "networkidle0"
}
```

- `executablePath`：WSL のネイティブ Chromium を指定（必須）
- `script`：Mermaid を CDN からロードして ` ```mermaid ` ブロックをレンダリングする（デフォルトでは非対応なので必要）
- `wait_until: networkidle0`：Mermaid の描画完了を待つ

## エイリアス `~/.bashrc`

```bash
alias mdpdf='md-to-pdf --config-file ~/.md-to-pdf.json'
```

`--config-file` を明示しないと `~/.md-to-pdf.json` は読まれない。

## 使い方

```bash
source ~/.bashrc

# 1ファイル
mdpdf file.md

# ワイルドカードで一括（逐次処理）
mdpdf 2020*.md

# 並列処理したい場合
ls 2020*.md | xargs -P4 -I{} md-to-pdf --config-file ~/.md-to-pdf.json {}
```

## Markdown で使える記法

````markdown
# Mermaid 図
```mermaid
flowchart LR
  A --> B
```

# 数式（KaTeX）
$$f(\beta) = |\beta|^\alpha$$
````
