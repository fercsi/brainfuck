#!/usr/local/bin/brainfuck
,
This  test program  checks if  debug instructiins  work as  expected; It
prints  "ON" if  debug  mode  is turned  on  and  "OFF" (more  precisely
"ON\bFF") if  debug mode is  turned off; Also  it prints memory  dump to
stderr in debug mode

[[->+>+<<]>>[-<<+>>]<-]     Fill memory with decreasing value
@                           Dump memory
>++++++++[-<++++++++++>]<-. O
-.                          N
$                           Exit program
>++++++++.<                 {BS}
--------.                   F
.                           F
