BUILTIN(ADD,
{
    printf("[ + ]\n");

    tmp = POP();
    AT(0) += tmp;

    // cell a = POP();
    // cell b = POP();
    // PUSH(a+b);
})

BUILTIN(SUB,
{
    printf("[ - ]\n");

    tmp = POP();
    AT(0) -= tmp;   
})

BUILTIN(MUL,
{
    printf("[ * ]\n");

    tmp = POP();
    AT(0) *= tmp;   
})

BUILTIN(DIV,
{
    printf("[ / ]\n");

    tmp = POP();
    AT(0) /= tmp;  
})

BUILTIN(MOD,
{
    printf("[ mod ]\n");

    tmp = POP();
    AT(0) = AT(0) % tmp;    
})

BUILTIN(DIVMOD,
{
    printf("[ /mod ]\n");

    cell a = POP();
    cell b = POP();
    PUSH( b % a );
    PUSH( b / a );   
})

BUILTIN(ADD1,
{
    printf("[ 1+ ]\n");

    AT(0) += 1;
})

BUILTIN(SUB1,
{
    printf("[ 1+ ]\n");

    AT(0) -= 1;
})

BUILTIN(EQ,
{
    printf("[ = ]\n");

    tmp = POP(); 
    AT(0) = AT(0) == tmp;
})

BUILTIN(GT,
{
    printf("[ > ]\n");

    tmp = POP();
    AT(0) = AT(0) > tmp;   
})

BUILTIN(LT,
{
    printf("[ < ]\n");

    tmp = POP();
    AT(0) = AT(0) > tmp;   
})

BUILTIN(LTE,
{
    printf("[ <= ]\n");

    tmp = POP();
    AT(0) = AT(0) <= tmp;   
})

BUILTIN(GTE,
{
    printf("[ >= ]\n");

    tmp = POP();
    AT(0) = AT(0) >= tmp;   
})

BUILTIN(EQZ,
{
    printf("[ 0= ]\n");

    AT(0) = AT(0)==0;
})

BUILTIN(GTZ,
{
    printf("[ 0> ]\n");

    AT(0) = AT(0) > 0;
})

BUILTIN(GTZ,
{
    printf("[ 0< ]\n");

    AT(0) = AT(0) < 0;
})

BUILTIN(GTEZ,
{
    printf("[ 0>= ]\n");

    AT(0) = AT(0) >= 0;
})

BUILTIN(GTEZ,
{
    printf("[ 0<= ]\n");

    AT(0) = AT(0) <= 0;
})


BUILTIN(NEQZ,
{
    printf("[ 0<> ]\n");

    AT(0) = AT(0)!=0;
})

// BUILTIN(SUB,  { tmp = POP(); AT(0) -= tmp; })
// BUILTIN(ADD,  { tmp = POP(); AT(0) += tmp; })
// BUILTIN(EQ,   { tmp = POP(); AT(0) = AT(0) == tmp; })




// bitwise //
BUILTIN(AND,
{
    printf("[ and ]\n");
    
    tmp = POP();
    AT(0) &= tmp;   
})

BUILTIN(OR,
{
    printf("[  or ]\n");

    tmp = POP();
    AT(0) |= tmp;
})

BUILTIN(XOR,
{
    printf("[ xor ]\n");

    tmp = POP();
    AT(0) ^= tmp; 
})

BUILTIN(INVERT,
{
    printf("[ invert ]\n");

    AT(0) = ~AT(0); 
})