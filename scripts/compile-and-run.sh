#!/bin/bash
# if running on windows, end of line sequence must be CRLF. If on linux, it must be LF
echo "Compilando"
gcc projeto.c -o ./output/projeto &&
echo "Executando"
./output/projeto