BUILTIN(LATEST,
{
    printf("[ latest ]\n");
    
    PUSH(&latest);
})

BUILTIN(0BRANCH,
{
    printf("[ 0branch ]\n");

    tmp = INTARG();

    if (!POP()) ip += (tmp / sizeof(void*)) - 1;
})


// BYTECODE(OPENFILE, "open-file", 2, 0, 0, {
//     char *mode = (char*)POP();
//     char *fn = (char*)POP();
//     PUSH(open_file(fn, mode));    
//   })
// BYTECODE(CLOSEFILE, "close-file", 1, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     close_file(state);    
//   })
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