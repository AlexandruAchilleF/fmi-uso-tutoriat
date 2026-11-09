#!/bin/bash
# ==============================================================================
# USO - Tutoriat 03: Exercitii de shell scripting
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

# Inlocuieste fiecare linie `todo` cu codul tau.
# Ruleaza scriptul din nou dupa fiecare exercitiu ca sa-ti vezi rezultatul.
todo() { echo "(necompletat)"; }

ex "1: Verifica argumentul primit"
# TODO: daca $# nu este 1, afiseaza "Utilizare: $0 <fisier_note>" pe stderr (>&2) si exit 1
# TODO: daca "$1" nu este un fisier (indiciu: [ ! -f "$1" ]), afiseaza o eroare pe stderr si exit 1
#       Atentie la spatii: [ $# -ne 1 ], nu [$# -ne 1]
todo
echo "OK, citesc notele din $1"

ex "2: Media fiecarui student, calculata in bash"
# TODO: citeste "$1" linie cu linie:  while read -r nume n1 n2 n3; do ... done < "$1"
#       si afiseaza media intreaga cu $(( (n1 + n2 + n3) / 3 )), de forma: ana: 9
#       Observa ce iese la diana (10 9 10). De ce?
todo

ex "3: Cine a promovat? Intrebam programul C"
if [ -x ./exercitiu ]; then
    promovati=0
    total=0
    # TODO: acelasi while read ca la ex. 2, dar pentru fiecare student:
    #         media=$(./exercitiu "$n1" "$n2" "$n3")
    #         daca $? este 0 -> "ana: 9.00 promovat" si promovati=$(( promovati + 1 ))
    #         altfel         -> "bogdan: 4.00 restant"
    #       si la fiecare student: total=$(( total + 1 ))
    todo
    echo "Promovati: $promovati din $total"
else
    echo "Compileaza intai programul C cu: make"
fi
