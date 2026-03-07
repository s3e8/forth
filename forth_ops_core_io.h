BUILTIN(DOT, {
    printf("[ . ]\n");

    cell val = POP();
    printf("%ld\n", (long)val);
})

BUILTIN(WORD,
{
    printf("[ word ]\n");

    PUSH(get_next_word(wordbuf));
})

BUILTIN(KEY,
{
    printf("[ key ]\n");

    PUSH(key());
})

BUILTIN(EMIT,
{
    printf("[ emit ]\n");

    emit(POP());
})

BUILTIN(TELL,
{
    printf("[ tell ]\n");

    fputs((char*)POP(), output_stream);
})

// BYTECODE(PROMPT, "prompt", 2, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     char *prompt = (char*)POP();
//     prompt_line(prompt, state);
//   })


// BYTECODE(OPENFILE, "open-file", 2, 0, 0, {
//     char *mode = (char*)POP();
//     char *fn = (char*)POP();
//     PUSH(open_file(fn, mode));
//   })
// BYTECODE(CLOSEFILE, "close-file", 1, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     close_file(state);    
//   })