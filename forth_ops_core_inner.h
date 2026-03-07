BUILTIN(EXECUTE, // exec-word
{
    printf("[ execute ]\n");

    PUSHRS(ip);
    ip = (void**)POP();
}) // todod: this is not an inner word... then what is it?



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