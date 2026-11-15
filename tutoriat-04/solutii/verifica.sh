#!/bin/bash
# ==============================================================================
# USO - Tutoriat 04: Solutiile exercitiilor de scripting
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Rulare:   make script      (sau: bash verifica.sh)
# Merge si pe Linux, si pe macOS (bash 3.2, fara nimic in plus de instalat).
#
# Toate exercitiile folosesc programul C: ./exercitiu <sursa> <destinatie>
# ==============================================================================

ex() { printf '\n\033[1;36m=== Ex %s ===\033[0m\n' "$1"; }

if [ ! -x ./exercitiu ]; then
    echo "Compileaza intai programul C cu: make" >&2
    exit 1
fi

ex "1: O eroare, prinsa in fisier"
# 2> trimite stderr (mesajul de la perror) in erori.txt, nu pe ecran.
./exercitiu lipsa.txt copie.txt 2> erori.txt
cod=$?                                   # salvam $? imediat, altfel il schimba urmatoarea comanda
echo "Codul de iesire: $cod"
echo "In erori.txt scrie: $(cat erori.txt)"

ex "2: Copia este identica?"
./exercitiu mesaj.txt copie.txt
# cmp -s nu afiseaza nimic; spune doar prin codul de iesire: 0 = identice
if cmp -s mesaj.txt copie.txt; then
    echo "OK: copie.txt este identic cu mesaj.txt"
else
    echo "GRESIT: fisierele difera" >&2
fi

ex "3: Backup pentru toate fisierele .txt"
mkdir -p backup
copii=0
for f in *.txt; do
    if ./exercitiu "$f" "backup/$f" > /dev/null; then   # > /dev/null ascunde mesajul "Am copiat"
        copii=$(( copii + 1 ))
    fi
done
echo "Am facut $copii copii in backup/:"
ls backup
