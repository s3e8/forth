: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here @ aligned here !                   ;
: allot     here @ swap here +! align               ;

: make-inline
    latest @ dup @ 
    f_inline xor
    swap !
;
: inline    make-inline ; immediate

: make-variable
    allot
    word create make-inline
    ' lit ,
    ,
    ' exit ,
    ' eow ,
;
: variable cellsize make-variable ;

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

: negate    0 swap - ; inline
: true      1   ; inline
: false     0   ; inline
: not       0=  ; inline

: cell  cellsize   ; inline
: cells cellsize * ; inline
: cell+ cellsize + ; inline
: cell- cellsize - ; inline

: str= 	inline strcmp 0= 	;
: str< 	inline strcmp 0< 	;
: str> 	inline strcmp 0> 	;
: str<> inline strcmp 		;

: ?builtin      @ f_builtin     and ;
: ?hasarg       @ f_hasarg      and ;
: ?immediate    @ f_immediate   and ;
: ?hidden       @ f_hidden      and ;
: ?inline       @ f_inline      and ;

:  compile, dup ?builtin if >cfa @ , else ' call , >cfa , then  ;
:  compile  word find compile,                                  ;
: [compile] word find compile,                                  ; immediate

: unless    ' 0= , [compile] if                             ; immediate
: case      0                                               ; immediate
:  of       ' over , ' = , [compile] if ' drop ,            ; immediate
: (of)      [compile] if ' drop ,                           ; immediate
: endof     [compile] else                                  ; immediate
: endcase   ' drop , begin ?dup while [compile] then repeat ; immediate

: '\n'      10      ; inline
:  bl       32      ; inline
:  cr       10 emit ; inline
: space     bl emit ; inline
: spaces    begin dup 0> while space 1- repeat drop ;
: crs       begin dup 0> while cr    1- repeat drop ;

: char      word c@ ;
: literal   ' lit , ,   ; immediate
: ':'   [ char : ] literal  ; inline 
: ';'   [ char ; ] literal  ; inline 
: '('   [ char ( ] literal  ; inline 
: ')'   [ char ) ] literal  ; inline 
: '"'   [ char " ] literal  ; inline 
: 'A'   [ char A ] literal  ; inline 
: '0'   [ char 0 ] literal  ; inline 
: '-'   [ char - ] literal  ; inline 
: '.'   [ char . ] literal  ; inline

: \ begin key '\n' <> while repeat ; immediate

\ yksiriviset kommentit toimii nyt

: ( immediate      \ moniriviset kommentit
    1
    begin
	key
	dup '(' =
	if
	    drop 1+
	else
	    ')' =
	    if
		1-
	    then
	then
    dup 0=
    until
    drop
;

: constalign consthere @ aligned consthere ! ;

: c, here @ c! here @ 1+ here ! ;
: const, consthere @ ! consthere @ cell+ consthere ! ;
: constc, consthere @ c! consthere @ 1+ consthere ! ;

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


: ? @ . ;
: ?iscall   >cfa @ ' call = ;

: alias todo ;






( dictionaries )
variable current-vocab
variable latest-defined-vocab

0 current-vocab !
0 latest-defined-vocab !

: vocab-name        ( vocabentry -- name )              cell+ @     ;
: vocab-next        ( vocabentry -- nextvocabentry/0 )  2 cells + @ ;
: vocab-latest      ( vocab-entry -- latest )           @           ;
: set-vocab-name    ( name vocabentry -- )              cell+ !     ;
: set-vocab-next    ( nextentry vocabentry -- )         2 cells + ! ;
: set-vocab-latest  ( latest vocabentry -- )            !           ;
: vocab-useslist    ( vocabentry -- useslist )          3 cells +   ;

: find-vocabulary 					( name -- vocabulary/0 )
    latest-defined-vocab @         	( name latestvocab )
    begin
	dup 0= if                  		\ is the entry zero?
	    2drop 0 exit           		\ return zero
	else
	    2dup vocab-name str<>   	\ compare names
	then
    while
	    vocab-next
    repeat
    nip
;

: in: immediate
    word find-vocabulary
    ?dup if
	latest @ current-vocab @ set-vocab-latest   \ save latest to current vocabulary
	dup current-vocab !                         \ this is the new current vocabulary
	vocab-latest latest !                       \ get new latest from current vocabulary and save it to latest
    else
	." no such vocabulary" cr
    then
;

: vocabulary immediate
    word            ( vocabname )
    make-const-str  ( constvocabname )
    consthere @     ( constvocabname vocabulary )
    2dup set-vocab-name   ( constvocabname vocabulary )
    nip                   ( vocabulary )

    current-vocab @       ( vocabulary currentvocab )
    ?dup if
	latest @ swap set-vocab-latest
    then
    latest-defined-vocab @  ( vocabulary latestvocab )
    over set-vocab-next     ( vocabulary )  \ link them
    latest @                ( vocabulary currlatest )
    over set-vocab-latest   ( vocabulary )  \ save latest
    dup latest-defined-vocab !  \ make it the last defined vocab
    dup current-vocab !         \ it also becomes the current vocab like with in:
    vocab-useslist consthere !       \ advance consthere
;

: use immediate
    word find-vocabulary
    ?dup if
	const,
    else
	." no such vocabulary to use" cr
    then
;

: definitions immediate
    0 const,   \ terminate uses list
;

: vocabularies ( -- )
    latest-defined-vocab @
    begin
	dup
    while
	    dup vocab-name tell space
	    vocab-next
    repeat
    drop
;






: interpret todo ;
: quit      todo ;

( moniriviset
  kommentit toimii nyt )