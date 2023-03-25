## # Brainfuck by FERcsI
#    ══════════════════════════════════════════════════════
#       Brainfuck interpreter
#       Copyright (c) 2023 Ferenc Vajda
#     ────────────────────────────────────────────────────
#       Makefile
#       Compiling, testing and installation
#     ────────────────────────────────────────────────────
#       This code is licensed under the MIT license. See
#       the LICENSE file in the project root for license
#       terms.
#    ══════════════════════════════════════════════════════    */
#------------------------------------------------------------------File info-#[
#@ > - **File**: Makefile
#@ > - **Author**: VAJDA, Ferenc
#@ > - **Last modified**: 2023-03-02T16:33:18+0000
#]

INSTALL_DIR=/usr/local/bin
CP=cp
RM=rm
CC=g++
OPTS=--std=c++17 -lstdc++ -O2 --pedantic

brainfuck: brainfuck.cpp
	$(CC) -o $@ $(OPTS) $<

install: brainfuck
	$(CP) $< $(INSTALL_DIR)/$<

.PHONY: test
test: brainfuck
	./test.sh

clean:
	$(RM) brainfuck
