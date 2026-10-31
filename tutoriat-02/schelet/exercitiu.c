/**
 * USO - Tutoriat 02: Utilitare Text, Filtre, Despecializare si Redirectari
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru un "mini-grep" scris in C, care se comporta ca un
 *       filtru UNIX adevarat:
 *       - citeste din fisierul primit ca argument SAU din stdin (cand lipseste)
 *       - scrie liniile potrivite pe stdout (datele, care pot merge mai departe intr-un pipe)
 *       - scrie diagnosticul si statisticile pe stderr (nu polueaza pipe-ul)
 *       - intoarce coduri de iesire identice cu grep: 0 = gasit, 1 = negasit, 2 = eroare
 *
 * Exemple de rulare:
 *       ./exercitiu linux date.in
 *       ./exercitiu -i -n linux date.in
 *       cat date.in | ./exercitiu -v shell
 *       ./exercitiu -c -i kernel < date.in
 *       ./exercitiu -i linux date.in 2> /dev/null | sort | uniq | wc -l
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024

/* Coduri de iesire cu aceeasi semnificatie ca la grep (vezi `man grep`, sectiunea EXIT STATUS) */
#define COD_GASIT   0
#define COD_NEGASIT 1
#define COD_EROARE  2

/* Optiunile primite din linia de comanda */
struct optiuni {
    int ignora_majuscule;  /* -i : "Linux" se potriveste cu "linux" */
    int inverseaza;        /* -v : afiseaza liniile care NU contin tiparul */
    int numerotare;        /* -n : prefixeaza fiecare linie cu numarul ei */
    int doar_numar;        /* -c : afiseaza doar cate linii s-au potrivit */
    const char *tipar;     /* sirul cautat */
    const char *fisier;    /* NULL inseamna: citim de la stdin */
};

/**
 * Instructiunile de utilizare merg pe stderr (descriptorul 2), ca sa nu ajunga
 * din greseala in fisierul in care cineva a redirectat stdout.
 */
static void print_usage(const char *prog_name) {
    fprintf(stderr, "Utilizare: %s [-i] [-v] [-n] [-c] <tipar> [fisier]\n", prog_name);
    fprintf(stderr, "  -i   ignora diferenta dintre litere mari si mici\n");
    fprintf(stderr, "  -v   afiseaza liniile care NU contin tiparul\n");
    fprintf(stderr, "  -n   afiseaza si numarul liniei\n");
    fprintf(stderr, "  -c   afiseaza doar numarul de linii potrivite\n");
    fprintf(stderr, "Fara [fisier] (sau cu '-'), programul citeste de la intrarea standard (stdin).\n");
    fprintf(stderr, "Exemple: %s -n linux date.in\n", prog_name);
    fprintf(stderr, "         cat date.in | %s -i kernel\n", prog_name);
}

/**
 * Parseaza argumentele. Optiunile (cele care incep cu '-') vin primele,
 * apoi tiparul, apoi optional numele fisierului.
 * Intoarce 0 la succes, -1 daca argumentele sunt gresite.
 */
static int parseaza_argumente(int argc, char *argv[], struct optiuni *opt) {
    int i = 1;

    memset(opt, 0, sizeof(*opt));

    for (; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        if (strcmp(argv[i], "--") == 0) { /* "--" marcheaza sfarsitul optiunilor */
            i++;
            break;
        }
        /* Permitem si optiuni lipite, ca la utilitarele reale: -in, -vc etc. */
        for (const char *c = argv[i] + 1; *c != '\0'; c++) {
            switch (*c) {
            case 'i': opt->ignora_majuscule = 1; break;
            case 'v': opt->inverseaza = 1; break;
            case 'n': opt->numerotare = 1; break;
            case 'c': opt->doar_numar = 1; break;
            default:
                fprintf(stderr, "%s: optiune necunoscuta: -%c\n", argv[0], *c);
                return -1;
            }
        }
    }

    if (i >= argc) {
        fprintf(stderr, "%s: lipseste tiparul de cautat\n", argv[0]);
        return -1;
    }
    opt->tipar = argv[i++];

    if (i < argc) {
        /* "-" este conventia UNIX pentru "citeste de la stdin" */
        opt->fisier = (strcmp(argv[i], "-") == 0) ? NULL : argv[i];
        i++;
    }

    if (i < argc) {
        fprintf(stderr, "%s: prea multe argumente (accept un singur fisier)\n", argv[0]);
        return -1;
    }
    return 0;
}

/**
 * Verifica daca 'tipar' apare ca subsir in 'linie'.
 * Cu ignora_majuscule != 0, comparatia se face dupa tolower() pe fiecare caracter.
 * (strcasestr() ar face acelasi lucru, dar este extensie GNU, nu C standard.)
 */
static int contine(const char *linie, const char *tipar, int ignora_majuscule) {
    size_t n = strlen(tipar);

    if (n == 0) {
        return 1; /* tiparul gol se potriveste cu orice linie, ca la grep "" */
    }

    if (!ignora_majuscule) {
        /* TODO 1: Cautarea simpla (cu diferenta intre litere mari si mici).
         * Folositi strstr(linie, tipar) (vezi `man 3 strstr`): intoarce NULL daca
         * tiparul nu apare in linie. Returnati 1 daca apare, 0 altfel.
         */
        return 0; /* TODO: inlocuiti cu apelul strstr() */
    }

    /* TODO 2: Cautarea fara diferenta intre litere mari si mici (optiunea -i).
     * Pentru fiecare pozitie p din linie (for (const char *p = linie; *p != '\0'; p++)):
     *   - comparati caracter cu caracter p[k] cu tipar[k], pentru k = 0, 1, ...
     *     folosind tolower((unsigned char)p[k]) == tolower((unsigned char)tipar[k])
     *   - opriti-va la prima nepotrivire sau cand p[k] == '\0'
     *   - daca ati ajuns la k == n, tiparul a fost gasit: return 1
     * Daca bucla se termina fara potrivire: return 0.
     */
    (void)linie; /* Eliminati acest cast dupa implementare */

    return 0;
}

/**
 * Citeste fluxul 'in' linie cu linie si afiseaza pe stdout liniile potrivite.
 * Intoarce numarul de linii potrivite; in *total pune numarul de linii citite.
 */
static int filtreaza(FILE *in, const struct optiuni *opt, int *total) {
    char buffer[MAX_LINE_LEN];
    int potrivite = 0;

    *total = 0;
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        (*total)++;

        /* Eliminam '\n' de la final; il punem la loc la afisare.
         * Asa tratam la fel si ultima linie, chiar daca fisierul nu se termina cu Enter. */
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        int gasit = contine(buffer, opt->tipar, opt->ignora_majuscule);

        /* TODO 3: Optiunea -v (opt->inverseaza).
         * Daca este activa, inversati rezultatul: gasit = !gasit;
         */

        if (!gasit) {
            continue;
        }

        potrivite++;

        /* TODO 4: Afisarea liniei pe stdout.
         *   - daca opt->doar_numar este activ (-c), NU afisati linia (continue;)
         *   - daca opt->numerotare este activ (-n), afisati intai printf("%d:", *total);
         *   - afisati linia: printf("%s\n", buffer);
         */
    }

    /* TODO 5: Optiunea -c.
     * Daca opt->doar_numar este activ, afisati pe stdout doar numarul: printf("%d\n", potrivite);
     */

    return potrivite;
}

int main(int argc, char *argv[]) {
    struct optiuni opt;

    /* Verificarea argumentelor (gata scrisa) */
    if (parseaza_argumente(argc, argv, &opt) != 0) {
        print_usage(argv[0]);
        return COD_EROARE;
    }

    /* TODO 6: Alegeti sursa datelor.
     * Pornim de la in = stdin (fluxul deja deschis de sistem pentru descriptorul 0).
     * Daca opt.fisier != NULL, deschideti fisierul cu fopen(opt.fisier, "r").
     * Daca fopen intoarce NULL:
     *     fprintf(stderr, "%s: ", argv[0]);
     *     perror(opt.fisier);
     *     return COD_EROARE;
     */
    FILE *in = stdin;

    int total = 0;
    int potrivite = filtreaza(in, &opt, &total);

    /* TODO 7: Inchideti fisierul cu fclose(in), DAR doar daca in != stdin
     * (stdin nu l-am deschis noi). Daca fclose intoarce != 0: perror("Avertisment: fclose");
     */

    /* TODO 8: Afisati statisticile pe STDERR (nu pe stdout!), ca sa nu ajunga in pipe:
     *     fprintf(stderr, "[filtru] %d din %d linii potrivite (sursa: %s)\n",
     *             potrivite, total, opt.fisier != NULL ? opt.fisier : "stdin");
     */
    (void)potrivite; /* Eliminati dupa ce folositi variabila */

    /* TODO 9: Codul de iesire, ca la grep:
     *   COD_GASIT (0) daca potrivite > 0, altfel COD_NEGASIT (1).
     */
    return COD_GASIT;
}
