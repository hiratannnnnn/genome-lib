gcc -Wall -Wextra -Werror -Iincludes all_check.c solve.c approx_sbpbi.c -Ldist -lgenome -lm -o all_check

gcc -Wall -Wextra -Werror -Iincludes check.c solve.c approx_sbpbi.c -Ldist -lgenome -lm -o check

cc make_input.c -Iincludes -Ldist -lm -lgenome -o make_input

cat input | xargs ./check
