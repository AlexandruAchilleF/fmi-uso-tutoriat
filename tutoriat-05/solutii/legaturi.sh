#!/bin/bash
# ==============================================================================
# USO - Tutoriat 05: Exercitii cu link-uri
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Rulare:   make script      (sau: make demo && bash legaturi.sh)
# Merge si pe Linux, si pe macOS (bash 3.2, fara nimic in plus de instalat).
#
# Ex 1 lucreaza in folderul lab/ (il facem de la zero de fiecare data).
# Ex 2 foloseste programul C pe folderul demo/ (facut de `make demo`).
# ==============================================================================

ex() { printf '\n\033[1;36m=== Ex %s ===\033[0m\n' "$1"; }

if [ ! -x ./exercitiu ] || [ ! -d demo ]; then
    echo "Compileaza intai programul si fa folderul demo/ cu: make && make demo" >&2
    exit 1
fi

ex "1: Hard link vs symlink"
rm -rf lab
mkdir lab
echo "Text important" > lab/original.txt

ln lab/original.txt lab/hard.txt        # hard link: alt nume pentru ACELASI i-nod
ln -s original.txt lab/soft.txt         # symlink: un fisier mic care tine minte un NUME
ls -li lab                              # coloana 1 = i-nodul, coloana 3 = cate nume are

rm lab/original.txt                     # stergem un nume, nu fisierul
echo "hard.txt contine: $(cat lab/hard.txt)"
if [ -e lab/soft.txt ]; then
    echo "soft.txt merge"
else
    echo "soft.txt e un link rupt: arata spre original.txt, care nu mai exista"
fi

ex "2: Gaseste hard link-urile cu programul C"
# Doua nume cu acelasi i-nod = acelasi fisier. uniq -d afiseaza doar ce se repeta.
inod=$(./exercitiu demo | cut -d' ' -f1 | sort | uniq -d)
echo "I-nodul care apare de doua ori: $inod"
./exercitiu demo | grep "^$inod "
