BUILTIN(LATEST,
{
    printf("[ latest ]\n");
    
    PUSH(&latest);
})

BUILTIN(CREATE,
{
    printf("[ create ]\n");

    create((char*)POP(), 0);
})

BUILTIN(FIND,
{
    printf("[ find ]\n");
    
    char* wordname = (char*)POP();
    PUSH(find(wordname));
})

BUILTIN(VARIABLE, {
    
    printf("[ variable ]\n");

    char* name = get_next_word(reader_state, current_word); // todo: or wordbuf
    if (!name)
    {
        printf("Error: No name for variable\n");
        NEXT();
    }

    printf("var name: %s\n", name);

    defvar(name, (cell)NULL); // todo: change to 0???
})

BUILTIN(HIDDEN,
{
    word_header_t* hdr = (word_header_t*)POP();
    hdr->flags ^= FLAG_HIDDEN;    
})