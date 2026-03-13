
: \\\ todo ;

variable symbol-table
1024 cells allot symbol-table !

: symbol-hash ( str -- index )
    0 swap
    begin dup c@ while
        swap 31 * swap c@ + swap
        1+
    repeat
    drop
    1024 mod
;

: intern-sym ( str -- sym )
    \ hash the string
    \ probe the table
    \ if found return existing
    \ if not found store and return new
;


\ LISP LIST-BUILDING WORDS IN FORTH-83

variable nil nil nil ; 		\ the empty list

( @items -> ) \ @items = maximum number of items this list

: newlist 	create here 2+ , nil , 2* allot ;
: first 	@ ; 									( @list -> @first ) 	\ @first is a pointer to first item of list
: null 		@ nil = ;								( @list | nil -> flag ) \ flag = true if list is empty
: tail 		dup null if @ else 2- then ;			( @list -> @tail )		\ @tail is a pointer to the tail of the list



: intern-str ( str -- conststr )
    consthere0 @          \ start at beginning of const region
    begin
        dup consthere @ <  \ while not at end
    while
        2dup str= if       \ found it?
            nip exit       \ return existing address
        then
        dup strlen 1+      \ advance past this string
        aligned +          \ keep aligned
    repeat
    drop                   \ not found, copy it in
    make-const-str
;






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