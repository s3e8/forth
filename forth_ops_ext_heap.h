
// BUILTIN(MALLOC,
// {
//     tmp = POP();
//     PUSH(MALLOC(tmp));
// })

// BUILTIN(MALLOC_ATOMIC,
// {
//     tmp = POP();
//     PUSH(MALLOC_ATOMIC(tmp));
// })

// BUILTIN(REALLOC,
// {
//     void *ptr = (void*)POP();
//     tmp = POP();
//     PUSH(REALLOC(ptr,tmp));
// })

// BUILTIN(RUN_GC, { RUNGC(); })

// BUILTIN(MFREE, { FREE((void*)POP(); })

// BUILTIN(CCOPY,
// {
//     tmp = POP();
//     void *dst = (void*)POP();
//     void *src = (void*)POP();
//     memcpy(dst, src, tmp);
// })

// BUILTIN(CMOVE,
// {
//     tmp = POP();
//     void *dst = (void*)POP();
//     void *src = (void*)POP();
//     memmove(dst, src, tmp);
// })
