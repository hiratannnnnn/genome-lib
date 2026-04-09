#include "test_framework.h"
#include "lib.h"
#include <string.h>

void run_tests_strings(void)
{
    TEST_GROUP("ft_split");
    {
        /* Basic split */
        char **res = ft_split("hello world foo", " ");
        ASSERT(res != NULL, "ft_split: basic - non-NULL");
        ASSERT(strcmp(res[0], "hello") == 0, "ft_split: basic - first token");
        ASSERT(strcmp(res[1], "world") == 0, "ft_split: basic - second token");
        ASSERT(strcmp(res[2], "foo") == 0,   "ft_split: basic - third token");
        ASSERT(res[3] == NULL,               "ft_split: basic - NULL-terminated");
        for (int i = 0; res[i]; i++) xfree(res[i], strlen(res[i]) + 1);
        xfree(res, sizeof(char *) * 4);

        /* Multiple separator characters */
        char **res2 = ft_split("a,b;c,d", ",;");
        ASSERT(res2 != NULL, "ft_split: multi-sep - non-NULL");
        ASSERT(strcmp(res2[0], "a") == 0, "ft_split: multi-sep - token a");
        ASSERT(strcmp(res2[1], "b") == 0, "ft_split: multi-sep - token b");
        ASSERT(strcmp(res2[2], "c") == 0, "ft_split: multi-sep - token c");
        ASSERT(strcmp(res2[3], "d") == 0, "ft_split: multi-sep - token d");
        ASSERT(res2[4] == NULL,           "ft_split: multi-sep - NULL-terminated");
        for (int i = 0; res2[i]; i++) xfree(res2[i], strlen(res2[i]) + 1);
        xfree(res2, sizeof(char *) * 5);

        /* Leading and trailing separators */
        char **res3 = ft_split(",,apple,,banana,,", ",");
        ASSERT(res3 != NULL, "ft_split: leading/trailing sep - non-NULL");
        ASSERT(strcmp(res3[0], "apple") == 0,  "ft_split: leading/trailing - first token");
        ASSERT(strcmp(res3[1], "banana") == 0, "ft_split: leading/trailing - second token");
        ASSERT(res3[2] == NULL,                "ft_split: leading/trailing - NULL-terminated");
        for (int i = 0; res3[i]; i++) xfree(res3[i], strlen(res3[i]) + 1);
        xfree(res3, sizeof(char *) * 3);

        /* Single token (no separator in string) */
        char **res4 = ft_split("onlyone", ",");
        ASSERT(res4 != NULL, "ft_split: single token - non-NULL");
        ASSERT(strcmp(res4[0], "onlyone") == 0, "ft_split: single token - value");
        ASSERT(res4[1] == NULL,                  "ft_split: single token - NULL-terminated");
        for (int i = 0; res4[i]; i++) xfree(res4[i], strlen(res4[i]) + 1);
        xfree(res4, sizeof(char *) * 2);

        /* All separators (no tokens) */
        char **res5 = ft_split(",,,,", ",");
        ASSERT(res5 != NULL, "ft_split: all-sep - non-NULL");
        ASSERT(res5[0] == NULL, "ft_split: all-sep - immediately NULL-terminated");
        xfree(res5, sizeof(char *) * 1);
    }

    TEST_GROUP("ft_strtrim");
    {
        /* Trim spaces */
        char *r1 = ft_strtrim("  hello  ", " ");
        ASSERT(r1 != NULL, "ft_strtrim: returns non-NULL");
        ASSERT(strcmp(r1, "hello") == 0, "ft_strtrim: trims spaces both sides");
        free(r1);

        /* Trim multiple characters */
        char *r2 = ft_strtrim("--hello-world--", "-");
        ASSERT(strcmp(r2, "hello-world") == 0, "ft_strtrim: trims dashes both sides");
        free(r2);

        /* No trimming needed */
        char *r3 = ft_strtrim("hello", " ");
        ASSERT(strcmp(r3, "hello") == 0, "ft_strtrim: nothing to trim");
        free(r3);

        /* Entire string is trim chars */
        char *r4 = ft_strtrim("   ", " ");
        ASSERT(r4 != NULL, "ft_strtrim: all-trim-chars returns non-NULL");
        ASSERT(strlen(r4) == 0, "ft_strtrim: all-trim-chars yields empty string");
        free(r4);

        /* Empty string */
        char *r5 = ft_strtrim("", " ");
        ASSERT(r5 != NULL, "ft_strtrim: empty input returns non-NULL");
        ASSERT(strlen(r5) == 0, "ft_strtrim: empty input yields empty string");
        free(r5);

        /* Only left side needs trimming */
        char *r6 = ft_strtrim("   hello", " ");
        ASSERT(strcmp(r6, "hello") == 0, "ft_strtrim: trims left side only");
        free(r6);

        /* Only right side needs trimming */
        char *r7 = ft_strtrim("hello   ", " ");
        ASSERT(strcmp(r7, "hello") == 0, "ft_strtrim: trims right side only");
        free(r7);

        /* Multiple different trim chars */
        char *r8 = ft_strtrim("xyzABCxyz", "xyz");
        ASSERT(strcmp(r8, "ABC") == 0, "ft_strtrim: trims multiple chars from set");
        free(r8);
    }
}
