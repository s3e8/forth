// from here to EXIT are stuff originally included in forth.c... stuff in the core inner to bootstrap.. some later to be replaces
BUILTIN(DODOES,
{

})

BUILTIN(DOES,
{

})

BUILTIN(DOCOL, 
{
    code = tick(header); // todo: rename code to xt?
    *--nestingstack = ip;

    if (header->flags & FLAG_BUILTIN)
    {
        code_immediatebuf[0] = code; // todo: or just put tick function here?
        ip = code_immediatebuf;
    } 
    else {
        word_immediatebuf[1] = code;
        ip = word_immediatebuf;
    }
})

BUILTIN(INTERPRET,
{
    // printf("[ interpret ]\n");
    if (!get_next_word(reader_state, current_word))
    {
        if (is_eof(reader_state) && reader_state->stream != stdin) 
            reader_state->stream = stdin; // todo: this still fires even if stream == stdin
            
        NEXT();
    }

    header = NULL;
    header = find(current_word);

    if (!header)
    {
        char* endptr = NULL;
        cell number = (cell)strtol(current_word, &endptr, (int)base);

        if (*endptr != '\0')
        {
            printf("unknown word: %s\n", current_word);
            NEXT();
        }
        else {
            if (state == STATE_COMPILE)
            {
                comma((cell)CODE(LIT));
                comma((cell)number);
            }
            else if (state == STATE_IMMEDIATE) { PUSH(number); }
            else { printf("error: Compiler state out of bounds. Should be either 0 or 1.\n"); return; }
        }
        
        NEXT();
    }

    if (state == STATE_COMPILE && !(header->flags & FLAG_IMMEDIATE))
    { // todo: use xt 'register'? 
        if (header->flags & FLAG_BUILTIN) comma((cell)tick(header));
        else {
            comma((cell) CODE(CALL));
            comma((cell)tick(header)); // todo: slarba for some reason uses CALL here... but CALL is already in word_immediatebuf...
        }
    }
    else { /* state == STATE_IMMEDIATE || word->flags & FLAG_IMMEDIATE */
        goto OP(DOCOL);
    }         
})

BUILTIN(BRANCH,
{
    // printf("[ branch ]\n");
    tmp = INTARG();
    ip += (tmp / sizeof(void*)) - 1;
})

BUILTIN(CALL,
{
    // printf("[ call ]\n");
    fn = ARG();
    PUSHRS(ip);
    ip = fn;
})

BUILTIN(LIT, 
{
    // printf("[ lit ]\n");
    PUSH(INTARG());
})

BUILTIN(EOW, {})

BUILTIN(IRETURN,
{
    // printf("[ ireturn ]\n");
    ip = *nestingstack++;
})

BUILTIN(EXIT,
{
    // printf("[ exit ]\n");
    ip = POPRS();
})



BUILTIN(EXECUTE, // exec-word
{
    // printf("[ execute ]\n");

    PUSHRS(ip);
    ip = (void**)POP();
}) // todo: this is not an inner word... then what is it?

// BYTECODE(BUITINEXEC, "exec-builtin", 1, 0, 0, {
//     builtin_immediatebuf[0] = (void*)POP();
//     *--nestingstack = ip;
//     ip = builtin_immediatebuf;
// })

BUILTIN(JUMP,
{
    // printf("[ jump ]\n");

    void *fn = ARG();
    ip = fn;
}) // todo: same as execute.. where does this belong?

BUILTIN(IWORD,
{
    // printf("[ iword ]\n");

    PUSH(get_next_word(reader_state, current_word));
})

// // todo: this is not for inner I dont think? 
// BUILTIN(IEXECUTE,
// {
//     printf("[ iexecute ]\n");
//     word_header_t* entry = (word_header_t*)POP();
//     CODE(DOCOL);
// })



// BYTECODE(ISEOF, "?eof", 1, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     PUSH(is_eof(state));
//   })
// BYTECODE(ISEOL, "?eol", 1, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     PUSH(is_eol(state));
//   })

// BYTECODE(PARSENUM, "number", 1, 0, 0, {
//     char *endptr = NULL;
//     char *str = (char*)POP();
//     cell val = (cell)strtol(str, &endptr, base);
//     if(*endptr!='\0') {
//       PUSH(0);
//     } else {
//       PUSH(val);
//       PUSH(1);
//     }
//   })