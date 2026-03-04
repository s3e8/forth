: here      dp @        ;
: >mark     here 0 ,    ;
: >resolve  here swap ! ;

: if        ' 0branch , >mark                                   ; immediate
: then      dup here swap - swap !                              ; immediate
: else      ' branch , >mark swap dup here swap - swap !        ; immediate
: begin     here                                                ; immediate
: until     ' 0branch , here - ,                                ; immediate
: again     '  branch , here - ,                                ; immediate
: while     ' 0branch , >mark                                   ; immediate
: repeat    '  branch , swap here - , dup here swap - swap !    ; immediate

