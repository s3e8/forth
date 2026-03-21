BUILTIN(FETCH,
{
    // printf("[ @ ]\n");

    cell* addr  = (cell*)POP();
    cell  val = *addr;
    PUSH(val);
})

BUILTIN(STORE,
{
    // printf("[ ! ]\n");

    cell* addr = (cell*)POP();
    cell  val   = POP();

    *addr = val;
})

BUILTIN(CSTORE,
{
    // printf("[ c! ]\n");

    address  = (char*)POP();
    value    = POP();

    *address = (char)value;
})

BUILTIN(CFETCH,
{
    // printf("[ c@ ]\n");

    address = (char*)POP();
    value   = *address;
    PUSH(value);
})

BUILTIN(MEMADD,
{
    cell* addr = (cell*)POP();
    tmp = POP();
    *addr += tmp;
})




BUILTIN(MEM_SUB,
{
    cell* addr = (cell*)POP();
    tmp = POP();
    *addr -= tmp;
})





BUILTIN(FCOMMA,
{
    float val = FPOP();
    *(float*)here = val;
    here += sizeof(cell);
})

BUILTIN(FSTORE,
{
    float* ptr = (float*)POP();
    float val = FPOP();
    *ptr = val;
})

BUILTIN(FFETCH,
{
    float* ptr = (float*)POP();
    FPUSH(*ptr);
})



BUILTIN(VAR_AT,
{
    cell* ptr = (cell*)ARG();
    PUSH(*ptr);
})

BUILTIN(VAR_TO, //todo: ??
{
    cell* ptr = (cell*)ARG();
    *ptr = POP();
})

BUILTIN(FVAR_AT,
{
    float* ptr = (float*)ARG();
    FPUSH(*ptr);
})

BUILTIN(FVAR_TO,
{
    float *ptr = (float*)ARG();
    *ptr = FPOP();
})

BUILTIN(SVAR_AT,
{
    short *ptr = (short*)ARG();
    PUSH(*ptr);
})

BUILTIN(SVAR_TO,
{
    short *ptr = (short*)ARG();
    *ptr = (short)POP();
})



BUILTIN(SSTORE,
{
    short *ptr = (short*)POP();
    tmp = POP();
    *ptr = (short)tmp;
})

BUILTIN(SFETCH,
{
    short *ptr = (short*)POP();
    PUSH(*ptr);
})

BUILTIN(BYTE_COPY,
{
    char* src = (char*)AT(1);
    char* dst = (char*)AT(0);

    *dst++ = *src++;

    AT(0) = (cell)dst;
    AT(1) = (cell)src;
})