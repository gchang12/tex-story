#!/bin/bash
cmd="pdflatex main";
src="prose";
tgt="scenes"
cp -r "$src" "$tgt";
for i in $(ls "$src"); do perl ~/Documents/coding/linux/convert-to-tex/tex-converter.pl "$src/$i" "$tgt/${i/.txt/.tex}"; done;
"$cmd" && "$cmd";
