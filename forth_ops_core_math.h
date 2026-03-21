BUILTIN(ADD,
{
    // printf("[ + ]\n");

    tmp = POP();
    AT(0) += tmp;

    // cell a = POP();
    // cell b = POP();
    // PUSH(a+b);
})

BUILTIN(SUB,
{
    // printf("[ - ]\n");

    tmp = POP();
    AT(0) -= tmp;
})

BUILTIN(MUL,
{
    // printf("[ * ]\n");

    tmp = POP();
    AT(0) *= tmp;
})

BUILTIN(DIV,
{
    // printf("[ / ]\n");

    tmp = POP();
    AT(0) /= tmp;
})

BUILTIN(MOD,
{
    // printf("[ mod ]\n");

    tmp = POP();
    AT(0) = AT(0) % tmp;
})

BUILTIN(DIVMOD,
{
    // printf("[ /mod ]\n");

    cell a = POP();
    cell b = POP();
    PUSH( b % a );
    PUSH( b / a );
})

BUILTIN(ADD1,
{
    // printf("[ 1+ ]\n");

    AT(0) += 1;
})

BUILTIN(SUB1,
{
    // printf("[ 1+ ]\n");

    AT(0) -= 1;
})

BUILTIN(EQ,
{
    // printf("[ = ]\n");

    tmp = POP();
    AT(0) = AT(0) == tmp;
})

BUILTIN(NEQ,
{
    // printf("[ <> ]\n");

    tmp = POP();
    AT(0) = AT(0) != tmp;
})

BUILTIN(GT,
{
    // printf("[ > ]\n");

    tmp = POP();
    AT(0) = AT(0) > tmp;
})

BUILTIN(LT,
{
    // printf("[ < ]\n");

    tmp = POP();
    AT(0) = AT(0) > tmp;
})

BUILTIN(LTE,
{
    // printf("[ <= ]\n");

    tmp = POP();
    AT(0) = AT(0) <= tmp;
})

BUILTIN(GTE,
{
    // printf("[ >= ]\n");

    tmp = POP();
    AT(0) = AT(0) >= tmp;
})

BUILTIN(EQZ,
{
    // printf("[ 0= ]\n");

    AT(0) = AT(0)==0;
})

BUILTIN(GTZ,
{
    // printf("[ 0> ]\n");

    AT(0) = AT(0) > 0;
})

BUILTIN(LTZ,
{
    // printf("[ 0< ]\n");

    AT(0) = AT(0) < 0;
})

BUILTIN(GTEZ,
{
    // printf("[ 0>= ]\n");

    AT(0) = AT(0) >= 0;
})

BUILTIN(LTEZ,
{
    // printf("[ 0<= ]\n");

    AT(0) = AT(0) <= 0;
})


BUILTIN(NEQZ,
{
    // printf("[ 0<> ]\n");

    AT(0) = AT(0)!=0;
})

// BUILTIN(SUB,  { tmp = POP(); AT(0) -= tmp; })
// BUILTIN(ADD,  { tmp = POP(); AT(0) += tmp; })
// BUILTIN(EQ,   { tmp = POP(); AT(0) = AT(0) == tmp; })

BUILTIN(ABS,
{
    // printf("[ abs ]\n");

    if ((cell)TOP() < 0) AT(0) = -AT(0);
})

BUILTIN(MIN,
{
    // printf("[ min ]\n");

    tmp = POP();
    if (tmp < TOP()) AT(0) = tmp;
})

BUILTIN(MAX,
{
    // printf("[ max ]\n");

    tmp = POP();
    if (tmp > TOP()) AT(0) = tmp;
})


// bitwise //
BUILTIN(AND,
{
    // printf("[ and ]\n");

    tmp = POP();
    AT(0) &= tmp;
})

BUILTIN(OR,
{
    // printf("[  or ]\n");

    tmp = POP();
    AT(0) |= tmp;
})

BUILTIN(XOR,
{
    // printf("[ xor ]\n");

    tmp = POP();
    AT(0) ^= tmp;
})

BUILTIN(NEG,
{
    // printf("[ invert ]\n");

    AT(0) = ~AT(0);
})

BUILTIN(LSHIFT,
{
    // printf("[ lshift ]\n");

    tmp = POP();
    AT(0) <<= tmp;
})

BUILTIN(RSHIFT,
{
    // printf("[ rshift ]\n");

    tmp = POP();
    AT(0) >>= tmp;
})




















BUILTIN(UDIVMOD,
{
    cell a = POP();
    cell b = POP();
    PUSH( (unsigned)b % (unsigned)a );
    PUSH( (unsigned)b / (unsigned)a );
})

BUILTIN(BI_ADD, // todo: purpose?
{
    tmp = POP();
    AT(0) += tmp;
    tmp = POP();
    AT(0) += tmp;
})

BUILTIN(U_LT,
{
    tmp = POP();
    AT(0) = (unsigned)AT(0) < (unsigned)tmp;
})

BUILTIN(U_GT,
{
    tmp = POP();
    AT(0) = (unsigned)AT(0) > (unsigned)tmp;
})

BUILTIN(U_LTE,
{
    tmp = POP();
    AT(0) = (unsigned)AT(0) >= (unsigned)tmp;
})

BUILTIN(U_GTE,
{
    tmp = POP();
    AT(0) = (unsigned)AT(0) >= (unsigned)tmp;
})


// */ -- ( n1 n2 n3 — n4 )
BUITIN(MUL_DIV,
{

})

// */MOD -- ( n1 n2 n3 — n4 n5 )
BUILTIN(MUL_DIV_MOD,
{
})
// +Loop
// leave
// page
// quit -- ( — ) Terminates execution for the current task and returns control to the terminal.