#include "debug_print.h"

#include <stdio.h>

void debug_print(char *msg, int v)
{
#ifdef DEBUG
    static int k;
    if (v <= 0)
        k -= 1;
    for (int i = 0; i < k; ++i)
        printf("\t");
    if (v > 0)
        k += 1;
    if (v == 2)
    {
        printf("- value - \"%s\"\n", msg);
        k -= 1;
    }
    else
    {
        printf("- %s%s\n", msg, v ? "" : " ok");
    }
#endif
    if (msg == NULL && v)
        msg = NULL;
    return;
}

void debug_print_int(int msg)
{
#ifdef DEBUG
    char tmp[128] = {0};
    sprintf(tmp, "%d", msg);
    debug_print(tmp, 2);
#endif
    if (msg == 1)
        msg = 1;
    return;
}

void debug_print_raw(char *msg)
{
#ifdef DEBUG
    printf("%s\n", msg);
#endif
    if (msg == NULL)
        msg = NULL;
    return;
}

void debug_print_raw_int(int msg)
{
#ifdef DEBUG
    printf("%d\n", msg);
#endif
    if (msg == 1)
        msg = 1;
    return;
}

void debug_print_raw_string_int(char *str, int v)
{
#ifdef DEBUG
    printf("%s %d\n", str, v);
#endif
    if (v == 1)
        v = 1;
    if (str == NULL)
        str = NULL;
    return;
}