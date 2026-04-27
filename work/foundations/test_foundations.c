#include "foundations.h"
#include <stdio.h>
#include <assert.h>

static int passed = 0;
static int failed = 0;

#define CHECK(cond, msg) do { \
    if (cond) { passed++; } \
    else { failed++; printf("FAIL: %s\n", msg); } \
} while(0)

static void test_reversal(void)
{
    int p[] = {1, 2, 3, 4, 5};
    apply_reversal(p, 5, 1, 3);   /* reverse [2,3,4] → [4,3,2] */
    CHECK(p[0]==1 && p[1]==4 && p[2]==3 && p[3]==2 && p[4]==5,
          "apply_reversal basic");
}

static void test_transposition(void)
{
    int p[] = {1, 2, 3, 4, 5};
    apply_transposition(p, 5, 0, 2, 5); /* swap [1,2] and [3,4,5] */
    CHECK(p[0]==3 && p[1]==4 && p[2]==5 && p[3]==1 && p[4]==2,
          "apply_transposition basic");
}

static void test_inversions(void)
{
    int p[] = {3, 4, 1, 2};
    CHECK(count_inversions(p, 4) == 4, "count_inversions (3 4 1 2) = 4");

    int q[] = {1, 2, 3, 4};
    CHECK(count_inversions(q, 4) == 0, "count_inversions identity = 0");
}

static void test_entropy(void)
{
    /* ent(+5 at pos 0) = |5-1| = 4 */
    CHECK(entropy_element(5, 0) == 4, "entropy_element");

    int p[] = {5, -4, 3, -1, 2};
    int e = entropy_perm(p, 5);
    /* |5-1|+|-4|-2|+|3-3|+|-1|-4|+|2-5| = 4+2+0+3+3 = 12 */
    CHECK(e == 12, "entropy_perm (+5 -4 +3 -1 +2) = 12");
}

static void test_is_lambda_perm(void)
{
    /* (2 1 3) with lambda=2: ||2|-1|=1<2, ||1|-2|=1<2, ||3|-3|=0<2 → yes */
    int p[] = {2, 1, 3};
    CHECK(is_lambda_perm(p, 3, 2) == 1, "is_lambda_perm (2 1 3), λ=2 → yes");

    /* (3 1 2) with lambda=2: ||3|-1|=2 ≥ 2 → no */
    int q[] = {3, 1, 2};
    CHECK(is_lambda_perm(q, 3, 2) == 0, "is_lambda_perm (3 1 2), λ=2 → no");
}

int main(void)
{
    test_reversal();
    test_transposition();
    test_inversions();
    test_entropy();
    test_is_lambda_perm();

    printf("foundations: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
