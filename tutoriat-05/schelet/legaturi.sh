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

# Inlocuieste fiecare linie `todo` cu codul tau.
# Ruleaza scriptul din nou dupa fiecare exercitiu ca sa-ti vezi rezultatul.
todo() { echo "(necompletat)"; }

if [ ! -x ./exercitiu ] || [ ! -d demo ]; then
    echo "Compileaza intai programul si fa folderul demo/ cu: make && make demo" >&2
    exit 1
fi

ex "1: Hard link vs symlink"
rm -rf lab
mkdir lab
echo "Text important" > lab/original.txt
# TODO: fa un hard link lab/hard.txt spre lab/original.txt          (indiciu: ln)
# TODO: fa un symlink lab/soft.txt spre original.txt                (indiciu: ln -s)
#       Atentie: tinta unui symlink se citeste din folderul link-ului,
#       deci scrii "original.txt", nu "lab/original.txt".
# TODO: afiseaza folderul cu i-noduri:  ls -li lab
#       Ce au in comun original.txt si hard.txt? Dar soft.txt?
# TODO: sterge lab/original.txt, apoi afiseaza continutul lui lab/hard.txt
# TODO: verifica cu  if [ -e lab/soft.txt ]; then ... else ... fi  daca soft.txt mai merge
todo

ex "2: Gaseste hard link-urile cu programul C"
# TODO: din iesirea lui  ./exercitiu demo  pastreaza doar numerele de i-nod (cut -d' ' -f1),
#       sorteaza-le si pastreaza doar ce se repeta (sort | uniq -d). Pune rezultatul in $inod:
#             inod=$( ... )
# TODO: afiseaza liniile din  ./exercitiu demo  care incep cu acel i-nod:  grep "^$inod "
todo
