#!/bin/bash
# ==============================================================================
# USO - Tutoriat 02: Solutiile exercitiilor cu filtre, pipe-uri si redirectari
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Rulare:   bash pipeline.sh        (sau: chmod +x pipeline.sh && ./pipeline.sh)
#
# Fisierul jurnal.txt are pe fiecare linie 5 campuri separate prin spatiu:
#     data       ora      utilizator actiune status
#     2026-10-27 08:02:11 ana        login   ok
# ==============================================================================

# Lucram mereu din directorul scriptului, ca sa gasim jurnal.txt si date.in
cd "$(dirname "$0")" || exit 1

ex() { printf '\n\033[1;36m=== Ex %s ===\033[0m\n' "$1"; }

ex "1: Cate linii are jurnal.txt?"
# wc -l < fisier afiseaza doar numarul (fara numele fisierului, pentru ca wc
# nu stie de unde vin datele: shell-ul i le da pe stdin)
wc -l < jurnal.txt

ex "2: Cate actiuni a facut fiecare utilizator, descrescator"
# uniq -c numara doar dublurile VECINE, de aceea sortam inainte
cut -d' ' -f3 jurnal.txt | sort | uniq -c | sort -nr

ex "3: Salveaza intrarile din 28 octombrie in ziua2.out, apoi adauga o linie de final"
# ^ inseamna "inceput de linie": nu prindem din greseala un 2026-10-28 din alt camp
grep '^2026-10-28' jurnal.txt > ziua2.out     # >  creeaza / suprascrie
echo "--- sfarsit ---" >> ziua2.out           # >> adauga la final
tail -n 3 ziua2.out

ex "4: Filtrul C: liniile cu 'linux' (orice capitalizare) separat de statistici"
if [ -x ./exercitiu ]; then
    ./exercitiu -i linux date.in > gasite.out 2> erori.out
    echo "Cod de iesire: $?"
    echo "-- gasite.out (stdout):"; cat gasite.out
    echo "-- erori.out (stderr):";  cat erori.out
else
    echo "Compileaza intai programul C cu: make"
fi

ex "5: Filtrul C intr-un pipe: cate linii NU contin 'comanda'?"
if [ -x ./exercitiu ]; then
    ./exercitiu -v -i comanda date.in 2> /dev/null | wc -l
fi
