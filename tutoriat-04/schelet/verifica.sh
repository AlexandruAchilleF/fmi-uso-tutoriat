#!/bin/bash
# ==============================================================================
# USO - Tutoriat 04: Exercitii de scripting
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Rulare:   make script      (sau: bash verifica.sh)
# Merge si pe Linux, si pe macOS (bash 3.2, fara nimic in plus de instalat).
#
# Toate exercitiile folosesc programul C: ./exercitiu <sursa> <destinatie>
# ==============================================================================

ex() { printf '\n\033[1;36m=== Ex %s ===\033[0m\n' "$1"; }

# Inlocuieste fiecare linie `todo` cu codul tau.
# Ruleaza scriptul din nou dupa fiecare exercitiu ca sa-ti vezi rezultatul.
todo() { echo "(necompletat)"; }

if [ ! -x ./exercitiu ]; then
    echo "Compileaza intai programul C cu: make" >&2
    exit 1
fi

ex "1: O eroare, prinsa in fisier"
# TODO: ruleaza  ./exercitiu lipsa.txt copie.txt  cu stderr trimis in erori.txt (indiciu: 2>)
# TODO: salveaza imediat codul de iesire: cod=$?
# TODO: afiseaza "Codul de iesire: $cod" si continutul lui erori.txt
#       Ce cod te astepti sa vezi? Ce scrie in erori.txt?
todo

ex "2: Copia este identica?"
# TODO: copiaza mesaj.txt in copie.txt cu ./exercitiu
# TODO: verifica cu  if cmp -s mesaj.txt copie.txt; then ... else ... fi
#       cmp -s nu afiseaza nimic: raspunde doar prin $? (0 = identice)
todo
