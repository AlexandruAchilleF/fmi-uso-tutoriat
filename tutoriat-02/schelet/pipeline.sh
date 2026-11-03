#!/bin/bash
# ==============================================================================
# USO - Tutoriat 02: Exercitii cu filtre, pipe-uri si redirectari
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

# Inlocuieste fiecare linie `todo` cu comanda ta (un singur rand, de obicei un pipe).
# Ruleaza scriptul din nou dupa fiecare exercitiu ca sa-ti vezi rezultatul.
todo() { echo "(necompletat)"; }

ex "1: Cate linii are jurnal.txt?"
# TODO: wc -l, dar fara sa apara si numele fisierului in rezultat (indiciu: <)
todo

ex "2: Afiseaza doar incercarile esuate"
# TODO: grep
todo

ex "3: Cate incercari esuate sunt?"
# TODO: o singura comanda, fara wc (indiciu: man grep, cauta optiunea -c)
todo

ex "4: Lista utilizatorilor distincti, in ordine alfabetica"
# TODO: cut -d' ' -f3 extrage al treilea camp; apoi sortare fara dubluri
todo

ex "5: Cate actiuni a facut fiecare utilizator, descrescator"
# TODO: cut | sort | uniq -c | sort ...  (de ce e nevoie de primul sort?)
todo

ex "6: Utilizatorul cu cele mai multe esecuri"
# TODO: porneste de la ex. 2 si ex. 5, pastreaza doar prima linie (head)
todo

ex "7: Salveaza intrarile din 28 octombrie in ziua2.out, apoi adauga o linie de final"
# TODO: grep '^2026-10-28' ... > ziua2.out, apoi echo "--- sfarsit ---" >> ziua2.out
#       La final afiseaza ultimele 3 linii din ziua2.out (tail)
todo

ex "8: Primii 5 utilizatori din /etc/passwd si shell-ul lor"
# TODO: separatorul in /etc/passwd este ':'; numele e campul 1, shell-ul campul 7
todo

ex "9: Primele 3 linii din date.in, cu majuscule"
# TODO: tr 'a-z' 'A-Z' citeste doar de la stdin
todo

ex "10: Afiseaza exact textul: Pretul este \$5 * 2"
# TODO: echo cu ghilimelele potrivite (incearca intai fara ghilimele si vezi ce iese)
todo

ex "11: Listeaza /etc/hostname si /nu/exista, dar ascunde erorile"
# TODO: ls ... cu stderr trimis in /dev/null
todo

ex "12: Filtrul C: liniile cu 'linux' (orice capitalizare) separat de statistici"
if [ -x ./exercitiu ]; then
    # TODO: rulati ./exercitiu -i linux date.in cu stdout in gasite.out si stderr in erori.out
    todo
    echo "Cod de iesire: $?"
    echo "-- gasite.out (stdout):"; cat gasite.out 2> /dev/null
    echo "-- erori.out (stderr):";  cat erori.out 2> /dev/null
else
    echo "Compileaza intai programul C cu: make"
fi

ex "13: Filtrul C intr-un pipe: cate linii NU contin 'comanda'?"
if [ -x ./exercitiu ]; then
    # TODO: ./exercitiu -v -i comanda date.in, fara statistici pe ecran, numarat cu wc -l
    todo
fi
