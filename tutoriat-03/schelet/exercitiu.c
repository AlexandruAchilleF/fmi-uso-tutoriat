/**
 * USO - Tutoriat 03: Shell Scripting in Bash
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru un program mic, gandit sa fie apelat dintr-un script:
 *       - primeste notele ca argumente in linia de comanda (argv[1], argv[2], ...)
 *       - afiseaza media pe stdout, cu 2 zecimale (scriptul o poate prinde cu $( ))
 *       - scrie erorile pe stderr
 *       - intoarce un cod de iesire pe care scriptul il testeaza cu if:
 *             0 = promovat (media >= 5), 1 = restant, 2 = eroare
 *
 * Legatura cu Bash:
 *       argc - 1   <->  $#   (cate argumente am primit)
 *       argv[1]    <->  $1   (primul argument)
 *       return x;  <->  $?   (codul de iesire, citit de shell)
 *
 * Exemple de rulare:
 *       ./exercitiu 9 10 8          -> 9.00, cod 0
 *       ./exercitiu 4 5 3           -> 4.00, cod 1
 *       ./exercitiu 7 abc           -> eroare pe stderr, cod 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define COD_PROMOVAT 0
#define COD_RESTANT  1
#define COD_EROARE   2

#define NOTA_MIN 1
#define NOTA_MAX 10

/**
 * Transforma textul primit in argv intr-o nota intre 1 si 10.
 * Intoarce 0 la succes (si pune nota in *nota), -1 daca textul nu e o nota valida.
 * Exemple: "9" -> 9, "10" -> 10, "abc" -> -1, "7x" -> -1, "11" -> -1.
 */
static int citeste_nota(const char *text, int *nota) {
    char *final;

    errno = 0;
    long v = strtol(text, &final, 10);     /* final arata unde s-a oprit conversia */
    if (final == text || *final != '\0' || errno != 0) {
        return -1;                         /* gol, litere dupa cifre sau numar urias */
    }
    if (v < NOTA_MIN || v > NOTA_MAX) {
        return -1;
    }
    *nota = (int)v;
    return 0;
}

int main(int argc, char *argv[]) {
    /* TODO 1: Daca nu am primit nicio nota (argc < 2), afiseaza pe stderr:
     *             Utilizare: ./exercitiu <nota1> [nota2 ...]
     *         si intoarce COD_EROARE.
     *         Atentie: argc numara si numele programului, deci fara note argc == 1. */

    int suma = 0;
    for (int i = 1; i < argc; i++) {
        int nota;
        /* TODO 2: Transforma argv[i] in nota cu citeste_nota(argv[i], &nota).
         *         Daca intoarce -1, afiseaza pe stderr "nota invalida: '...'"
         *         si intoarce COD_EROARE. */

        /* TODO 3: Adauga nota la suma. */
        (void)nota; /* sterge linia asta dupa ce rezolvi TODO 2 si 3 */
    }

    /* TODO 4: Calculeaza media ca double si afiseaz-o pe stdout cu 2 zecimale (%.2f).
     *         Indiciu: (double)suma / (argc - 1). Fara (double), 29 / 3 ar da 9. */
    (void)suma;

    /* TODO 5: Intoarce COD_PROMOVAT daca media >= 5, altfel COD_RESTANT.
     *         Scriptul note.sh citeste acest cod cu $?. */
    return COD_EROARE;
}
