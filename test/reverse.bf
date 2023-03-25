#!/usr/local/bin/brainfuck

This  test program  reads  input line  by line  and  prints each  line's
characters in reversed order

                        cell 0 stops iteration
>                       move to cell 1
+++++ +++++             ascii LF to the end of the reversef line
>                       move to first position
,                       read character
[                       enter if not NULL (EOF)
    ----- -----         subtract 10 to check if LF
    [                   if not:
        +++++ +++++     add the removed 10
        >               move to next position
        ,               read character
        ----- -----     LF?
    ]
    <                   Last character was LF; we don't need that
    [                   repeat until cell0=0 reached
        .               print
        [-]             clean not used cell
        <               move backwards to reverse text
    ]
    >                   restart: wait for next row
    +++++ +++++
    >
    ,
]
>+++++ +++++[-<+++++ +++++>]<--. b
>++++[-<++++++>]<-.              y
>++++[-<----->]<.                e
<.                               \n
