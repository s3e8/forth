BUILTIN(FIND,
{
    printf("[ find ]\n");
    
    char* wordname = (char*)POP();
    PUSH(find(wordname));
})

