BUILTIN(EXECUTE, // exec-word
{
    printf("[ execute ]\n");

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
    printf("[ jump ]\n");

    void *fn = ARG();
    ip = fn;
}) // todo: same as execute.. where does this belong?

BUILTIN(IWORD,
{
    printf("[ iword ]\n");

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