


: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here aligned here !                     ;

: allot     here swap here +! align                 ;

:  compile, dup ?builtin if >cfa @ , else ' call , >cfa , then  ;
:  compile  word find compile,                                  ;
: [compile] word find compile,                                  ; immediate

: make-inline
    latest @ dup @ 
    f_inline xor
    swap !
;

: make-variable
    allot
    word create make-inline
    ' lit ,
    ,
    ' exit ,
    ' eow ,
;

: inline make-inline ; immediate

: variable          cellsize    make-variable ;
: fvariable         todo ;
: v3variable 3      todo ;
: m3variable 3 3 *  todo ;

variable consthere
variable consthere0
variable datahere
variable datahere0


: bl    32      ; inline
: cr    10 emit ; inline
: space 32 emit ; inline
: spaces begin dup 0 > while space 1 - repeat drop ;

: str=  strcmp 0=   ; inline
: str<  strcmp 0<   ; inline
: str>  strcmp 0>   ; inline
: str<> strcmp      ; inline

: negate    0 swap - ; inline
: true      1   ; inline
: false     0   ; inline
: not       0=  ; inline






: alias todo ;

: here      dp @        ;

: cell  cellsize   ; inline
: cells cellsize * ; inline
: cell+ cellsize + ; inline
: cell- cellsize - ; inline





: ? @ . ;

: ?builtin      @ f_builtin     and ;
: ?hasarg       @ f_hasarg      and ;
: ?immediate    @ f_immediate   and ;
: ?hidden       @ f_hidden      and ;
: ?inline       @ f_inline      and ;

: ?iscall   >cfa @ ' call = ;


: interpret todo ;
: quit      todo ;