






: variable          cellsize    make-variable ;
: fvariable         todo ;
: v3variable 3      todo ;
: m3variable 3 3 *  todo ;

variable consthere
variable consthere0
variable datahere
variable datahere0




: str=  strcmp 0=   ; inline
: str<  strcmp 0<   ; inline
: str>  strcmp 0>   ; inline
: str<> strcmp      ; inline










: here      dp @        ;

: starts-with-colon? c@ ':' = ;



: handle-status ( code -- )
    case
        200 of ." OK"                    endof
        201 of ." Created"               endof
        400 of ." Bad Request"           endof
        401 of ." Unauthorized"          endof
        404 of ." Not Found"             endof
        500 of ." Internal Server Error" endof
        ." Unknown status"
    endcase
    cr
;
200 handle-status
404 handle-status
500 handle-status

: calculate ( a b op -- result )
    case
        '+' of + endof
        '-' of - endof
        '*' of * endof
        '/' of / endof
        ." unknown op" drop
    endcase
;
10 5 '+' calculate .
10 5 '-' calculate .
10 5 '*' calculate .
10 5 '/' calculate .

: day-name ( n -- )
    case
        0 of ." Monday"    endof
        1 of ." Tuesday"   endof
        2 of ." Wednesday" endof
        3 of ." Thursday"  endof
        4 of ." Friday"    endof
        5 of ." Saturday"  endof
        6 of ." Sunday"    endof
        ." unknown day"
    endcase
    cr
;

0 day-name
4 day-name
6 day-name