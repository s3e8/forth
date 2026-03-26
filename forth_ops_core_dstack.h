BUILTIN(DUP,
{
    // printf("[ dup ]\n");

    cell value = TOP();
    PUSH(value);
})

BUILTIN(SWAP,
{
    tmp = AT(1);
    AT(1) = AT(0);
    AT(0) = tmp;
})

BUILTIN(DROP,
{
    // printf("[ drop ]\n");
    ds++;
})

BUILTIN(OVER,
{
    // printf("[ over ]\n");

    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(NIP,
{
    // printf("[ nip ]\n");

    AT(1) = AT(0);
    ds++;
})

BUILTIN(TUCK,
{
    // printf("[ tuck ]\n");

    tmp = AT(1);
    AT(1) = AT(0);
    AT(0) = tmp;
    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(ROT,
{
    // printf("[ rot ]\n");

    cell eax = POP();
    cell ebx = POP();
    cell ecx = POP();
    PUSH(ebx);
    PUSH(eax);
    PUSH(ecx);
})

BUILTIN(MROT,
{
    // printf("[ -rot ]\n");

    cell eax = POP();
    cell ebx = POP();
    cell ecx = POP();
    PUSH(eax);
    PUSH(ecx);
    PUSH(ebx);
})

BUILTIN(2DUP,
{
    // printf("[ 2dup ]\n");

    tmp = AT(1);
    PUSH(tmp);
    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(2DROP,
{
    // printf("[ 2drop ]\n");

    ds += 2;
})

BUILTIN(CONDDUP,
{
    // printf("[ ?dup ]\n");

    if (TOP()) PUSH(TOP());  // dup only if nonzero
})

// BUILTIN(DUP,  { tmp = TOP(); PUSH(tmp); })
// BUILTIN(SWAP, { a = POP(); b = POP(); PUSH(a); PUSH(b); })
// BUILTIN(DROP, { ds++; })
// BUILTIN(OVER, { tmp = AT(1); PUSH(tmp); })

BUILTIN(DSP_FETCH,
{
    // printf("[ dsp@ ]\n");

    tmp = (cell)ds;
    PUSH(tmp);
})

BUILTIN(DSP_STORE,
{
    cell *newds = (cell*)POP();
    ds = newds;
})







BUILTIN(RSP_GET, { PUSH(rs); })
BUILTIN(RSP_PUT, { rs = (void***)POP(); })

BUILTIN(GET_T0,   { PUSH(ts); })
BUILTIN(SET_T0,   {    ts = (cell*)POP(); })
BUILTIN(TO_TMP,   { *--ts =        POP(); })
BUILTIN(FROM_TMP, { PUSH(*ts++); })

BUILTIN(GET_FS, { PUSH(fs); })
BUILTIN(SET_FS, { fs = (float*)POP(); })



BUILTIN(FLT,
{
    float a = FPOP();
    float b = FPOP();
    FPUSH(a < b);
})
BUILTIN(FGT,
{
    float a = FPOP();
    float b = FPOP();
    FPUSH(a > b);
})

BUILTIN(FLTE,
{
    float a = FPOP();
    float b = FPOP();
    FPUSH(a <= b);
})
BUILTIN(FGTE,
{
    float a = FPOP();
    float b = FPOP();
    FPUSH(a >= b);
})





BUILTIN(FDUP,
{
    float val = FTOP();
    FPUSH(val);
})

BUILTIN(FDUP2, // todo: 2FDUP... duhhh.. right?
{
    float val = FAT(1);
    FPUSH(val);
    val = FAT(1);
    FPUSH(val);
})

BUILTIN(FDUPVEC,
{
    float val = FAT(2);
    FPUSH(val);
    val = FAT(2);
    FPUSH(val);
    val = FAT(2);
    FPUSH(val);
})

BUILTIN(DUP_AT,
{
    cell *addr = (cell*)TOP();
    PUSH(*addr);
})

BUILTIN(FNIP,   { FAT(1) = FAT(0); fs++;      })
BUILTIN(2NIP,   {  AT(2) =  AT(0); ds += 2;   })
BUILTIN(2FNIP,  { FAT(2) = FAT(0); fs += 2;   })
BUILTIN(F2DUP,
{
    tmp = AT(1);
    PUSH(tmp);
    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(COND_DUP, {
    tmp = TOP();
    if(tmp) PUSH(tmp);
})

BUILTIN(FSWAP,
{
    float tmp = FAT(1);
    FAT(1) = FAT(0);
    FAT(0) = tmp;
})

BUILTIN(SWAPDUP,
{
    tmp = AT(1);
    AT(1) = AT(0);
    AT(0) = tmp;
    PUSH(tmp);
})

BUILTIN(FDROP, { ++fs; })

BUILTIN(FROT,
{
    float eax = FPOP();
    float ebx = FPOP();
    float ecx = FPOP();
    FPUSH(ebx);
    FPUSH(eax);
    FPUSH(ecx);
})

BUILTIN(FMROT,
{
    float eax = FPOP();
    float ebx = FPOP();
    float ecx = FPOP();
    FPUSH(eax);
    FPUSH(ecx);
    FPUSH(ebx);
})
