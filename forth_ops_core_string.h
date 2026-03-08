BUILTIN(STRCAT,
{
    printf("[ strcat ]\n");

    char *dst = (char*)POP();
    char *src = (char*)POP();
    PUSH(strcat(dst, src));
})

BUILTIN(STRLEN,
{
    printf("[ strlen ]\n");

    char *str = (char*)POP();
    PUSH(strlen(str));
})

BUILTIN(STRNCPY,
{
    printf("[ strncpy ]\n");

    tmp = POP();
    char *dst = (char*)POP();
    char *src = (char*)POP();
    PUSH(strncpy(dst, src, tmp));  
})

BUILTIN(STRCPY,
{
    printf("[ strcpy ]\n");

    char *dst = (char*)POP();
    char *src = (char*)POP();
    PUSH(strcpy(dst, src));
})

BUILTIN(STRCMP,
{
    printf("[ strcmp ]\n");

    char *b = (char*)POP();
    char *a = (char*)POP();
    PUSH(strcmp(a,b));
})

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