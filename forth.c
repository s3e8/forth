/* yet another attempt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // todo: impl own isspace

/* the cell type */
#include <stdint.h>
typedef uintptr_t cell;

/* config defaults --------------------------------- */
#define FORTH_VERSION               0
#define WORDBUF_LENGTH              128
#define LINEBUF_LENGTH              512
#define DICTIONARY_SIZE             10*1024*1024 // 10mb
#define NESTINGSTACK_MAX_DEPTH      512
#define NESTINGSTACK_SIZE           512
#define DATASTACK_SIZE              1024
#define RETURNSTACK_SIZE            512

/// reader ///
typedef struct reader_state
{
    FILE* stream;
    cell  word_length;
    cell  line_length;
    char* current_word;
    char* current_line;
    char* remaining_words;
} reader_state_t;


static void init_reader_state(reader_state_t* state, char* wbuf, cell wbuf_size, char* lbuf, cell lbuf_size, FILE* fp) {
    state->stream             = fp;
    state->word_length        = wbuf_size;
    state->line_length        = lbuf_size;
    state->current_line       = lbuf;
    state->current_line[0]    = '\0';
    state->remaining_words    = lbuf;
}


static reader_state_t* open_file(const char* filename, const char* mode)
{
    FILE* fp = fopen(filename, mode);
    if (!fp) return NULL;

    char* lbuf = malloc(LINEBUF_LENGTH);
    if (!lbuf) goto err_exit;
    char* wbuf = malloc(LINEBUF_LENGTH);
    if (!wbuf) goto err_exit;

    reader_state_t* reader_state = malloc(sizeof(reader_state_t));
    if (!reader_state) goto err_exit;

    reader_state->stream            = fp;
    reader_state->line_length       = LINEBUF_LENGTH;
    reader_state->word_length       = WORDBUF_LENGTH;
    reader_state->current_line      = lbuf;
    reader_state->remaining_words   = lbuf;
    reader_state->current_line[0]   = '\0';

    return reader_state;

    err_exit:
        free(lbuf);
        fclose(fp);
        return NULL;
}


static void close_file(reader_state_t* reader_state)
{
    if (reader_state->stream) fclose(reader_state->stream);
    free(reader_state->current_line);
    free(reader_state);
}


static void skip_whitespace(reader_state_t* reader_state)
{
    while(isspace(*reader_state->remaining_words)) reader_state->remaining_words++;
}


static cell is_eol(reader_state_t* reader_state)
{
    skip_whitespace(reader_state);
    return (cell)(*reader_state->remaining_words == '\0');
}


static cell is_eof(reader_state_t* reader_state)
{
    return (cell)(is_eol(reader_state) && feof(reader_state->stream));
}


static char* get_next_line(reader_state_t* reader_state)
{
    // printf("getting next line...\n");
    if (reader_state->stream == stdin)
    {
        printf("forth> ");
        fflush(stdout);
    }
    if (!fgets(reader_state->current_line, reader_state->line_length, reader_state->stream)) return NULL;

    reader_state->remaining_words = reader_state->current_line;
    // no, we need \n later
    // remaining_words[strcspn(remaining_words, "\n")] = '\0'; // todo: is this necessary? -- yes... change new line to termination character

    return reader_state->remaining_words;
}


static char* get_next_word(reader_state_t* reader_state, char* tobuf)
{
    char* new_word_buffer = tobuf; // setting up a temp buffer to copy new word into current_word

    // buffer exhausted? get_next_line
    while (1)
    {
        // Skip whitespace using isspace() for all whitespace characters
        while (*reader_state->remaining_words && isspace(*reader_state->remaining_words))
        { // todo: match with skip_whitespace
            reader_state->remaining_words++;
        }

        // If we're at the end of the current buffer
        if (*reader_state->remaining_words == '\0')
        {
            // printf("Buffer exhausted, getting next line...\n");
            if (!get_next_line(reader_state))
            {
                // todo: should I set stream to stdin here?
                // printf("No more lines, returning NULL\n");
                tobuf[0] = '\0';
                return NULL;
            }
            // Continue to skip whitespace on the new line
            continue;  // <-- This goes back to the START of the while loop
        }

        // Found a non-whitespace character, start reading word
        break;
    }

    // // Copy word until whitespace or end of string
    while (*reader_state->remaining_words && !isspace(*reader_state->remaining_words))
    {
        *new_word_buffer++ = *reader_state->remaining_words++;
    }

    *new_word_buffer = '\0'; // todo: what's this doing?

    return tobuf;;
}

// static cell is_number(const char* token, cell result, cell base) {
//     char *endptr; // todo: token is current_work, remove arg
//     result = (cell)strtol(token, &endptr, (int)base);

//     // Success if we consumed the whole string and it wasn't empty
//     return (*endptr == '\0' && endptr != token);
// }

static int key(reader_state_t* reader_state)
{
    if (*reader_state->remaining_words == '\0')
    {
        if (!get_next_line(reader_state))
        {
            if (reader_state->stream != stdin)
            {
                reader_state->stream = stdin;
                if (!get_next_line(reader_state)) return -1;
            }
            else return -1;
        }
    }

    return *reader_state->remaining_words++;
}


void emit(int c, FILE* output_stream)
{
    // printf("int: %d\n", c);
    fputc(c, output_stream);
}


/* toupper() clone so we don't have to pull in ctype.h */
char up(char c) // todo: cell?
{
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}










/// dictionary ///
#define BIT(x) (1<<(x))
#define FLAG_HIDDEN     BIT(0)
#define FLAG_IMMEDIATE  BIT(1)
#define FLAG_BUILTIN    BIT(2)
#define FLAG_HASARG     BIT(3)
#define FLAG_INLINE     BIT(4)
#define FLAG_DEFERRED   BIT(5)

#define STATE_IMMEDIATE 0
#define STATE_COMPILE   1


typedef struct word_header
{
    cell                flags;
    struct word_header* next_word;
    char                name[WORDBUF_LENGTH];
} word_header_t;


/* file-global dictionary vars */
static void*            here;
static void*            here0;
static cell             here_size = DICTIONARY_SIZE;
static word_header_t*   latest = NULL;


static word_header_t* create(const char* name, cell flags)
{
    if (!name) name = '\0'; // for unnamed words.. apparently, todo:

    word_header_t* new_word = (word_header_t*)here;
    here += sizeof(word_header_t);
    // Optional: Automatically align // You might want to make sure here is always aligned after every operation. Some Forths do this automatically after each dictionary update: .. apparently
    // here = (void*)ALIGN_ADDR((cell)here);

    strncpy(new_word->name, name, WORDBUF_LENGTH);
    new_word->flags     = flags;
    new_word->next_word = latest;

    latest = new_word;
    // todo: error case if no new word is created?

    return new_word;
}


static word_header_t* find(const char* name)
{
    if (!name) return NULL;

    word_header_t* word = latest;
    while (word)
    {
        if (!(word->flags & FLAG_HIDDEN) &&
            !strncmp(word->name, name, WORDBUF_LENGTH))
            return word;

        word = word->next_word;
    }

    return NULL;
}


static void** cfa(word_header_t* word)
{   /* void** is a ptr to a code array */
    return (void**)(word + 1);
}


static void* tick(word_header_t* word)
{   /* void** is a ptr to a code array */
    if (word->flags & FLAG_BUILTIN) return *cfa(word);
    else                            return  cfa(word);
}


static void comma(cell value)
{
    *(cell*)here = value;
    here += sizeof(cell);
}


/* bootstrap utility functions */
void* getcode(const char* name)
{
    word_header_t* word = find(name);
    return *(cfa(word)); // todo: change to tick?
}

void defcode(const char* name, void* code, cell flags)
{
    create(name, flags | FLAG_BUILTIN);
    comma((cell)code);
}

void defword(const char* name, void* code[], int wordcount, cell flags)
{
    create(name, flags);
    // todo: dont need to comma(call or docol here cause it's added in as code[0]
    // todo: should I impl this without wordcount?
    for (int i = 0; i < wordcount; i++) comma((cell)code[i]);
    comma((cell)getcode("eow")); // not sure we need this...
}

void defconst(const char* name, cell value)
{   // todo: better name for flagdef?
    void* flagdef[] = { getcode("lit"), (void*)value, getcode("exit") };
    defword(name, flagdef, 3, FLAG_INLINE);
}

void defvar(const char* name, cell value)
{
    cell address = (cell)here; // the const lit value is the address of the var's value that occurs just before the const
    comma(value);

    defconst(name, address);
}

void deffconst(const char* name, cell value)
{   // todo: better name for flagdef?
    void* flagdef[] = { getcode("flit"), (void*)value, getcode("exit") };
    defword(name, flagdef, 3, FLAG_INLINE);
}










/// vm ///
#define NEXT() goto *(*ip++)  // todo: why ** //

#define PUSHRS(x)   (*--rs = (void**)(x))   // grow/decrement downward first, then store
#define POPRS()     (*rs++)                 // pop, then shrink upward

#define TOP()       (*ds)
#define PEEK()      (*(ds-1)) // slarba uses TOP() for dup... why must i use this instead when I've basically modelled everything after what he's doing?
#define AT(x)       (*(ds+(x))) // todo: what's this?
#define PUSH(x)     (*--ds = (cell)(x))     // grow/decrement downward first, then store
#define POP()       (*ds++)                 // pop, then shrink upward

#define FTOP()      (*fs)
#define FPEEK()     (*(fs-1)) // slarba uses TOP() for dup... why must i use this instead when I've basically modelled everything after what he's doing?
#define FAT(x)      (*(fs+(x)))
#define FPUSH(x)     *--fs = (float)(x)
#define FPOP()       (*fs++)

#define ARG()       (*ip++)                 // takes the next item on the ip thread as
#define INTARG()    ((cell)(*ip++))         // takes the next item on the ip thread as an int argument, casted as (cell) cause... forth
#define FLOATARG()  (*(float*)ip)





// helper macros //
#define OP(name) op_##name
#define CODE(name) &&op_##name
#define BUILTIN(name, code) OP(name): { code NEXT(); }

#define CELL_SIZE sizeof(cell)
#define CELL_MASK (CELL_SIZE - 1)
#define IS_ALIGNED(addr) (((cell) (addr) & CELL_MASK) == 0)
#define ALIGN_ADDR(addr)  ((cell)((addr) + CELL_MASK) &  ~CELL_MASK)
#define OFFSET(x)  (void*)((x) * sizeof(cell)) // todo: or (x) * sizeof...? // to calculate branch offsets









// void **ip, cell *ds, void ***rs, reader_state_t *inputstate, FILE *outp, int argc, char **argv
extern void start_forth(void** ip, cell* ds, void*** rs, reader_state_t* reader_state, FILE* output_stream, int argc, char** argv)
{
    cell*    s0 = ds;
    cell*    t0 = NULL; // temp stack, to be later set in forth
    cell*    ts = NULL;
    float*   f0 = NULL;
    float*   fs = NULL;
    void***  r0 = rs;

    void**   nestingstack_space[NESTINGSTACK_MAX_DEPTH];                 // i think this is for re-entering the interpreter after immediate execution??
    void***  nestingstack = nestingstack_space + NESTINGSTACK_MAX_DEPTH; // ... as opposed to rs which is for lots of stuff // todo: rename to ns?

    // some default vars and constants
    cell base  = 10;
    cell state = 0;

    /* and finally, some quick access variables */
    // todo: organize by what is mostly used by each type of bytecode... eg stack, math, dict, etc
    register word_header_t* header;
    register char* address; // todo: shouldn't this be cell?
    register void* code;
    register void* fn;
    register void* xt;
    register cell  value;
    register cell  tmp;
    register cell  a;
    register cell  b;

    void* code_immediatebuf[] = { NULL, CODE(IRETURN) };                                  // todo: do i need to put length inside immediatebuf[2] etc
    void* word_immediatebuf[] = { CODE(CALL), NULL, CODE(IRETURN) };                      //
    void* quit[]              = { CODE(INTERPRET),  CODE(BRANCH), OFFSET(-2), CODE(EOW) }; /* the interpreter loop */

    char            current_word[LINEBUF_LENGTH];
    char            wordbuf[WORDBUF_LENGTH];
    char            stdinbuf[1024];
    reader_state_t  stdin_state;

    /* ------------------------------------------------------ */
    /*     | name         | code              | flags         */

    /// WORDS NEEDED FOR INNER INTERPRETER ///
    defcode("interpret",    CODE(INTERPRET),    0);
    defcode("branch",       CODE(BRANCH),       FLAG_HASARG);
    defcode("eow",          CODE(EOW),          0);
    defcode("call",         CODE(CALL),         FLAG_HASARG); // for scheduling
    defcode("ireturn",      CODE(IRETURN),      0);
    defcode("lit",          CODE(LIT),          FLAG_HASARG);
    defcode("exit",         CODE(EXIT),         0);
    /// END WORDS NEEDED FOR INNER INTERPRETER ///
    //////////////////////////////////////////////

    /// VARIABLE & CONST ACCESSORS (some defined as bytecode) ///
    defconst("version",     FORTH_VERSION);
    defconst("f_builtin",   FLAG_BUILTIN);
    defconst("f_hasarg",    FLAG_HASARG);
    defconst("f_immediate", FLAG_IMMEDIATE);
    defconst("f_hidden",    FLAG_HIDDEN);
    defconst("f_inline",    FLAG_INLINE);
    defconst("f_deferred",  FLAG_DEFERRED);
    defconst("cellsize",    (cell) sizeof(cell));
    defconst("floatsize",   (cell) sizeof(float));
    defconst("headersize",  (cell) sizeof(word_header_t));
    defconst("here",        (cell) &here); // we give the address so we can store stuff there
    defconst("here0",       (cell) here0); // todo: why not &here0? cause malloc?
    defconst("s0",          (cell) &s0);
    defconst("r0",          (cell) &r0);
    defconst("f0",          (cell) &f0); // todo... why not define fs/ts?
    defconst("t0",          (cell) &t0);
    defconst("state",       (cell) &state); // todo: should this be a const or a code...
    defconst("base",        (cell) &base);

    // todo:
    // defcode("does>",    CODE(DOES),     0);
    // defcode("dodoes",   CODE(DODOES),   0);
    // defcode("fsp!", CODE(SETFS), 0);
    // defcode("fsp@", CODE(GETFS), 0);
    // defcode("tsp!", CODE(SETT0), 0); gett0 or getts?
    // defcode("tsp@", CODE(GETT0), 0);
    // defcode("2nip",     CODE(2NIP),     0);  // used in new find
    // defcode("execute",  CODE(EXECUTE),  0);  // needed for interpret rewrite

    // defcode("depth",      CODE(DEPTH),     0);  // or define in forth

    // misc
    defcode("jump",         CODE(JUMP),             FLAG_HASARG);
    defcode("noop",         CODE(NOOP),             0);
    defcode("execute",      CODE(EXECUTE),          0); // todo: vs builtin-exe vs iexe?
    defcode("exec-builtin", CODE(EXEC_BUILTIN),     0); // todo: rename defcode to definstr? nah...
    defcode("iexecute",     CODE(IEXECUTE),         0);

    // defcode("field@", CODE(GET_FIELD), FLAG_HASARG);
    // defcode("field!", CODE(SET_FIELD), FLAG_HASARG);
    // defcode("ffield@", CODE(GET_FFIELD), FLAG_HASARG);
    // defcode("ffield!", CODE(SET_FFIELD), FLAG_HASARG);
    // defcode("sfield@", CODE(GET_SFIELD), FLAG_HASARG);
    // defcode("sfield!", CODE(SET_SFIELD), FLAG_HASARG);

    defcode("1branch",   CODE(1BRANCH),    FLAG_HASARG);
    defcode("<branch",   CODE(LTBRANCH),   FLAG_HASARG);
    defcode(">branch",   CODE(GTBRANCH),   FLAG_HASARG);
    defcode("<=branch",  CODE(LTEBRANCH),  FLAG_HASARG);
    defcode(">=branch",  CODE(GTEBRANCH),  FLAG_HASARG);
    defcode("0<branch",  CODE(LTZBRANCH),  FLAG_HASARG); // todo: change 0 order?
    defcode("0>branch",  CODE(GTZBRANCH),  FLAG_HASARG);
    defcode("0<=branch", CODE(LTEZBRANCH), FLAG_HASARG);
    defcode("0>=branch", CODE(GTEZBRANCH), FLAG_HASARG);
    defcode("=branch",   CODE(EQBRANCH),   FLAG_HASARG);
    defcode("<>branch",  CODE(NEQBRANCH),  FLAG_HASARG);

    defcode("2r>", CODE(2FROMR), 0);
    defcode("2>r", CODE(2TOR),  0);
    defcode("rdrop", CODE(RDROP), 0);
    defcode("2rdrop", CODE(2RDROP), 0);
    defcode("dsp@", CODE(DSP_FETCH), 0);
    defcode("dsp!", CODE(DSP_STORE), 0);
    defcode("rsp@", CODE(RSP_GET), 0);
    defcode("rsp!", CODE(RSP_PUT), 0);
    // // lit
    defcode("lit+", CODE(LIT_PLUS), FLAG_HASARG);
    defcode("lit-", CODE(LIT_MINUS), FLAG_HASARG);
    defcode("flit", CODE(FLIT), FLAG_HASARG);
    defcode("flit+", CODE(FLIT_PLUS), FLAG_HASARG);
    defcode("flit-", CODE(FLIT_MINUS), FLAG_HASARG);
    // dup
    // dup2??
    defcode("fdup", CODE(FDUP), 0);
    defcode("fdup2", CODE(FDUP2), 0);
    defcode("fdupvec", CODE(FDUPVEC), 0);
    defcode("dup@", CODE(DUP_AT), 0);
    // nip
    defcode("fnip", CODE(FNIP), 0);
    defcode("2nip", CODE(2NIP), 0);
    defcode("2fnip", CODE(2FNIP), 0);
    // 2dup
    defcode("f2dup", CODE(F2DUP), 0);
    defcode("?dup", CODE(COND_DUP), 0);
    // swap
    defcode("fswap", CODE(FSWAP), 0);
    defcode("swapdup", CODE(SWAPDUP), 0);
    // over
    // tuck
    // drop
    defcode("fdrop", CODE(FDROP), 0);
    // rot
    // -rot
    defcode("frot", CODE(FROT), 0);
    defcode("-frot", CODE(FMROT), 0);
    // // 2drop
    // // /mod
    defcode("u/mod", CODE(UDIVMOD), 0);
    // // +
    defcode("bi+", CODE(BI_ADD), 0);
    // // ...
    defcode("u<", CODE(U_LT), 0);
    defcode("u>", CODE(U_GT), 0);
    defcode("u<=", CODE(U_LTE), 0);
    defcode("u>=", CODE(U_GTE), 0);
    // ...
    defcode("-!", CODE(MEM_SUB), 0);
    // // ...
    defcode("f,", CODE(FCOMMA), 0);
    //
    defcode("f!", CODE(FSTORE), 0);
    defcode("f@", CODE(FFETCH), 0);
    defcode("var@", CODE(VAR_AT), FLAG_HASARG);
    defcode("var!", CODE(VAR_TO), FLAG_HASARG);
    defcode("fvar@", CODE(FVAR_AT), FLAG_HASARG);
    defcode("fvar!", CODE(FVAR_TO), FLAG_HASARG);
    defcode("svar@", CODE(SVAR_AT), FLAG_HASARG);
    defcode("svar!", CODE(SVAR_TO), FLAG_HASARG);
    // // // c!
    // // / c@
    defcode("s!", CODE(SSTORE), 0);
    defcode("s@", CODE(SFETCH), 0);
    defcode("c@c!", CODE(BYTE_COPY), 0);

    // defcode("malloc", CODE(MALLOC), 0);
    // defcode("malloc-atomic", CODE(MALLOC_ATOMIC), 0);
    // defcode("mrealloc", CODE(REALLOC), 0);
    // defcode("run-gc", CODE(RUN_GC), 0);
    // defcode("mfree, CODE(MFREE), 0);
    // defcode("ccopy", CODE(CCOPY), 0);
    // defcode("cmove", CODE(CMOVE), 0);
    // //
    defcode("number",   CODE(PARSE_NUM), 0);  // needed for interpret rewrite
    defcode("fnumber",  CODE(PARSE_FNUM), 0);

    // defcode("open-file",  CODE(OPEN_FILE),  0);
    // defcode("close-file", CODE(CLOSE_FILE), 0);
    // defcode("?eof",       CODE(IS_EOF),     0);
    // defcode("?eol",       CODE(IS_EOL),     0);
    // defcode("prompt",     CODE(PROMPT),     0);

    // defcode("tsp@", CODE(GET_T0), 0); // todo: rename?
    // defcode("tsp!", CODE(SET_T0), 0);
    // defcode(">t", CODE(TO_TMP), 0);
    // defcode("t>", CODE(FROM_TMP), 0);

    // // dyncall stuff


    // defcode("new-thread", CODE(NEW_THREAD), 0);
    // defcode("kill-thread", CODE(KILL_THREAD), 0);

    // defcode("fsp!", CODE(SET_FS), 0);
    // defcode("fsp@", CODE(GET_FS), 0);
    // defcode("f+", CODE(FADD), 0);
    // defcode("f-", CODE(FSUB), 0);
    // defcode("f*", CODE(FMUL), 0);
    // defcode("f/", CODE(FDIV), 0);
    // defcode("powf", CODE(POWF), 0); // todo: rename to fpow?
    // defcode("f<", CODE(FLT), 0);
    // defcode("f>", CODE(FGT), 0);
    // defcode("f<=", CODE(FLTE), 0);
    // defcode("f>=", CODE(FGTE), 0);
    // defcode("fabs", CODE(FABS), 0);
    // defcode("ffloor", CODE(FFLOOR), 0);
    // defcode("fsqrt", CODE(FSQRT), 0);
    // defcode("fsin", CODE(FSIN), 0);
    // defcode("fcos", CODE(FCOS), 0);
    // defcode("ftan", CODE(FTAN), 0);
    // defcode("fasin", CODE(FASIN), 0);
    // defcode("facos", CODE(FACOS), 0);
    // defcode("fatan", CODE(FATAN), 0);
    // defcode("fatan2", CODE(FATAN), 0);
    // defcode("fceil", CODE(FCEIL, 0);
    // defcode("f>i", CODE(F_TO_I), 0);
    // defcode("i>f", CODE(I_TO_F), 0);
    // defcode("v3+", CODE(V3_ADD), 0);
    // defcode("v3-", CODE(V3_SUB), 0);
    // defcode("v3s*", CODE(V3_SCALAR_MULT), 0);
    // defcode("v3s/", CODE(V3_SCALAR_DIV), 0);
    // defcode("v3dot", CODE(V3_DOT), 0);
    // defcode("v3len^2", CODE(V3_LEN_SQUARED), 0);
    // defcode("v3cross", CODE(V3_CROSS), 0);
    // defcode("matvec3*", CODE(MAT_V3_MUL), 0); // todo: did i butcher the naming?
    // defcode("matmat*", CODE(M33_M33_MUL), 0); // todo: ^ same here, obv
    // defcode("store-v3", CODE(STORE_V3), 0);
    // defcode("load-v3", CODE(LOAD_V3), 0);

    // //
    // //

    // defcode("format", CODE(FORMAT), 0); // dyncall thing??
    // //     BYTECODE(FORMAT, "format", 1, 0, 0, {
    // //     char *fmt = (char*)POP();
    // //     static char format_buf[512];
    // //     char *out = format_buf;
    // //     char *f = fmt;
    // //     while (*f) {
    // //         if (*f == '%') {
    // //             f++;
    // //             switch (*f++) {
    // //                 case 'd': out += sprintf(out, "%d", (int)POP()); break;
    // //                 case 's': out += sprintf(out, "%s", (char*)POP()); break;
    // //                 case '%': *out++ = '%'; break;
    // //             }
    // //         } else {
    // //             *out++ = *f++;
    // //         }
    // //     }
    // //     *out = '\0';
    // //     PUSH(format_buf);
    // // })

    // // */ -- ( n1 n2 n3 — n4 )
    // BUITIN(MUL_DIV,
    // {

    // })

    // // */MOD -- ( n1 n2 n3 — n4 n5 )
    // BUILTIN(MUL_DIV_MOD,
    // {
    // })
    // // +Loop
    // // leave
    // // page
    // // quit -- ( — ) Terminates execution for the current task and returns control to the terminal.













    // control flow //
    defcode("0branch",      CODE(0BRANCH),          0);

    // rstack //
    defcode("r>",           CODE(FROMR),            0);
    defcode(">r",           CODE(TOR),              0);
    defcode("r@",           CODE(RFETCH),           0);

    // compile //
    defcode(":",            CODE(COLON),        0);
    defcode(";",            CODE(SEMICOLON),    FLAG_IMMEDIATE);
    defcode("'",            CODE(TICK),         FLAG_IMMEDIATE);
    defcode(",",            CODE(COMMA),        0);
    defcode("[",            CODE(LBRAC),        FLAG_IMMEDIATE);
    defcode("]",            CODE(RBRAC),        0);
    defcode(">cfa",         CODE(TOCFA),        0);
    defcode("immediate",    CODE(IMMEDIATE),    FLAG_IMMEDIATE);
    // defcode("inline",       CODE(INLINE),       FLAG_IMMEDIATE); // keep in bootstrap
    // defcode("compile",      CODE(COMPILE),      FLAG_IMMEDIATE);

    // memory //
    defcode("!",            CODE(STORE),        0);
    defcode("@",            CODE(FETCH),        0);
    defcode("c!",           CODE(CSTORE),       0);
    defcode("c@",           CODE(CFETCH),       0);
    defcode("+!",           CODE(MEMADD),       0);

    // dstack //
    defcode("dup",          CODE(DUP),          0);
    defcode("swap",         CODE(SWAP),         0);
    defcode("drop",         CODE(DROP),         0);
    defcode("over",         CODE(OVER),         0);
    defcode("nip",          CODE(NIP),          0);
    defcode("tuck",         CODE(TUCK),         0);
    defcode("rot",          CODE(ROT),          0);
    defcode("-rot",         CODE(MROT),         0);
    defcode("2dup",         CODE(2DUP),         0);
    defcode("2drop",        CODE(2DROP),        0);
    defcode("?dup",         CODE(CONDDUP),      0);

    // math //
    defcode("+",            CODE(ADD),          0);
    defcode("-",            CODE(SUB),          0);
    defcode("*",            CODE(MUL),          0);
    defcode("/",            CODE(DIV),          0);
    defcode("mod",          CODE(MOD),          0);
    defcode("/mod",         CODE(DIVMOD),       0);
    defcode("1+",           CODE(ADD1),         0);
    defcode("1-",           CODE(SUB1),         0);
    defcode("=",            CODE(EQ),           0);
    defcode("<>",           CODE(NEQ),          0);
    defcode(">",            CODE(GT),           0);
    defcode("<",            CODE(LT),           0);
    defcode(">=",           CODE(GTE),          0);
    defcode("<=",           CODE(LTE),          0);
    defcode("0=",           CODE(EQZ),          0);
    defcode("0>",           CODE(GTZ),          0);
    defcode("0<",           CODE(LTZ),          0);
    defcode("0>=",          CODE(GTEZ),         0); // todo: change the order! .. or keep cause forth
    defcode("0<=",          CODE(LTEZ),         0);
    defcode("0<>",          CODE(NEQZ),         0);
    defcode("abs",          CODE(ABS),          0);
    defcode("min",          CODE(MIN),          0);
    defcode("max",          CODE(MAX),          0);
    // bitwise (bit ops?) //
    defcode("and",          CODE(AND),          0);
    defcode("or",           CODE(OR),           0);
    defcode("xor",          CODE(XOR),          0);
    defcode("invert",       CODE(NEG),          0);
    defcode("lshift",       CODE(LSHIFT),       0);
    defcode("rshift",       CODE(RSHIFT),       0);

    // io //
    defcode(".",            CODE(DOT),          0);
    defcode("word",         CODE(WORD),         0);
    defcode("key",          CODE(KEY),          0);
    defcode("emit",         CODE(EMIT),         0);
    defcode("tell",         CODE(TELL),         0);

    // dictionary //
    defcode("latest",       CODE(LATEST),           0); // bytecode, since it's a C var that changes
    defcode("create",       CODE(CREATE),       0);
    defcode("find",         CODE(FIND),         0);
    defcode("variable",     CODE(VARIABLE),     0);
    defcode("hidden",       CODE(HIDDEN),       0);

    // string //
    defcode("strcat",       CODE(STRCAT),       0);
    defcode("strlen",       CODE(STRLEN),       0);
    defcode("strncpy",      CODE(STRNCPY),      0);
    defcode("strcpy",       CODE(STRCPY),       0);
    defcode("strcmp",       CODE(STRCMP),       0);

    // sys //
    defcode("bye",          CODE(BYE),          0);
    defcode("die",          CODE(DIE),          0);

    // inner //
    defcode("iword",            CODE(IWORD),            0);
    // defcode("iexecute",     CODE(IEXECUTE),         0);
    // defcode("execute",      CODE(EXECUTE),      0); // todo: vs builtin-exe vs iexe?

    // dbg //
    defcode("ps",           CODE(PRINT_STACK),  0);
    defcode("todo",         CODE(TODO),         0);

    ip = quit;
    NEXT();

    return;

    #include "forth_ops_core_all.h"
}








int main(int argc, char** argv)
{

    cell    datastack[1024];
    void**  returnstack[512];


    reader_state_t* input_state = open_file("slarba_forth.f", "r");
    if(!input_state)
    {
        fprintf(stderr, "Cannot open bootstrap file forth.f!\n");
        return 1;
    }


    here_size   = 10*1024*1024;
    here0       = malloc(here_size);
    here        = here0;
    start_forth(NULL, datastack+1024, returnstack+512, input_state, stdout, argc, argv);
    close_file(input_state);
    // todo: ideally we can defcode some more stuff here...


    return 0;

}