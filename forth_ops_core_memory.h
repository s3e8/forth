BUILTIN(FETCH,
{
    printf("[ @ ]\n");

    cell* addr  = (cell*)POP();
    cell  val = *addr;
    PUSH(val);
})

BUILTIN(STORE,
{
    printf("[ ! ]\n");

    cell* addr = (cell*)POP();
    cell  val   = POP();

    *addr = val;
})

BUILTIN(CSTORE,
{
    printf("[ c! ]\n");

    address  = (char*)POP();
    value    = POP();
    
    *address = (char)value;
})

BUILTIN(CFETCH,
{
    printf("[ c@ ]\n");

    address = (char*)POP();
    value   = *address;
    PUSH(value);
})