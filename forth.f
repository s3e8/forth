: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here aligned here !                     ;
: allot     here swap here +! align                 ;

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

: variable cellsize make-variable ;
: foo 1 ;
: bar 2 ;


: inline    make-inline ; immediate
: literal   ' lit , ,   ; immediate


: ?builtin      @ f_builtin     and ;
: ?hasarg       @ f_hasarg      and ;
: ?immediate    @ f_immediate   and ;
: ?hidden       @ f_hidden      and ;
: ?inline       @ f_inline      and ;


: >mark     here @ 0 ,    ;
: >resolve  here @ swap ! ;


: if        ' 0branch , >mark                                       ; immediate
: then      dup here @ swap - swap !                                ; immediate
: else      '  branch , >mark swap dup here @ swap - swap !         ; immediate
: begin     here @                                                  ; immediate
: until     ' 0branch , here @ - ,                                  ; immediate
: again     '  branch , here @ - ,                                  ; immediate
: while     ' 0branch , >mark                                       ; immediate
: repeat    '  branch , swap here @ - , dup here @ swap - swap !    ; immediate
: recurse   '  call ,   latest @ >cfa ,                             ; immediate

:  compile, dup ?builtin if >cfa @ , else ' call , >cfa , then  ;
:  compile  word find compile,                                  ;
: [compile] word find compile,                                  ; immediate

: unless    ' 0= , [compile] if ; immediate
: case 0 ; immediate


: '\n'      10      ; inline
: bl        32      ; inline
: cr        10 emit ; inline
: space     32 emit ; inline
: spaces    begin dup 0 > while space 1 - repeat drop ;

: cell  cellsize   ; inline
: cells cellsize * ; inline
: cell+ cellsize + ; inline
: cell- cellsize - ; inline





: ? @ . ;

: ?iscall   >cfa @ ' call = ;


: alias todo ;


: interpret todo ;
: quit      todo ;