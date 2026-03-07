






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

: negate    0 swap - ; inline
: true      1   ; inline
: false     0   ; inline
: not       0=  ; inline








: here      dp @        ;

