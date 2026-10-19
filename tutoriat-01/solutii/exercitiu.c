/**
 * USO - Tutoriat 01: Concepte Fundamentale de SO & Linia de Comanda Linux
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutie completa demonstrativa pentru:
 *       - inspectarea argumentelor din linia de comanda (argc, argv)
 *       - interogarea variabilelor de mediu (getenv)
 *       - citirea robusta a fisierelor text linie cu linie (fopen, fgets, fclose)
 *       - parsarea de text si contorizarea statistica (linii, cuvinte, caractere)
 *       - utilizarea corecta a fluxurilor standard (stdout, stderr) si codurilor de retur
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024

/**
 * Afiseaza instructiunile de utilizare ale programului la fluxul stderr.
 * In UNIX, mesajele de eroare si instructiunile de utilizare se trimit pe descriptorul 2 (stderr),
 * nu pe descriptorul 1 (stdout), pentru a nu polua iesirea de date in conducte (pipes).
 */
void print_usage(const char *prog_name) {
    fprintf(stderr, "Utilizare: %s <cale_fisier_intrare>\n", prog_name);
    fprintf(stderr, "Exemplu:   %s date.in\n", prog_name);
}

/**
 * Functie pentru inspectarea variabilelor de mediu ale procesului curent.
 * Fiecare proces Linux mosteneste o copie a tabelei de mediu a procesului parinte (shell-ul).
 */
void print_environment_info(void) {
    printf("========================================================\n");
    printf("        INFORMATII DESPRE MEDIUL DE EXECUTIE            \n");
    printf("========================================================\n");

    /* 1. Numele utilizatorului conectat */
    const char *user = getenv("USER");
    if (user == NULL) {
        user = getenv("LOGNAME");
    }
    if (user != NULL) {
        printf("[+] Utilizator curent   : %s\n", user);
    } else {
        printf("[+] Utilizator curent   : (nedefinit in mediu)\n");
    }

    /* 2. Directorul personal ($HOME) */
    const char *home = getenv("HOME");
    if (home != NULL) {
        printf("[+] Director personal   : %s\n", home);
    } else {
        printf("[+] Director personal   : (nedefinit in mediu)\n");
    }

    /* 3. Terminalul curent ($TERM) */
    const char *term = getenv("TERM");
    if (term != NULL) {
        printf("[+] Tip terminal ($TERM): %s\n", term);
    }

    /* 4. Variabila $PATH si explicatia de securitate */
    const char *path = getenv("PATH");
    if (path != NULL) {
        printf("[+] PATH (primii 70 B)  : %.70s...\n", path);
        /* Verificam daca directorul curent '.' sau '::' se afla in PATH */
        if (strstr(path, ":.:") != NULL || strncmp(path, ".:", 2) == 0 || strstr(path, "::") != NULL) {
            printf("[!] ATENTIE: Directorul curent '.' a fost gasit in PATH! (Risc de securitate)\n");
        } else {
            printf("[i] Notă: Directorul curent '.' NU este in PATH (Buna practica de securitate).\n");
            printf("    De aceea executia fisierului local necesita prefixul './' (ex: ./exercitiu)\n");
        }
    }

    printf("========================================================\n\n");
}

/**
 * Numara cuvintele dintr-un sir de caractere.
 * Un cuvant este definit ca o secventa contigua de caractere non-spatiu.
 */
static int count_words_in_string(const char *str) {
    int words = 0;
    int in_word = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    return words;
}

/**
 * Analizeaza fisierul specificat:
 * - deschide fisierul pentru citire
 * - parcurge linie cu linie folosind un buffer fix
 * - contorizeaza liniile, cuvintele si caracterele
 * - afiseaza progresul si statisticile agregate
 */
int analyze_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        /* perror afiseaza mesajul nostru urmat de descrierea textuala a erorii din errno */
        perror("Eroare la deschiderea fisierului de intrare");
        fprintf(stderr, "Fisierul specificat nu a putut fi accesat: %s\n", filename);
        return -1;
    }

    printf("Analizam fisierul: %s\n", filename);
    printf("--------------------------------------------------------\n");

    char buffer[MAX_LINE_LEN];
    int line_count = 0;
    int word_count = 0;
    int total_chars = 0;

    /* Citim linie cu linie pana cand intalnim EOF sau o eroare */
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
        size_t len = strlen(buffer);
        total_chars += (int)len;

        int words_on_line = count_words_in_string(buffer);
        word_count += words_on_line;

        /* Eliminam newline-ul de la final pentru afisare estetica */
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        printf("[%2d] (%2d cuv, %3zu B): %s\n", line_count, words_on_line, len, buffer);
    }

    /* Inchiderea obligatorie a descriptorului de fisier */
    if (fclose(file) != 0) {
        perror("Avertisment: Eroare la apelul fclose");
    }

    printf("--------------------------------------------------------\n");
    printf("STATISTICI FINALE:\n");
    printf("  [+] Linii citite     : %d\n", line_count);
    printf("  [+] Cuvinte gasite   : %d\n", word_count);
    printf("  [+] Octeti procesati : %d\n", total_chars);
    printf("========================================================\n");

    return 0;
}

int main(int argc, char *argv[]) {
    /* Verificare validitate argumente */
    if (argc < 2) {
        fprintf(stderr, "Eroare: Numar insuficient de argumente! (primit: %d, asteptat: minim 2)\n\n", argc);
        print_usage(argv[0]);
        /* Returnam 1 (EXIT_FAILURE) pentru a semnala esecul in shell (echo $? va fi 1) */
        return EXIT_FAILURE;
    }

    /* 1. Afisam informatiile despre mediul procesului */
    print_environment_info();

    /* 2. Procesam fisierul dat ca prim argument */
    const char *target_file = argv[1];
    int status = analyze_file(target_file);

    if (status != 0) {
        fprintf(stderr, "\n[ESEC] Procesarea fisierului '%s' a esuat.\n", target_file);
        return EXIT_FAILURE;
    }

    printf("\n[SUCCES] Programul s-a incheiat cu succes.\n");
    /* Returnam 0 (EXIT_SUCCESS) - in shell echo $? va fi 0 */
    return EXIT_SUCCESS;
}
