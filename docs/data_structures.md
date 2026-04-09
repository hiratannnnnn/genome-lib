# データ構造リファレンス

## 現在の構造体（types.h）

### Edge — 辺（有向・無向共用）

```c
typedef struct Edge {
    int    id;       // 辺ID
    int    from;     // 始点頂点ID
    int    to;       // 終点頂点ID
    double cost;     // コスト（重みなしグラフでは 0.0）
    struct Edge *next; // 単方向リスト
} Edge;
```

### Vertex — 頂点

```c
typedef struct Vertex {
    int   id;
    int   label;        // 汎用ラベル（アルゴリズム中に一時的に使う）
    Edge *incidence;    // 接続する辺の単方向リスト
} Vertex;
```

### EdgeNode — 辺を格納する双方向リストノード

```c
typedef struct EdgeNode {
    Edge          *edge;
    struct EdgeNode *prev;
    struct EdgeNode *next;
} EdgeNode;
```

### PathNode — 頂点を格納する双方向リストノード

```c
typedef struct PathNode {
    Vertex          *v;
    struct PathNode *prev;
    struct PathNode *next;
} PathNode;
```

### Node — 汎用双方向リストノード（EdgeNode / PathNode を統合する目的で追加）

```c
typedef struct Node {
    void     *ptr;   // Edge* または Vertex* を格納
    NodeType  type;
    struct Node *prev;
    struct Node *next;
} Node;

typedef enum {
    NODE_TYPE_EDGE,
    NODE_TYPE_VERTEX,
    ROUND,   // ← 未使用・要削除（Issue #12）
    SQUARE,  // ← 未使用・要削除（Issue #12）
} NodeType;
```

---

## 設計上の問題点と今後の方針

### PathNode / EdgeNode / Node が3種類ある理由

- `PathNode`（頂点リスト）と `EdgeNode`（辺リスト）が先に実装された
- 後から汎用的に扱うために `Node` を追加したが、3種類が並立した状態になっている
- Issue #6 として統合を検討中
- **ゲノムライブラリへ移行するタイミング**で、ドメインに合った形に再設計する

### Heap 構造体（現在未使用）

```c
typedef int (*HeapCmp)(const void *a, const void *b);

typedef struct {
    void **array;
    int    size;
    int    capacity;
    HeapCmp cmp;
} Heap;
```

将来のダイクストラ法や優先度付きアルゴリズム向けに定義されているが、実装ファイルが存在しない。

---

## メモリ管理

すべての動的確保は追跡付きアロケータ経由で行う：

| 関数 | 対応する標準関数 |
|------|----------------|
| `xmalloc(size)` | `malloc` |
| `xcalloc(n, size)` | `calloc` |
| `xfree(ptr, size)` | `free` |
| `xrealloc(ptr, old_size, new_size)` | `realloc` |

確保・解放のたびにグローバル変数 `mem` を増減し、`mem_peak` にピークを記録する。

> **注**: `malloc` / `free` の直接呼び出しが一部ソースに残っている（Issue #7）。統一が必要。
