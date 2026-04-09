#include "test_framework.h"
#include "lib.h"

#define TMP_INT_MATRIX  "/tmp/test_glib_int_matrix.txt"
#define TMP_DBL_MATRIX  "/tmp/test_glib_dbl_matrix.txt"
#define TMP_ADJ_LIST    "/tmp/test_glib_adj_list.txt"
#define TMP_GNL         "/tmp/test_glib_gnl.txt"

/* ---- helpers ---- */

static void write_file(const char *path, const char *content)
{
    FILE *f = fopen(path, "w");
    if (!f) return;
    fputs(content, f);
    fclose(f);
}

/* ---- tests ---- */

void run_tests_io(void)
{
    TEST_GROUP("write_adjacent_matrix / read_adj (roundtrip)");
    {
        /*
         * Build a 3x3 adjacency matrix, write, re-read, compare.
         */
        int **orig = gen_matrix_int(3, 3);
        orig[0][1] = orig[1][0] = 1;
        orig[1][2] = orig[2][1] = 1;

        int ret = write_adjacent_matrix(orig, 3, TMP_INT_MATRIX);
        ASSERT(ret == 0, "write_adjacent_matrix: returns 0 on success");

        int n = 0;
        int **loaded = read_adj(&n, TMP_INT_MATRIX);
        ASSERT(loaded != NULL, "read_adj: non-NULL after write");
        ASSERT(n == 3, "read_adj: n correctly read as 3");

        int eq = 1;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (orig[i][j] != loaded[i][j]) eq = 0;
        ASSERT(eq, "read_adj: matrix matches written data");

        free_matrix_int(orig, 3, 3);
        free_matrix_int(loaded, 3, 3);
    }

    TEST_GROUP("write_double_matrix / read_double_matrix (roundtrip)");
    {
        double **orig = gen_matrix_double(2, 3);
        orig[0][0] = 1.5; orig[0][1] = 2.5; orig[0][2] = 0.0;
        orig[1][0] = 0.0; orig[1][1] = 3.5; orig[1][2] = 4.5;

        int ret = write_double_matrix(orig, 2, 3, TMP_DBL_MATRIX);
        ASSERT(ret == 0, "write_double_matrix: returns 0 on success");

        int r = 0, c = 0;
        double **loaded = read_double_matrix(&r, &c, TMP_DBL_MATRIX);
        ASSERT(loaded != NULL, "read_double_matrix: non-NULL after write");
        ASSERT(r == 2 && c == 3, "read_double_matrix: dimensions correct (2x3)");

        int eq = 1;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 3; j++)
                if (fabs(orig[i][j] - loaded[i][j]) > 1e-6) eq = 0;
        ASSERT(eq, "read_double_matrix: values match written data");

        free_matrix_double(orig, 2, 3);
        free_matrix_double(loaded, r, c);
    }

    TEST_GROUP("write_adjacent_list / read_list (roundtrip)");
    {
        int **orig = gen_matrix_int(3, 3);
        orig[0][1] = orig[1][0] = 1;
        orig[0][2] = orig[2][0] = 1;

        int ret = write_adjacent_list(orig, 3, TMP_ADJ_LIST);
        ASSERT(ret == 0, "write_adjacent_list: returns 0 on success");

        int n = 0;
        int **loaded = read_list(&n, TMP_ADJ_LIST);
        ASSERT(loaded != NULL, "read_list: non-NULL after write");
        ASSERT(n == 3, "read_list: n = 3");

        int eq = 1;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (orig[i][j] != loaded[i][j]) eq = 0;
        ASSERT(eq, "read_list: matrix matches written adjacency list");

        free_matrix_int(orig, 3, 3);
        free_matrix_int(loaded, 3, 3);
    }

    TEST_GROUP("get_next_line");
    {
        /* Write a file with 3 lines */
        write_file(TMP_GNL, "first\nsecond\nthird\n");

        int fd = open(TMP_GNL, O_RDONLY);
        ASSERT(fd >= 0, "get_next_line: file opened");

        char *line1 = get_next_line(fd);
        ASSERT(line1 != NULL, "get_next_line: first line non-NULL");
        ASSERT(strcmp(line1, "first\n") == 0, "get_next_line: first line = \"first\\n\"");
        free(line1);

        char *line2 = get_next_line(fd);
        ASSERT(line2 != NULL, "get_next_line: second line non-NULL");
        ASSERT(strcmp(line2, "second\n") == 0, "get_next_line: second line = \"second\\n\"");
        free(line2);

        char *line3 = get_next_line(fd);
        ASSERT(line3 != NULL, "get_next_line: third line non-NULL");
        ASSERT(strcmp(line3, "third\n") == 0, "get_next_line: third line = \"third\\n\"");
        free(line3);

        char *eof = get_next_line(fd);
        ASSERT(eof == NULL, "get_next_line: returns NULL at EOF");
        close(fd);

        /* File with no trailing newline */
        write_file(TMP_GNL, "noeol");
        int fd2 = open(TMP_GNL, O_RDONLY);
        char *l = get_next_line(fd2);
        ASSERT(l != NULL && strcmp(l, "noeol") == 0,
               "get_next_line: reads last line without trailing newline");
        free(l);
        char *l2 = get_next_line(fd2);
        ASSERT(l2 == NULL, "get_next_line: NULL after last line without newline");
        close(fd2);
    }
}
