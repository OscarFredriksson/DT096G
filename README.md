# DT096G
Programspråksteori

### Labb 0: Addition parser 

### Labb 1: Regex parser

### Labb 2: Javascript prototype mechanism

### Labb 3: Pow calculated at compile time

## Labb 1:

#### Operationer

Operator  | Operation              | Förklaring                                         | Syntax
:---:     |   ---                  | ---                                                | ---
 \+       | eller                  | Passar ett av två alternativ.                      | `OP1+OP2`
 \*       | flera                  | Passar ett eller flera upprepningar av en operand. | `OP*`
 ()       | infångningsgrupp       | Deluttryck. Uttrycket parsas separat.              | `(EXPR)`
 .        | tecken                 | Matchar varlfritt tecken.                          | `.`
 {}       | räknare                | Matchar precis N stycken operander.                | `OP{3}`
 \I       | ignorera versalisering |                                                    | `EXPR\I`
 \O{}     | output                 | Vilken infångningsgrupp som ska ges som output.    | `EXPR\O{2}` Default: `\O{0}` (hela matchningen)

#### Grammatik

```
<prgm>      ::= <expr>
<expr>      ::=  <op> | <op><expr>
<op>        ::= <ig-caps> | <sub-op>
<sub-op>    ::= <group> | <greedy> | <or> | <str>
<str>       ::= <star> | <count> | (<dot> | <char>)+
<ig-caps>   ::= <sub-op> "\I"
<group>     ::= "(" <expr> ")"
<or>        ::= <str> "+" <str>
<count>     ::= <char> "{N}" | <dot> "{N}"
<star>      ::= <dot> "*" | <char> "*"
<dot>       ::= "."
<char>      ::= "letter"
<greedy>    ::= <star>
``` 
