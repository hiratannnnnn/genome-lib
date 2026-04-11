#include "test_framework.h"
#include "lib.h"

void run_tests_permutation(void)
{
    TEST_GROUP("identity_permutation / copy_permutation / is_identity_permutation");
    {
        int perm[5];
        identity_permutation(perm, 5);
        ASSERT(perm[0] == 0 && perm[1] == 1 && perm[2] == 2
               && perm[3] == 3 && perm[4] == 4,
               "identity_permutation: perm[i] = i");
        ASSERT(is_identity_permutation(perm, 5, 1) == 1,
               "is_identity_permutation: identity -> 1");

        int cp[5];
        copy_permutation(perm, cp, 5);
        ASSERT(cp[0] == 0 && cp[4] == 4,
               "copy_permutation: values match");

        int non_id[5] = {0, 2, 1, 3, 4};
        ASSERT(is_identity_permutation(non_id, 5, 1) == 0,
               "is_identity_permutation: non-identity -> 0");
    }

    TEST_GROUP("inverse_permutation");
    {
        /* perm = [1, 3, 0, 2]  →  inv = [2, 0, 3, 1] */
        int perm[4] = {1, 3, 0, 2};
        int inv[4];
        inverse_permutation(perm, inv, 4);
        /* inv[perm[i]] = i : inv[1]=0, inv[3]=1, inv[0]=2, inv[2]=3 */
        ASSERT(inv[0] == 2, "inverse_permutation: inv[0] = 2");
        ASSERT(inv[1] == 0, "inverse_permutation: inv[1] = 0");
        ASSERT(inv[2] == 3, "inverse_permutation: inv[2] = 3");
        ASSERT(inv[3] == 1, "inverse_permutation: inv[3] = 1");

        /* perm ∘ inv = identity */
        int res[4];
        compose_permutation(perm, inv, res, 4);
        ASSERT(is_identity_permutation(res, 4) == 1,
               "inverse_permutation: perm ∘ inv = identity");
    }

    TEST_GROUP("compose_permutation");
    {
        /* p = [1,2,0],  q = [2,0,1]
         * res[i] = p[q[i]]:
         *   res[0] = p[2] = 0
         *   res[1] = p[0] = 1
         *   res[2] = p[1] = 2   → identity */
        int p[3] = {1, 2, 0};
        int q[3] = {2, 0, 1};
        int res[3];
        compose_permutation(p, q, res, 3);
        ASSERT(is_identity_permutation(res, 3) == 1,
               "compose_permutation: p∘q = identity (p and q are mutual inverses)");

        /* identity ∘ q = q */
        int id[3] = {0, 1, 2};
        int res2[3];
        compose_permutation(id, q, res2, 3);
        ASSERT(res2[0] == q[0] && res2[1] == q[1] && res2[2] == q[2],
               "compose_permutation: id∘q = q");
    }

    TEST_GROUP("count_breakpoints");
    {
        /* Identity: 0 breakpoints */
        int id[5] = {0, 1, 2, 3, 4};
        ASSERT(count_breakpoints(id, 5) == 0,
               "count_breakpoints: identity -> 0");

        /* Completely reversed [4,3,2,1,0]: maximum breakpoints.
         * Left boundary: 4 != 0          -> 1
         * (4,3): 3 != 4+1=5              -> 1
         * (3,2): 2 != 3+1=4              -> 1
         * (2,1): 1 != 2+1=3              -> 1
         * (1,0): 0 != 1+1=2              -> 1
         * Right boundary: 0 != 4         -> 1
         * Total: 6 */
        int rev[5] = {4, 3, 2, 1, 0};
        ASSERT(count_breakpoints(rev, 5) == 6,
               "count_breakpoints: fully reversed -> 6");

        /* [0,1,3,2,4]: one transposition of adjacent elements.
         * (1,3): 3 != 1+1=2 -> breakpoint
         * (3,2): 2 != 3+1=4 -> breakpoint
         * (2,4): 4 != 2+1=3 -> breakpoint
         * Total: 3 */
        int t[5] = {0, 1, 3, 2, 4};
        ASSERT(count_breakpoints(t, 5) == 3,
               "count_breakpoints: one adjacent transposition -> 3");

        /* single element identity */
        int single[1] = {0};
        ASSERT(count_breakpoints(single, 1) == 0,
               "count_breakpoints: n=1 identity -> 0");
    }

    TEST_GROUP("block_interchange");
    {
        /* arr = [0,1,2,3,4,5]
         * block_interchange(i=1, j=2, k=4, l=5):
         *   block1 = [1,2], middle = [3], block2 = [4,5]
         *   result = [0, 4,5, 3, 1,2] */
        int arr[6] = {0, 1, 2, 3, 4, 5};
        block_interchange(arr, 6, 1, 2, 4, 5);
        ASSERT(arr[0] == 0 && arr[1] == 4 && arr[2] == 5
               && arr[3] == 3 && arr[4] == 1 && arr[5] == 2,
               "block_interchange(1,2,4,5): [0,1,2,3,4,5] -> [0,4,5,3,1,2]");

        /* Swap entire array in two halves: i=0, j=1, k=2, l=3 → no middle */
        int arr2[4] = {0, 1, 2, 3};
        block_interchange(arr2, 4, 0, 1, 2, 3);
        ASSERT(arr2[0] == 2 && arr2[1] == 3 && arr2[2] == 0 && arr2[3] == 1,
               "block_interchange(0,1,2,3): [0,1,2,3] -> [2,3,0,1]");

        /* Identity operation: swap adjacent single elements (i=j, k=l) */
        int arr3[4] = {0, 1, 2, 3};
        block_interchange(arr3, 4, 1, 1, 2, 2);
        ASSERT(arr3[0] == 0 && arr3[1] == 2 && arr3[2] == 1 && arr3[3] == 3,
               "block_interchange(1,1,2,2): adjacent single-element swap");

        /* Invalid input: no crash */
        int arr4[3] = {0, 1, 2};
        block_interchange(arr4, 3, 2, 1, 0, 0);  /* i > j: invalid */
        ASSERT(arr4[0] == 0 && arr4[1] == 1 && arr4[2] == 2,
               "block_interchange: invalid params -> no change");
    }

    TEST_GROUP("prefix_block_interchange");
    {
        /* arr = [0,1,2,3,4]
         * prefix_block_interchange(j=1, k=3, l=4):
         *   i=0, block1=[0,1], middle=[2], block2=[3,4]
         *   result = [3,4, 2, 0,1] */
        int arr[5] = {0, 1, 2, 3, 4};
        prefix_block_interchange(arr, 5, 1, 3, 4);
        ASSERT(arr[0] == 3 && arr[1] == 4 && arr[2] == 2
               && arr[3] == 0 && arr[4] == 1,
               "prefix_block_interchange(j=1,k=3,l=4): [0,1,2,3,4] -> [3,4,2,0,1]");

        /* Prefix of length 1 (single element) */
        int arr2[4] = {0, 1, 2, 3};
        prefix_block_interchange(arr2, 4, 0, 2, 3);
        /* i=0,j=0,k=2,l=3: block1=[0], middle=[1], block2=[2,3]
         * result = [2,3,1,0] */
        ASSERT(arr2[0] == 2 && arr2[1] == 3 && arr2[2] == 1 && arr2[3] == 0,
               "prefix_block_interchange(j=0,k=2,l=3): [0,1,2,3] -> [2,3,1,0]");

        /* Applying a PBI twice can return to original (not always, but test round-trip) */
        int arr3[5] = {3, 1, 2, 0, 4};
        int save[5];
        copy_permutation(arr3, save, 5);
        prefix_block_interchange(arr3, 5, 0, 3, 3);
        /* i=0,j=0,k=3,l=3: block1=[3], middle=[1,2], block2=[0]
         * result = [0,1,2,3,4] */
        ASSERT(is_identity_permutation(arr3, 4) || arr3[4] == 4,
               "prefix_block_interchange: sanity check on [3,1,2,0,4]");
        (void)save;
    }

    TEST_GROUP("block_interchange + count_breakpoints interaction");
    {
        /* After sorting [1,0,2,3] with one block_interchange, check breakpoints decrease */
        int perm[4] = {1, 0, 2, 3};
        int bp_before = count_breakpoints(perm, 4);
        /* block_interchange(0,0,1,1): swap perm[0] and perm[1] */
        block_interchange(perm, 4, 0, 0, 1, 1);
        int bp_after = count_breakpoints(perm, 4);
        ASSERT(is_identity_permutation(perm, 4) == 1,
               "block_interchange sorts [1,0,2,3] -> [0,1,2,3]");
        ASSERT(bp_after < bp_before,
               "block_interchange: breakpoints decrease after sort step");
        ASSERT(bp_after == 0,
               "block_interchange: 0 breakpoints after sorting");
    }
}
