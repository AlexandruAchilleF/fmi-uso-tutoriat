/**
 * USO - Tutoriat 03: Shell Scripting in Bash
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutia completa pentru un program mic, gandit sa fie apelat dintr-un script:
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
    /* 1. Avem macar o nota? argc numara si numele programului, deci fara note argc == 1. */
    if (argc < 2) {
        fprintf(stderr, "Utilizare: %s <nota1> [nota2 ...]\n", argv[0]);
        fprintf(stderr, "Exemplu:   %s 9 10 8\n", argv[0]);
        return COD_EROARE;
    }

    /* 2. Citim si adunam notele. La prima nota gresita ne oprim cu eroare. */
    int suma = 0;
    for (int i = 1; i < argc; i++) {
        int nota;
        if (citeste_nota(argv[i], &nota) != 0) {
            fprintf(stderr, "%s: nota invalida: '%s' (accept numere intregi de la %d la %d)\n",
                    argv[0], argv[i], NOTA_MIN, NOTA_MAX);
            return COD_EROARE;
        }
        suma += nota;
    }

    /* 3. Media. Impartim ca double: 29 / 3 in int ar da 9, nu 9.67. */
    int numar_note = argc - 1;
    double media = (double)suma / numar_note;

    /* 4. Rezultatul (doar numarul) merge pe stdout, ca sa-l poata prinde scriptul cu $( ). */
    printf("%.2f\n", media);

    /* 5. Codul de iesire spune scriptului daca studentul a promovat. */
    return media >= 5.0 ? COD_PROMOVAT : COD_RESTANT;
}
