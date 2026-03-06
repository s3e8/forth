BUILTIN(PRINT_STACK, 
{
    printf("[ ps ]\n");

    printf("stack: [ ");
    for (cell* p = ds; p < s0; p++)
        printf("%ld ", (long)*p);
    printf("]\n");
})

BUILTIN(PRINT_CURRENT_WORD,
{

})

BUILTIN(PRINT_CURRENT_LINE,
{
    
})

BUILTIN(TODO,
{
    printf("[ ps ]\n");
    printf("word not implemented: %s\n", current_word);
    return;
})