BUILTIN(DUP,
{
    printf("[ dup ]\n");

    cell value = TOP();
    PUSH(value);
})

BUILTIN(SWAP,
{
    printf("[ swap ]\n");
    
    // BYTECODE(SWAP, "swap", 2, 0, 0, {
    //     tmp = AT(1);
    //     AT(1) = AT(0);
    //     AT(0) = tmp;
    // })

    //  // Stack: ( a b -- b a )
    // // Access stack items directly

    // cell b = AT(-1);     // Top item (b)
    // cell a = AT(-2);     // Second item (a)

    // // Swap them in place
    // AT(-1) = a;
    // AT(-2) = b;

    // print_stack(ds, s0);

    // cell b = POP();      // Remove top value (b)
    // cell a = POP();      // Remove next value (a)

    // PUSH(b);             // Push b first
    // PUSH(a);             // Then push a

    tmp = AT(1);
    AT(1) = AT(0);
    AT(0) = tmp;
})

BUILTIN(DROP, 
{ 
    printf("[ drop ]\n");
    ds++; 
})

BUILTIN(OVER, 
{ 
    printf("[ over ]\n");

    tmp = AT(1); 
    PUSH(tmp); 
})

BUILTIN(NIP,
{
    printf("[ nip ]\n");

    AT(1) = AT(0); 
    ds++;
})

BUILTIN(TUCK,
{
    printf("[ tuck ]\n");

    tmp = AT(1);
    AT(1) = AT(0);
    AT(0) = tmp;
    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(ROT,
{
    printf("[ rot ]\n");

    cell eax = POP();
    cell ebx = POP();
    cell ecx = POP();
    PUSH(ebx);
    PUSH(eax);
    PUSH(ecx);  
})

BUILTIN(MROT,
{
    printf("[ -rot ]\n");
    
    cell eax = POP();
    cell ebx = POP();
    cell ecx = POP();
    PUSH(eax);
    PUSH(ecx);
    PUSH(ebx);    
})

BUILTIN(2DUP,
{
    printf("[ 2dup ]\n");

    tmp = AT(1);
    PUSH(tmp);
    tmp = AT(1);
    PUSH(tmp);
})

BUILTIN(2DROP,
{
    printf("[ 2drop ]\n");

    ds += 2;
})

BUILTIN(CONDDUP,
{
    printf("[ ?dup ]\n");

    if (TOP()) PUSH(TOP());  // dup only if nonzero
})

// BUILTIN(DUP,  { tmp = TOP(); PUSH(tmp); })
// BUILTIN(SWAP, { a = POP(); b = POP(); PUSH(a); PUSH(b); })
// BUILTIN(DROP, { ds++; })
// BUILTIN(OVER, { tmp = AT(1); PUSH(tmp); })