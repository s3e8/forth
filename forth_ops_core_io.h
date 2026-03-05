BUILTIN(DOT, {
    printf("[ . ]\n");

    cell val = POP();
    printf("%ld\n", (long)val);
})

BUILTIN(WORD,
{
    printf("[ word ]\n");

    PUSH(get_next_word());
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
    printf("[ emit ]\n");

    fputs((char*)POP());
})

// BYTECODE(PROMPT, "prompt", 2, 0, 0, {
//     reader_state_t *state = (reader_state_t*)POP();
//     char *prompt = (char*)POP();
//     prompt_line(prompt, state);
//   })