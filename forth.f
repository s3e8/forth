: alias todo ;

: here      dp @        ;

: cell      inline cellsize     ;
: cells     inline cellsize *   ;

: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here aligned here !                     ;
: allot     here swap here +! align                 ;

: >mark     here 0 ,    ;
: >resolve  here swap ! ;

: if        ' 0branch , >mark                                   ; immediate
: then      dup here swap - swap !                              ; immediate
: else      '  branch , >mark swap dup here swap - swap !       ; immediate
: begin     here                                                ; immediate
: until     ' 0branch , here - ,                                ; immediate
: again     '  branch , here - ,                                ; immediate
: while     ' 0branch , >mark                                   ; immediate
: repeat    '  branch , swap here - , dup here swap - swap !    ; immediate
: recurse   '  call ,   latest @ >cfa ,                         ; immediate

: cr    10 emit ;
: space 32 emit ;
: spaces begin dup 0 > while space 1 - repeat drop ;

: ? @ . ;

: ?builtin      @ f_builtin     and ;
: ?hasarg       @ f_hasarg      and ;
: ?immediate    @ f_immediate   and ;
: ?hidden       @ f_hidden      and ;
: ?inline       @ f_inline      and ;

: ?iscall   >cfa @ ' call = ;

:  compile, dup ?builtin if >cfa @ , else ' call , >cfa , then  ;
:  compile  word find compile,                                  ;
: [compile] word find compile,                                  ; immediate


: interpret todo ;
: quit      todo ;