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

: min 2dup < if drop else nip then ;
: max 2dup > if drop else nip then ;
: fmin f2dup f< if fdrop else fnip then ;
: fmax f2dup f> if fdrop else fnip then ;






: alias todo ;

: here      dp @        ;

: cell  cellsize   ; inline
: cells cellsize * ; inline
: cell+ cellsize + ; inline
: cell- cellsize - ; inline

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