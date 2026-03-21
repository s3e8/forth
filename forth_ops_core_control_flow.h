BUILTIN(0BRANCH,
{
    // printf("[ 0branch ]\n");

    tmp = INTARG();

    if (!POP()) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(1BRANCH,
{
    tmp = INTARG();

    if (POP()) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(LTBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a < b) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(GTBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a > b) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(LTEBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a <= b) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(GTEBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a >= b) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(LTZBRANCH,
{
    tmp = INTARG();
    cell a = POP();
    if (a < 0) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(GTZBRANCH,
{
    tmp = INTARG();
    cell a = POP();
    if (a > 0) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(LTEZBRANCH,
{
    tmp = INTARG();
    cell a = POP();
    if (a <= 0) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(GTEZBRANCH,
{
    tmp = INTARG();
    cell a = POP();
    if (a >= 0) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(EQBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a == b) ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(NEQBRANCH,
{
    tmp = INTARG();
    cell b = POP();
    cell a = POP();
    if (a != b) ip += (tmp / sizeof(void*)) - 1;
})

// BYTECODE(NEQBRANCH, "<>branch", 2, 0, FLAG_HASARG, {
//     tmp = INTARG();
//     cell b = POP();
//     cell a = POP();
//     if(a!=b) ip += (tmp/sizeof(void