: aligned   cellsize 1- + cellsize 1- invert and    ;
: align     here @ aligned here !                   ;
: allot     here @ swap here +! align               ;
: c,        here @ c! here @ 1+ here ! 				;

: make-inline	latest @ dup @ f_inline xor swap ! 	;
: inline    	make-inline 						; immediate

: make-variable	allot word create make-inline ' lit , , ' exit , ' eow , 	;
: variable cellsize make-variable 											;

: alias todo ;