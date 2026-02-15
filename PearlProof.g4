grammar PearlProof;

file: line+ EOF;

line: 'check' expr '=' expr ;

expr: expr '^' expr           # Power
    | expr ('*'|'/') expr     # MulDiv
    | expr ('+'|'-') expr     # AddSub
    | '(' expr ')'            # Parens
    | INT                     # Int
    | ID                      # Var
    ;

INT: [0-9]+ ;
ID: [a-zA-Z][a-zA-Z0-9]* ;
WS: [ \t\r\n]+ -> skip ;
