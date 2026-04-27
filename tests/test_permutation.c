#include "lib.h"
#include "test_framework.h"

void	run_tests_permutation(void)
{
	int	perm[5];
	int	cp[5];
	int	non_id[5] = {0, 2, 1, 3, 4};
	int	q4[4] = {1, 3, 0, 2};
	int	inv[4];
	int	res[4];
	int	p3[3] = {1, 2, 0};
	int	q3[3] = {2, 0, 1};
	int	res3[3];
	int	id3[3] = {0, 1, 2};
	int	res3b[3];
	int	id5[5] = {0, 1, 2, 3, 4};
	int	rev5[5] = {4, 3, 2, 1, 0};
	int	bp3[5] = {0, 1, 3, 2, 4};
	int	single[1] = {0};
	int	arr[5] = {0, 1, 2, 3, 4};
	int	nat5[5];
	int	found;
	int	pos;
	int	np[3] = {0, 1, 2};
	int	last[3] = {2, 1, 0};
	int	ret;
	int	ra[5] = {0, 1, 2, 3, 4};
	int	rb[5] = {0, 1, 2, 3, 4};
	int	rc[3] = {7, 8, 9};
	int	counter[3] = {0, 1, 2};
	int	steps;

	TEST_GROUP("identity_permutation / copy_permutation
		/ is_identity_permutation");
	{
		identity_permutation(perm, 5, 0);
		ASSERT(perm[0] == 0 && perm[1] == 1 && perm[2] == 2 && perm[3] == 3
			&& perm[4] == 4, "identity_permutation(n=5): perm[i] = i");
		ASSERT(is_identity_permutation(perm, 5, 0) == 1,
			"is_identity_permutation: 0-indexed identity -> 1");
		copy_permutation(perm, cp, 5);
		ASSERT(cp[0] == 0 && cp[4] == 4, "copy_permutation: values match");
		cp[2] = 99;
		ASSERT(perm[2] == 2,
			"copy_permutation: deep copy (modifying cp leaves perm intact)");
		ASSERT(is_identity_permutation(non_id, 5, 0) == 0,
			"is_identity_permutation: non-identity -> 0");
		/* Natural (1-indexed) identity: perm[i] = i+1 */
		identity_permutation(nat5, 5, 1);
		ASSERT(nat5[0] == 1 && nat5[4] == 5,
			"identity_permutation(is_natural=1): perm[i]=i+1");
		ASSERT(is_identity_permutation(nat5, 5, 1) == 1,
			"is_identity_permutation(is_natural=1): natural identity -> 1");
		ASSERT(is_identity_permutation(nat5, 5, 0) == 0,
			"is_identity_permutation: natural perm is not 0-indexed identity");
	}
	TEST_GROUP("inverse_permutation");
	{
		/* q4 = {1, 3, 0, 2}: inv[q4[i]]=i -> inv={2,0,3,1} */
		inverse_permutation(q4, inv, 4);
		ASSERT(inv[0] == 2, "inverse_permutation: inv[0] = 2");
		ASSERT(inv[1] == 0, "inverse_permutation: inv[1] = 0");
		ASSERT(inv[2] == 3, "inverse_permutation: inv[2] = 3");
		ASSERT(inv[3] == 1, "inverse_permutation: inv[3] = 1");
		/* q4 composed with inv should give identity */
		compose_permutation(q4, inv, res, 4);
		ASSERT(is_identity_permutation(res, 4, 0) == 1,
			"inverse_permutation: q4 o inv = identity");
		/* Inverse of identity is identity */
		compose_permutation(inv, q4, res, 4);
		ASSERT(is_identity_permutation(res, 4, 0) == 1,
			"inverse_permutation: inv o q4 = identity");
	}
	TEST_GROUP("compose_permutation");
	{
		/* p3={1,2,0}, q3={2,0,1}: res[i]=p3[q3[i]]
			* res[0]=p3[2]=0, res[1]=p3[0]=1, res[2]=p3[1]=2 -> identity */
		compose_permutation(p3, q3, res3, 3);
		ASSERT(is_identity_permutation(res3, 3, 0) == 1,
			"compose_permutation: p3 o q3 = identity (mutual inverses)");
		/* id3 o q3 = q3 */
		compose_permutation(id3, q3, res3b, 3);
		ASSERT(res3b[0] == q3[0] && res3b[1] == q3[1] && res3b[2] == q3[2],
			"compose_permutation: id o q3 = q3");
		/* q3 o id3 = q3 */
		compose_permutation(q3, id3, res3b, 3);
		ASSERT(res3b[0] == q3[0] && res3b[1] == q3[1] && res3b[2] == q3[2],
			"compose_permutation: q3 o id = q3");
	}
	TEST_GROUP("count_breakpoints");
	{
		/* Identity {0,1,2,3,4}: 0 breakpoints */
		ASSERT(count_breakpoints(id5, 5) == 0, "count_breakpoints: identity
			-> 0");
		/* Fully reversed {4,3,2,1,0}: 6 breakpoints (all positions) */
		ASSERT(count_breakpoints(rev5, 5) == 6,
			"count_breakpoints: fully reversed -> 6");
		/* {0,1,3,2,4}: breakpoints at (1,3),(3,2),(2,4) -> 3 */
		ASSERT(count_breakpoints(bp3, 5) == 3,
			"count_breakpoints: one adjacent swap -> 3");
		/* Single element identity */
		ASSERT(count_breakpoints(single, 1) == 0,
			"count_breakpoints: n=1 identity -> 0");
	}
	TEST_GROUP("find_value");
	{
		/* arr = {0,1,2,3,4} */
		found = find_value(arr, 5, 3);
		ASSERT(found == 3, "find_value: value 3 is at index 3");
		found = find_value(arr, 5, 0);
		ASSERT(found == 0, "find_value: value 0 is at index 0");
		found = find_value(arr, 5, 4);
		ASSERT(found == 4, "find_value: value 4 is at index 4");
		found = find_value(arr, 5, 99);
		ASSERT(found == -1, "find_value: missing value returns -1");
	}
	TEST_GROUP("first_breakpoint_position");
	{
		/* Identity: no breakpoint -> -1 */
		pos = first_breakpoint_position(id5, 5);
		ASSERT(pos == -1, "first_breakpoint_position: identity -> -1");
		/* {0,1,3,2,4}: first breakpoint between index 1 and 2 */
		pos = first_breakpoint_position(bp3, 5);
		ASSERT(pos == 1, "first_breakpoint_position: {0,1,3,2,4}
			-> breakpoint at index 1");
		/* {4,3,2,1,0}: first breakpoint at index 0 */
		pos = first_breakpoint_position(rev5, 5);
		ASSERT(pos == 0, "first_breakpoint_position: reversed
			-> breakpoint at index 0");
	}
	TEST_GROUP("rev_array_int");
	{
		/* Reverse entire array in place: arr={0,1,2,3,4} -> {4,3,2,1,0} */
		rev_array_int(ra, ra + 4);
		ASSERT(ra[0] == 4 && ra[1] == 3 && ra[2] == 2 && ra[3] == 1
			&& ra[4] == 0, "rev_array_int: full reversal {0,1,2,3,4}
			-> {4,3,2,1,0}");
		/* Reverse suffix [1..3]: {0,1,2,3,4} -> {0,3,2,1,4} */
		rev_array_int(rb + 1, rb + 3);
		ASSERT(rb[0] == 0 && rb[1] == 3 && rb[2] == 2 && rb[3] == 1
			&& rb[4] == 4, "rev_array_int: suffix [1..3] -> {0,3,2,1,4}");
		/* Single element: no change */
		rev_array_int(rc + 1, rc + 1);
		ASSERT(rc[0] == 7 && rc[1] == 8 && rc[2] == 9,
			"rev_array_int: single element -> unchanged");
	}
	TEST_GROUP("next_permutation");
	{
		/* {0,1,2} -> next = {0,2,1} */
		ret = next_permutation(np, 3);
		ASSERT(ret == 1, "next_permutation({0,1,2}): returns 1");
		ASSERT(np[0] == 0 && np[1] == 2 && np[2] == 1,
			"next_permutation({0,1,2}) = {0,2,1}");
		/* {0,2,1} -> next = {1,0,2} */
		ret = next_permutation(np, 3);
		ASSERT(ret == 1, "next_permutation({0,2,1}): returns 1");
		ASSERT(np[0] == 1 && np[1] == 0 && np[2] == 2,
			"next_permutation({0,2,1}) = {1,0,2}");
		/* {2,1,0} is the last permutation: returns 0 */
		ret = next_permutation(last, 3);
		ASSERT(ret == 0, "next_permutation({2,1,0}): returns 0 (last perm)");
		/* Exhaust all 3! = 6 permutations of {0,1,2}: count = 5 transitions */
		steps = 0;
		while (next_permutation(counter, 3))
			steps++;
		ASSERT(steps == 5,
			"next_permutation: exactly 5 transitions through all 3! permutations");
	}
}
