BUILTIN(FROMR,
{
    // printf("[ r> ]\n");

    tmp = (cell)POPRS();
    PUSH(tmp);
})

BUILTIN(TOR,
{
    // printf("[ >r ]\n");

    tmp = POP();
    PUSHRS(tmp);
})

BUILTIN(RFETCH,
{
    // printf("[ r@ ]\n");

    PUSH((cell)*rs);  // peek at top of return stack, don't pop
})

BUILTIN(2FROMR,
{
    tmp = (cell)POPRS();
    PUSH(tmp);
    tmp = (cell)POPRS();
    PUSH(tmp);
})

BUILTIN(2TOR,
{
    tmp = POP();
    PUSHRS(tmp);
    tmp = POP();
    PUSHRS(tmp);
})


BUILTIN(RDROP,  { rs++; })
BUILTIN(2RDROP, { rs += 2; })


BUILTIN(RS_PUT, { rs = (void***)POP(); })
BUILTIN(RS_GET, { PUSHRS(rs); })












