: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here @ aligned here !                   ;
: allot     here @ swap here +! align               ;
: c,        here @ c! here @ 1+ here ! 				;

: make-inline	latest @ dup @ f_inline xor swap ! 	;
: inline    	make-inline 						; immediate

: make-variable	allot word create make-inline ' lit , , ' exit , ' eow , 	;
: variable cellsize make-variable 											;

variable consthere
variable consthere0
4096 cellsize * allot
dup consthere !
consthere0 !

variable datahere
variable datahere0
4096 cellsize * allot
dup datahere !
datahere0 !

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

: negate    0 swap - 	; inline
: true      1   		; inline
: false     0   		; inline
: not       0=  		; inline


: cell  cellsize   ; inline
: cells cellsize * ; inline
: cell+ cellsize + ; inline
: cell- cellsize - ; inline

: str= 	inline strcmp 0= 	;
: str< 	inline strcmp 0< 	;
: str> 	inline strcmp 0> 	;
: str<> inline strcmp 		;

: ? @ . ;
: ?builtin      @ f_builtin     and ;
: ?hasarg       @ f_hasarg      and ;
: ?immediate    @ f_immediate   and ;
: ?hidden       @ f_hidden      and ;
: ?inline       @ f_inline      and ;
: ?iscall   	>cfa @ ' call = 	;

:  compile, 	dup ?builtin if >cfa @ , else ' call , >cfa , then  ;
:  compile  	word find compile,                                  ;
: [compile] 	word find compile,                                  ; immediate
:  literal		' lit , ,   										; immediate

: unless    	' 0= , [compile] if                             ; immediate
: case      	0                                               ; immediate
:  of       	' over , ' = , [compile] if ' drop ,            ; immediate
: (of)      	[compile] if ' drop ,                           ; immediate
: endof     	[compile] else                                  ; immediate
: endcase   	' drop , begin ?dup while [compile] then repeat ; immediate

: '\n'      	10      ; inline
:  bl       	32      ; inline
:  cr       	10 emit ; inline
: space     	bl emit ; inline
: spaces    	begin dup 0> while space 1- repeat drop ;
: crs       	begin dup 0> while cr    1- repeat drop ;

: char      	word c@ ;
: ':'   		[ char : ] literal  ; inline
: ';'   		[ char ; ] literal  ; inline
: '('   		[ char ( ] literal  ; inline
: ')'   		[ char ) ] literal  ; inline
: '"'   		[ char " ] literal  ; inline
: 'A'   		[ char A ] literal  ; inline
: '0'   		[ char 0 ] literal  ; inline
: '-'   		[ char - ] literal  ; inline
: '.'   		[ char . ] literal  ; inline

: decimal 		10 base ! ; immediate
: hex 			16 base ! ; immediate

: \ 			begin key '\n' <> while repeat ; immediate

: min/max 		todo ;

: ( 			immediate      \ moniriviset kommentit
    1
	begin
		key dup '(' =
		if
	    	drop 1+
		else
	    	')' =
	    	if 1-
	    	then
		then
    	dup 0=
    until drop
;

: constalign 	consthere @ aligned consthere ! 			;
: const, 		consthere @ ! consthere @ cell+ consthere ! ;
: constc, 		consthere @ c! consthere @ 1+ consthere ! 	;

: s" immediate
    state @ if            ( if compiling, emit a lit instruction with the starting pointer )
	consthere @          ( save string starting pos )
	begin
	    key     ( startpos key )
	    dup '"' <>  ( startpos key notadoublequote )
	while
		constc,
	repeat
	drop
	0 constc,  ( null-terminate! )
	' lit ,
	,              ( emit starting pos )
	constalign
    else
	consthere @
	begin
	    key
	    dup '"' <>
	while
		over c!
		1+
	repeat
	drop
	0 over c! drop
	consthere @
    then
;

: ." immediate
    state @ if
	[compile] s"
	' tell ,
    else
	begin
	    key
	    dup '"' = if
		drop exit
	    then
	    emit
	again
    then
;

: pick 1+ cellsize * dsp@ + @ ;

: make-const-str ( str -- conststr )
    dup consthere @
    strcpy drop
    consthere @ swap
    strlen 1+ consthere +!
    constalign
;







\ LISP LIST-BUILDING WORDS IN FORTH-83

variable nil nil nil !														\ the empty list
													( @items -> ) 			\ @items = maximum number of items this list
: newlist 	create here 2+ , nil , 2* allot ;
: first 	@ ; 									( @list -> @first ) 	\ @first is a pointer to first item of list
: null 		@ nil = ;								( @list | nil -> flag ) \ flag = true if list is empty
: tail 		dup null if @ else 2- then ;			( @list -> @tail )		\ @tail is a pointer to the tail of the list




