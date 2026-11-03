/**
 * USO - Tutoriat 01: Concepte Fundamentale de SO & Linia de Comanda Linux
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru:
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

    /* TODO 1: Numele utilizatorului conectat.
     * Extrageti variabila "USER" folosind getenv("USER").
     * Daca este NULL, incercati variabila "LOGNAME".
     * Daca ambele sunt NULL, afisati "[+] Utilizator curent   : (nedefinit in mediu)\n".
     * Altfel afisati: "[+] Utilizator curent   : %s\n", user
     */
    const char *user = NULL; /* TODO: apelati getenv() */
    if (user != NULL) {
        printf("[+] Utilizator curent   : %s\n", user);
    } else {
        printf("[+] Utilizator curent   : (nedefinit in mediu)\n");
    }

    /* TODO 2: Directorul personal ($HOME).
     * Extrageti variabila "HOME" folosind getenv("HOME").
     * Daca este definita, afisati: "[+] Director personal   : %s\n", home
     * Altfel afisati: "[+] Director personal   : (nedefinit in mediu)\n"
     */
    const char *home = NULL; /* TODO: apelati getenv() */
    if (home != NULL) {
        printf("[+] Director personal   : %s\n", home);
    } else {
        printf("[+] Director personal   : (nedefinit in mediu)\n");
    }

    /* TODO 3: Tipul terminalului curent ($TERM).
     * Extrageti variabila "TERM" cu getenv("TERM") si daca nu e NULL afisati:
     * printf("[+] Tip terminal ($TERM): %s\n", term);
     */
    const char *term = NULL; /* TODO: apelati getenv() */
    if (term != NULL) {
        printf("[+] Tip terminal ($TERM): %s\n", term);
    }

    /* TODO 4: Variabila $PATH si explicatia de securitate.
     * Extrageti variabila "PATH" folosind getenv("PATH").
     * Daca este definita:
     *   - afisati primii 70 de octeti: printf("[+] PATH (primii 70 B)  : %.70s...\n", path);
     *   - verificati daca directorul curent '.' sau '::' se afla in PATH:
     *     (strstr(path, ":.:") != NULL || strncmp(path, ".:", 2) == 0 || strstr(path, "::") != NULL)
     *   - daca DA, afisati avertismentul de securitate
     *   - daca NU, afisati nota explicativa despre securitate si necesitatea prefixului './'
     */
    const char *path = NULL; /* TODO: apelati getenv() */
    if (path != NULL) {
        printf("[+] PATH (primii 70 B)  : %.70s...\n", path);
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

    /* TODO 5: Implementati numararea cuvintelor din 'str'.
     * Parcurgeti sirul caracter cu caracter pana la '\0'.
     * Folositi o variabila de stare 'in_word' (0 sau 1) si functia isspace((unsigned char)str[i]):
     *   - daca intalniti un caracter spatiu (isspace), setati in_word = 0;
     *   - daca nu este spatiu si in_word == 0, setati in_word = 1 si incrementati words;
     */
    (void)str; /* Eliminati acest cast dupa implementare */

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
    /* TODO 6: Deschideti fisierul 'filename' in modul citire ("r") folosind fopen().
     * Daca apelul returneaza NULL (eroare la deschidere):
     *   - afisati mesajul de diagnostic folosind:
     *       perror("Eroare la deschiderea fisierului de intrare");
     *       fprintf(stderr, "Fisierul specificat nu a putut fi accesat: %s\n", filename);
     *   - returnati -1
     */
    FILE *file = NULL; /* TODO: apelati fopen(filename, "r") */

    /* Comentati / eliminati aceasta verificare temporara dupa ce apelati fopen */
    if (file == NULL) {
        /* Placeholder temporar pentru compilare */
        (void)filename;
    }

    printf("Analizam fisierul: %s\n", filename);
    printf("--------------------------------------------------------\n");

    char buffer[MAX_LINE_LEN];
    int line_count = 0;
    int word_count = 0;
    int total_chars = 0;

    /* TODO 7: Cititi linie cu linie folosind fgets(buffer, sizeof(buffer), file).
     * In bucla while:
     *   - incrementati line_count
     *   - adaugati lungimea liniei (strlen(buffer)) la total_chars
     *   - apelati count_words_in_string(buffer) si adaugati rezultatul la word_count
     *   - eliminati caracterul '\n' de la finalul liniei (daca buffer[len - 1] == '\n')
     *   - afisati linia curenta:
     *     printf("[%2d] (%2d cuv, %3zu B): %s\n", line_count, words_on_line, len, buffer);
     */
    (void)buffer; /* Eliminati acest cast dupa implementarea buclei while */
    (void)count_words_in_string; /* Eliminati dupa apelul in bucla */

    /* TODO 8: Inchideti fisierul apeland fclose(file).
     * Daca fclose(file) != 0, afisati: perror("Avertisment: Eroare la apelul fclose");
     */
    if (file != NULL) {
        fclose(file);
    }

    printf("--------------------------------------------------------\n");
    printf("STATISTICI FINALE:\n");
    printf("  [+] Linii citite     : %d\n", line_count);
    printf("  [+] Cuvinte gasite   : %d\n", word_count);
    printf("  [+] Octeti procesati : %d\n", total_chars);
    printf("========================================================\n");

    return (file == NULL) ? -1 : 0;
}

int main(int argc, char *argv[]) {
    /* Verificare validitate argumente (gata scrisa conform ghidului) */
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
