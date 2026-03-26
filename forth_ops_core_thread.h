BUILTIN(NEW_THREAD,
{
    int ds_size = (int)POP();
    int rs_size = (int)POP();
    void** entry = (void**)POP();
    PUSH(create_thread(ds_size, rs_size, entry));
})

BUILTIN(KILL_THREAD,
{
    kill_thread();
    ip = current_thread->ip;
    ds = current_thread->ds;
    rs = current_thread->rs;
    ts = current_thread->ts;
    fs = current_thread->fs;
    t0 = current_thread->t0;
    s0 = current_thread->s0;
    r0 = current_thread->r0;
    f0 = current_thread->f0;
})

BUILTIN(PAUSE,
{
    current_thread->ip = ip;
    current_thread->ds = ds;
    current_thread->rs = rs;
    current_thread->ts = ts;
    current_thread->fs = fs;
    current_thread->t0 = t0;
    current_thread->s0 = s0;
    current_thread->r0 = r0;
    current_thread->f0 = f0;

    current_thread = current_thread->next;
    
    ip = current_thread->ip;
    ds = current_thread->ds;
    rs = current_thread->rs;
    ts = current_thread->ts;
    fs = current_thread->fs;
    t0 = current_thread->t0;
    s0 = current_thread->s0;
    r0 = current_thread->r0;
    f0 = current_thread->f0;
})