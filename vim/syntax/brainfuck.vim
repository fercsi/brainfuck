"" # Brainfuck Syntax
"    ══════════════════════════════════════════════════════
"       VIM Configuration
"       © 1999-2023, Ferenc Vajda. All rights reserved.
"     ────────────────────────────────────────────────────
"       syntax/brainfuck.vim
"       Syntax settings for BrainFuck
"    ══════════════════════════════════════════════════════    */
"------------------------------------------------------------------File info-"[
"@ > - **File**: syntax/brainfuck.vim
"@ > - **Author**: VAJDA, Ferenc
"@ > - **Last modified**: 2023-02-26T19:19:52+0000
"]

syn match    bfInstruction  /[-+,.<>[\]]/
syn match    bfError        /[-+,.<>[\]]/ contained
syn match    bfExtended     /[@$]/
syn keyword  bfTodo         TODO FIXME XXX contained
syn region   bfComment      start="#" end="$" keepend contains=bfTodo,bfError
syn region   bfText         start=/[^ \t+,.<>[\]#@\$-]/ end="$" keepend contains=bfError

hi def link bfInstruction  Statement
hi def link bfExtended     SpecialChar
hi def link bfComment      Comment
hi def link bfTodo         Todo
hi def link bfError        Error
