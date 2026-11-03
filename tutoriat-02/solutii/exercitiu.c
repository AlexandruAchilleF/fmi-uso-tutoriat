/**
 * USO - Tutoriat 02: Utilitare Text, Filtre, Despecializare si Redirectari
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutia completa a unui "mini-grep" scris in C, care se comporta ca un
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
        return strstr(linie, tipar) != NULL;
    }

    for (const char *p = linie; *p != '\0'; p++) {
        size_t k = 0;
        while (k < n && p[k] != '\0' &&
               tolower((unsigned char)p[k]) == tolower((unsigned char)tipar[k])) {
            k++;
        }
        if (k == n) {
            return 1;
        }
    }
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
        if (opt->inverseaza) {
            gasit = !gasit;
        }
        if (!gasit) {
            continue;
        }

        potrivite++;
        if (opt->doar_numar) {
            continue; /* la -c nu afisam liniile, doar numarul lor la final */
        }
        if (opt->numerotare) {
            printf("%d:", *total);
        }
        printf("%s\n", buffer);
    }

    if (opt->doar_numar) {
        printf("%d\n", potrivite);
    }
    return potrivite;
}

int main(int argc, char *argv[]) {
    struct optiuni opt;

    if (parseaza_argumente(argc, argv, &opt) != 0) {
        print_usage(argv[0]);
        return COD_EROARE;
    }

    /* 1. Alegem sursa datelor: fisierul primit sau intrarea standard. */
    FILE *in = stdin;
    if (opt.fisier != NULL) {
        in = fopen(opt.fisier, "r");
        if (in == NULL) {
            /* perror adauga explicatia lui errno, ex: "No such file or directory" */
            fprintf(stderr, "%s: ", argv[0]);
            perror(opt.fisier);
            return COD_EROARE;
        }
    }

    /* 2. Filtram. */
    int total = 0;
    int potrivite = filtreaza(in, &opt, &total);

    /* 3. Inchidem fisierul. stdin NU il inchidem: nu l-am deschis noi. */
    if (in != stdin && fclose(in) != 0) {
        perror("Avertisment: fclose");
    }

    /* 4. Statisticile merg pe stderr: in `./exercitiu x f | wc -l` nu ajung la wc,
     *    dar tot le vezi pe ecran. Le poti ascunde cu 2> /dev/null. */
    fprintf(stderr, "[filtru] %d din %d linii potrivite (sursa: %s)\n",
            potrivite, total, opt.fisier != NULL ? opt.fisier : "stdin");

    /* 5. Codul de iesire, ca la grep: permite `./exercitiu x f && echo gasit`. */
    return potrivite > 0 ? COD_GASIT : COD_NEGASIT;
}
