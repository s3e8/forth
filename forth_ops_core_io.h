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