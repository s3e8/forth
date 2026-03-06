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

// BUILTIN(SUB,  { tmp = POP(); AT(0) -= tmp; })
// BUILTIN(ADD,  { tmp = POP(); AT(0) += tmp; })
// BUILTIN(EQ,   { tmp = POP(); AT(0) = AT(0) == tmp; })