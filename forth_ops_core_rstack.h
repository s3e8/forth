BUILTIN(FROMR,
{
    printf("[ r> ]\n");

    tmp = (cell)POPRS();
    PUSH(tmp);    
})

BUILTIN(TOR,
{
    printf("[ >r ]\n");

    tmp = POP();
    PUSHRS(tmp);   
})

BUILTIN(RFETCH,
{
    printf("[ r@ ]\n");

    PUSH((cell)*rs);  // peek at top of return stack, don't pop
})