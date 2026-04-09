#include "test_framework.h"
#include "lib.h"

/* ===== Edge ===== */

static void test_edges(void)
{
    TEST_GROUP("Edge: create / len / get_last / get_target");
    {
        Edge *e = create_edge(0, 1, 2, 3.14);
        ASSERT(e != NULL,      "create_edge: non-NULL");
        ASSERT(e->id   == 0,   "create_edge: id");
        ASSERT(e->from == 1,   "create_edge: from");
        ASSERT(e->to   == 2,   "create_edge: to");
        ASSERT(e->cost == 3.14,"create_edge: cost");
        ASSERT(e->next == NULL,"create_edge: next is NULL");

        Edge *e2 = create_edge(1, 2, 3, 1.0);
        e->next = e2;
        ASSERT(len_edge_list(e) == 2,  "len_edge_list: 2 edges");
        ASSERT(len_edge_list(e2) == 1, "len_edge_list: 1 edge from middle");
        ASSERT(len_edge_list(NULL) == 0, "len_edge_list: NULL = 0");

        ASSERT(get_last_edge(e) == e2, "get_last_edge: returns tail");
        ASSERT(get_last_edge(NULL) == NULL, "get_last_edge(NULL): NULL");

        ASSERT(get_target_edge(e, 2) == e,    "get_target_edge: finds first edge (to=2)");
        ASSERT(get_target_edge(e, 3) == e2,   "get_target_edge: finds second edge (to=3)");
        ASSERT(get_target_edge(e, 99) == NULL,"get_target_edge: missing target = NULL");

        xfree(e, sizeof(Edge));
        xfree(e2, sizeof(Edge));
    }

    TEST_GROUP("Edge: add_directed_edge / add_undirected_edge");
    {
        Vertex **vs = create_vertex_array(3);
        add_directed_edge(vs, 0, 1, 0, 1.0);
        add_directed_edge(vs, 0, 2, 1, 2.0);
        ASSERT(len_edge_list(vs[0]->incidence) == 2, "add_directed_edge: 2 edges from vertex 0");
        ASSERT(vs[1]->incidence == NULL, "add_directed_edge: vertex 1 has no outgoing edges");
        ASSERT(vs[2]->incidence == NULL, "add_directed_edge: vertex 2 has no outgoing edges");

        add_undirected_edge(vs, 1, 2, 2, 5.0);
        ASSERT(len_edge_list(vs[1]->incidence) == 1, "add_undirected_edge: vertex 1 gets 1 edge");
        ASSERT(len_edge_list(vs[2]->incidence) == 1, "add_undirected_edge: vertex 2 gets 1 edge");
        ASSERT(vs[1]->incidence->to == 2, "add_undirected_edge: v1->v2 direction correct");
        ASSERT(vs[2]->incidence->to == 1, "add_undirected_edge: v2->v1 direction correct");
        free_vertex_array(vs, 3);
    }
}

/* ===== Vertex ===== */

static void test_vertices(void)
{
    TEST_GROUP("Vertex: create / create_array / reset_labels");
    {
        Vertex *v = create_vertex(5);
        ASSERT(v != NULL,          "create_vertex: non-NULL");
        ASSERT(v->id == 5,         "create_vertex: id set");
        ASSERT(v->label == 5,      "create_vertex: label initialized to id");
        ASSERT(v->incidence == NULL,"create_vertex: incidence is NULL");
        xfree(v, sizeof(Vertex));

        Vertex **vs = create_vertex_array(4);
        ASSERT(vs != NULL,              "create_vertex_array: non-NULL");
        ASSERT(vs[0]->id == 0,          "create_vertex_array: vs[0].id = 0");
        ASSERT(vs[3]->id == 3,          "create_vertex_array: vs[3].id = 3");

        /* Modify labels, then reset */
        vs[0]->label = 99;
        vs[1]->label = 88;
        reset_labels(vs, 4);
        ASSERT(vs[0]->label == 0, "reset_labels: vs[0].label reset to id (0)");
        ASSERT(vs[1]->label == 1, "reset_labels: vs[1].label reset to id (1)");
        free_vertex_array(vs, 4);
    }

    TEST_GROUP("Vertex: update_labels");
    {
        Vertex **vs = create_vertex_array(3);
        PathNode *p0 = create_pathnode(vs[0]);
        PathNode *p1 = create_pathnode(vs[1]);
        PathNode *p2 = create_pathnode(vs[2]);
        p0->next = p1; p1->prev = p0;
        p1->next = p2; p2->prev = p1;

        update_labels(p0, 42);
        ASSERT(vs[0]->label == 42, "update_labels: vs[0] updated");
        ASSERT(vs[1]->label == 42, "update_labels: vs[1] updated");
        ASSERT(vs[2]->label == 42, "update_labels: vs[2] updated");

        xfree(p0, sizeof(PathNode));
        xfree(p1, sizeof(PathNode));
        xfree(p2, sizeof(PathNode));
        free_vertex_array(vs, 3);
    }
}

/* ===== EdgeNode ===== */

static void test_edgenodes(void)
{
    TEST_GROUP("EdgeNode: create / append / insert / count / cost / pop / merge");
    {
        Edge *e1 = create_edge(0, 0, 1, 1.0);
        Edge *e2 = create_edge(1, 1, 2, 2.0);
        Edge *e3 = create_edge(2, 2, 3, 3.0);

        EdgeNode *en1 = create_edgenode(e1);
        ASSERT(en1 != NULL,        "create_edgenode: non-NULL");
        ASSERT(en1->edge == e1,    "create_edgenode: edge pointer set");
        ASSERT(en1->prev == NULL,  "create_edgenode: prev is NULL");
        ASSERT(en1->next == NULL,  "create_edgenode: next is NULL");

        EdgeNode *en2 = create_edgenode(e2);
        EdgeNode *en3 = create_edgenode(e3);

        EdgeNode *head = NULL;
        append_edgenode(&head, en1);
        append_edgenode(&head, en2);
        append_edgenode(&head, en3);

        ASSERT(count_edgenodes(head) == 3, "count_edgenodes: 3 nodes");
        ASSERT(count_edgenodes(NULL) == 0, "count_edgenodes(NULL): 0");
        ASSERT(get_last_edgenode(head) == en3, "get_last_edgenode: returns tail");

        double total = cost_of_edgenodes(head);
        ASSERT(total == 6.0, "cost_of_edgenodes: 1+2+3 = 6.0");

        /* insert_node_after en1 */
        Edge *e_mid = create_edge(99, 0, 9, 9.9);
        EdgeNode *en_mid = create_edgenode(e_mid);
        insert_node_after(en1, en_mid);
        ASSERT(count_edgenodes(head) == 4,   "insert_node_after: count becomes 4");
        ASSERT(en1->next == en_mid,           "insert_node_after: en1->next is new node");
        ASSERT(en_mid->next == en2,           "insert_node_after: new node->next is en2");

        /* pop first */
        EdgeNode *popped = edgenode_pop_first(&head);
        ASSERT(popped == en1,              "edgenode_pop_first: returns first node");
        ASSERT(count_edgenodes(head) == 3, "edgenode_pop_first: count becomes 3");
        xfree(popped, sizeof(EdgeNode));

        /* merge two lists */
        EdgeNode *head2 = NULL;
        Edge *e4 = create_edge(3, 3, 4, 4.0);
        EdgeNode *en4 = create_edgenode(e4);
        append_edgenode(&head2, en4);

        merge_edgenode(&head, &head2);
        ASSERT(count_edgenodes(head) == 4, "merge_edgenode: total 4 nodes after merge");
        ASSERT(head2 == NULL, "merge_edgenode: source list becomes NULL");

        /* cleanup */
        free_edgenode(&head);
        ASSERT(head == NULL, "free_edgenode: head set to NULL");
        xfree(e1, sizeof(Edge));
        xfree(e2, sizeof(Edge));
        xfree(e3, sizeof(Edge));
        xfree(e4, sizeof(Edge));
        xfree(e_mid, sizeof(Edge));
    }
}

/* ===== PathNode ===== */

static void test_pathnodes(void)
{
    TEST_GROUP("PathNode: create / append / count / pop / merge / free");
    {
        Vertex **vs = create_vertex_array(5);

        PathNode *p0 = create_pathnode(vs[0]);
        ASSERT(p0 != NULL,        "create_pathnode: non-NULL");
        ASSERT(p0->v == vs[0],    "create_pathnode: vertex pointer set");
        ASSERT(p0->prev == NULL,  "create_pathnode: prev is NULL");
        ASSERT(p0->next == NULL,  "create_pathnode: next is NULL");

        PathNode *head = NULL;
        append_pathnode(&head, p0);
        append_pathnode(&head, create_pathnode(vs[1]));
        append_pathnode(&head, create_pathnode(vs[2]));
        ASSERT(count_pathnodes(head) == 3, "count_pathnodes: 3 nodes");
        ASSERT(count_pathnodes(NULL) == 0, "count_pathnodes(NULL): 0");

        PathNode *last = get_last_pathnode(head);
        ASSERT(last->v == vs[2], "get_last_pathnode: tail vertex is vs[2]");

        /* pop last */
        pathnode_pop_last(&head);
        ASSERT(count_pathnodes(head) == 2, "pathnode_pop_last: count becomes 2");
        ASSERT(get_last_pathnode(head)->v == vs[1], "pathnode_pop_last: new tail is vs[1]");

        /* pop first */
        PathNode *first = pathnode_pop_first(&head);
        ASSERT(first->v == vs[0], "pathnode_pop_first: returns head (vs[0])");
        ASSERT(count_pathnodes(head) == 1, "pathnode_pop_first: count becomes 1");
        xfree(first, sizeof(PathNode));

        /* insert after */
        PathNode *p_new = create_pathnode(vs[3]);
        insert_pathnode_after(head, p_new);
        ASSERT(count_pathnodes(head) == 2, "insert_pathnode_after: count becomes 2");
        ASSERT(head->next == p_new, "insert_pathnode_after: head->next is new node");

        /* merge */
        PathNode *head2 = NULL;
        append_pathnode(&head2, create_pathnode(vs[4]));
        merge_pathnode(&head, &head2);
        ASSERT(count_pathnodes(head) == 3, "merge_pathnode: total 3 nodes");
        ASSERT(head2 == NULL, "merge_pathnode: source becomes NULL");

        free_pathnode(&head);
        ASSERT(head == NULL, "free_pathnode: head set to NULL");
        free_vertex_array(vs, 5);
    }
}

/* ===== Node (generic) ===== */

static void test_nodes(void)
{
    TEST_GROUP("Node: create / append / length / pop / enqueue / dequeue / type access");
    {
        Edge *e = create_edge(0, 0, 1, 1.0);
        Vertex *v = create_vertex(7);

        Node *ne = create_node(e, NODE_TYPE_EDGE);
        ASSERT(ne != NULL,              "create_node(edge): non-NULL");
        ASSERT(ne->type == NODE_TYPE_EDGE, "create_node(edge): type set");
        ASSERT(node_get_edge(ne) == e,  "node_get_edge: returns correct edge");
        ASSERT(node_get_vertex(ne) == NULL, "node_get_vertex on edge node: NULL");

        Node *nv = create_node(v, NODE_TYPE_VERTEX);
        ASSERT(nv->type == NODE_TYPE_VERTEX, "create_node(vertex): type set");
        ASSERT(node_get_vertex(nv) == v, "node_get_vertex: returns correct vertex");
        ASSERT(node_get_edge(nv) == NULL,    "node_get_edge on vertex node: NULL");

        /* append / length */
        Node *queue = NULL;
        append_node(&queue, ne);
        append_node(&queue, nv);
        ASSERT(get_node_length(queue) == 2, "get_node_length: 2 nodes");
        ASSERT(get_node_length(NULL) == 0,  "get_node_length(NULL): 0");

        /* pop first */
        Node *popped = pop_first_node(&queue);
        ASSERT(popped == ne,               "pop_first_node: returns first node");
        ASSERT(get_node_length(queue) == 1, "pop_first_node: length decreases to 1");
        free_node(popped);

        /* enqueue / dequeue (FIFO) */
        Node *q2 = NULL;
        Edge *ea = create_edge(1, 1, 2, 2.0);
        Edge *eb = create_edge(2, 2, 3, 3.0);
        Node *na = create_node(ea, NODE_TYPE_EDGE);
        Node *nb = create_node(eb, NODE_TYPE_EDGE);
        enqueue_node(&q2, na);
        enqueue_node(&q2, nb);
        Node *dq = dequeue_node(&q2);
        ASSERT(dq == na, "dequeue_node: FIFO - first in, first out");
        free_node(dq);

        /* cleanup */
        free_nodes(&queue);
        ASSERT(queue == NULL, "free_nodes: queue set to NULL");
        free_nodes(&q2);
        xfree(e, sizeof(Edge));
        xfree(v, sizeof(Vertex));
        xfree(ea, sizeof(Edge));
        xfree(eb, sizeof(Edge));
    }
}

void run_tests_data_structures(void)
{
    test_edges();
    test_vertices();
    test_edgenodes();
    test_pathnodes();
    test_nodes();
}
