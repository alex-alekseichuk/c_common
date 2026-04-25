#include <stdio.h>
#include <stdlib.h>

#include <common/allocator.h>
#include <common/rb_tree.h>

DECL_RB_TREE(int)
IMPL_RB_TREE(int)

static int int_cmp(void *a, void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;

    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}

static int count_nodes(const intRbNode *node) {
    if (node == (const intRbNode *)intRbNIL) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

typedef struct NodeInfo {
    const intRbNode *node;
    int depth;
    int index; // printing order index
} NodeInfo;

static void fill_nodes_in_print_order(
    const intRbNode *node,
    int depth,
    NodeInfo *out,
    int *out_idx
) {
    if (node == (const intRbNode *)intRbNIL) return;

    // Sideways tree order: right subtree first, then node, then left subtree.
    fill_nodes_in_print_order(node->right, depth + 1, out, out_idx);

    out[*out_idx] = (NodeInfo){
        .node = node,
        .depth = depth,
        .index = *out_idx,
    };
    (*out_idx)++;

    fill_nodes_in_print_order(node->left, depth + 1, out, out_idx);
}

static int lookup_index(const NodeInfo *infos, int n, const intRbNode *needle) {
    for (int i = 0; i < n; i++) {
        if (infos[i].node == needle) return i;
    }
    return -1;
}

static void print_tree_sideways_with_connectors(intRbTree *tree) {
    const intRbNode *root = tree->root;
    if (root == (const intRbNode *)intRbNIL) {
        printf("<empty>\n");
        return;
    }

    const int node_count = count_nodes(root);
    if (node_count <= 0) {
        printf("<empty>\n");
        return;
    }

    // First pass already counts nodes; allocate info table.
    NodeInfo *infos = (NodeInfo *)malloc((size_t)node_count * sizeof(NodeInfo));
    if (!infos) {
        printf("<allocation failure>\n");
        return;
    }

    int idx = 0;
    fill_nodes_in_print_order(root, 0, infos, &idx);

    int max_depth = 0;
    for (int i = 0; i < node_count; i++) {
        if (infos[i].depth > max_depth) max_depth = infos[i].depth;
    }

    // One output line per node (no empty rows between nodes).
    const int height = node_count;
    const int indent_unit = 4;
    const int x_node = 2; // where the node text starts inside a depth column
    const int width = (max_depth + 1) * indent_unit + 64;

    char *grid = (char *)malloc((size_t)height * (size_t)width);
    if (!grid) {
        free(infos);
        printf("<allocation failure>\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[(size_t)y * (size_t)width + (size_t)x] = ' ';
        }
    }

    // Place node text.
    for (int i = 0; i < node_count; i++) {
        const intRbNode *node = infos[i].node;
        const int y = infos[i].index;
        const int x0 = infos[i].depth * indent_unit + x_node;

        char buf[64];
        const char c = (node->color == RB_RED) ? 'R' : 'B';
        snprintf(buf, sizeof(buf), "%d(%c)", node->value, c);

        for (int k = 0; buf[k] != '\0' && (x0 + k) < width; k++) {
            grid[(size_t)y * (size_t)width + (size_t)(x0 + k)] = buf[k];
        }
    }

    // Draw connectors: for each parent edge to each child (direct children only).
    // Skip drawing when parent/child are consecutive in print order.
    for (int i = 0; i < node_count; i++) {
        const intRbNode *parent = infos[i].node;
        const int parent_depth = infos[i].depth;
        const int y_parent = infos[i].index;

        const intRbNode *children[2] = {parent->left, parent->right};
        for (int ci = 0; ci < 2; ci++) {
            const intRbNode *child = children[ci];
            if (child == (const intRbNode *)intRbNIL) continue;

            const int j = lookup_index(infos, node_count, child);
            if (j < 0) continue;

            // "Neighbour lines" => consecutive nodes in printing order.
            if (abs(infos[i].index - infos[j].index) == 1) continue;

            const int y_child = infos[j].index;
            // Place connectors just to the left of node text:
            // '+' at x_mid, '-' at x_dash, '|' in between.
            const int x_mid = parent_depth * indent_unit + (x_node - 2);
            const int x_dash = parent_depth * indent_unit + (x_node - 1);

            if (x_mid >= 0 && x_mid < width) {
                grid[(size_t)y_parent * (size_t)width + (size_t)x_mid] = '+';
                grid[(size_t)y_child * (size_t)width + (size_t)x_mid] = '+';
                for (int x = x_mid + 1; x <= x_mid + 3; x++)
                    grid[(size_t)y_child * (size_t)width + (size_t)x] = '-';
            }
            if (x_dash >= 0 && x_dash < width) {
                grid[(size_t)y_parent * (size_t)width + (size_t)x_dash] = '-';
                grid[(size_t)y_child * (size_t)width + (size_t)x_dash] = '-';
            }

            const int step = (y_child > y_parent) ? 1 : -1;
            for (int y = y_parent + step; y != y_child; y += step) {
                if (x_mid >= 0 && x_mid < width) {
                    grid[(size_t)y * (size_t)width + (size_t)x_mid] = '|';
                }
            }
        }
    }

    // Render.
    for (int y = 0; y < height; y++) {
        // Trim trailing spaces for nicer output.
        int last = width - 1;
        while (last > 0 && grid[(size_t)y * (size_t)width + (size_t)last] == ' ') {
            last--;
        }
        if (last < 0) last = 0;

        for (int x = 0; x <= last; x++) {
            putchar(grid[(size_t)y * (size_t)width + (size_t)x]);
        }
        putchar('\n');
    }

    free(grid);
    free(infos);
}

int main(void) {
    int values[] = {
        10, 5, 15, 3, 7, 12, 18, 7,
        1, 6, 8, 13, 14, 16, 20, 17, 19,
        -2, 0, 11, 9
    };
    const int n = (int)(sizeof(values) / sizeof(values[0]));

    Allocator alloc = sys_alloc;
    intRbTree tree = make_intRbTree(&alloc, int_cmp);

    for (int i = 0; i < n; i++) {
        (void)intRbTree_insert(&tree, values[i]);
    }

    printf("Red-black tree (sideways):\n");
    print_tree_sideways_with_connectors(&tree);

    rb_free(&tree);

    return 0;
}

