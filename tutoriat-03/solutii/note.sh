#!/bin/bash
# ==============================================================================
# USO - Tutoriat 03: Solutiile exercitiilor de shell scripting
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Rulare:   bash note.sh note.txt     (sau: chmod +x note.sh && ./note.sh note.txt)
#
# Fisierul note.txt are pe fiecare linie numele unui student si 3 note:
#     ana 9 10 8
#     bogdan 4 5 3
# ==============================================================================

# Ruleaza scriptul din folderul in care se afla (acolo sunt note.txt si ./exercitiu).

ex() { printf '\n\033[1;36m=== Ex %s ===\033[0m\n' "$1"; }

ex "1: Verifica argumentul primit"
# $# = cate argumente a primit scriptul. Mesajele de eroare merg pe stderr (>&2).
if [ $# -ne 1 ]; then
    echo "Utilizare: $0 <fisier_note>" >&2
    exit 1
fi
# -f = exista si este fisier obisnuit. "$1" intre ghilimele: merge si cu spatii in nume.
if [ ! -f "$1" ]; then
    echo "Eroare: fisierul '$1' nu exista" >&2
    exit 1
fi
echo "OK, citesc notele din $1"

ex "2: Cati studenti sunt in fisier?"
# $( ) ruleaza comanda si pune ce a afisat ea in variabila. Fara spatii la =.
n=$(wc -l < "$1")
echo "Sunt $n studenti in $1"

ex "3: Cate linii are fiecare fisier din folder?"
# for ia pe rand fiecare cuvant din lista; shell-ul transforma *.txt in numele fisierelor
for f in *.c *.sh *.txt; do
    echo "$f: $(wc -l < "$f") linii"
done

ex "4: Media fiecarui student, calculata in bash"
# read imparte fiecare linie in cuvinte: primul in nume, apoi n1, n2, n3.
# $(( )) face calcule, dar DOAR cu numere intregi: 29 / 3 = 9, nu 9.67.
while read -r nume n1 n2 n3; do
    echo "$nume: $(( (n1 + n2 + n3) / 3 ))"
done < "$1"

ex "5: Cine a promovat? Intrebam programul C"
if [ -x ./exercitiu ]; then
    promovati=0
    while read -r nume n1 n2 n3; do
        media=$(./exercitiu "$n1" "$n2" "$n3")   # stdout-ul programului intra in variabila
        if [ $? -eq 0 ]; then                    # $? = codul de iesire al lui ./exercitiu
            echo "$nume: $media promovat"
            promovati=$(( promovati + 1 ))
        else
            echo "$nume: $media restant"
        fi
    done < "$1"
    echo "Promovati: $promovati din $n"
else
    echo "Compileaza intai programul C cu: make"
fi
