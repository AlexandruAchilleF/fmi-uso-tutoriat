/**
 * USO - Tutoriat 01: Concepte Fundamentale de SO & Linia de Comanda Linux
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Acomodarea cu argumentele din linia de comanda (argc, argv),
 *       variabilele de mediu (getenv), citirea de date si codurile de retur.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024

/**
 * Afiseaza instructiunile de utilizare ale programului.
 * Observati utilizarea fluxului stderr pentru mesaje de diagnostic si utilizare.
 */
void print_usage(const char *prog_name) {
    fprintf(stderr, "Utilizare: %s <cale_fisier_intrare>\n", prog_name);
    fprintf(stderr, "Exemplu:   %s date.in\n", prog_name);
}

/**
 * Functie pentru inspectarea variabilelor de mediu ale procesului curent.
 *
 * In Linux, variabilele de mediu sunt mostenite de la shell-ul parinte.
 * Functia getenv() cauta in tabela de mediu a procesului cheia specificata.
 */
void print_environment_info(void) {
    printf("=== Informatii despre Mediul de Executie ===\n");

    /* TODO 1: Extrageti si afisati numele utilizatorului curent din variabila "USER".
     * Daca "USER" este NULL, incercati variabila "LOGNAME".
     * Daca ambele sunt NULL, afisati "Necunoscut".
     */
    const char *user = NULL; /* TODO: Inlocuiti cu apelul getenv() */
    if (user != NULL) {
        printf("[+] Utilizator curent : %s\n", user);
    } else {
        printf("[+] Utilizator curent : Necunoscut\n");
    }

    /* TODO 2: Extrageti si afisati calea directorului personal din variabila "HOME". */
    const char *home = NULL; /* TODO: Inlocuiti cu apelul getenv() */
    if (home != NULL) {
        printf("[+] Director personal : %s\n", home);
    }

    /* TODO 3: Extrageti variabila "PATH" si afisati primii 60 de octeti ai acesteia.
     * Explicati de ce '.' (directorul curent) nu ar trebui sa apara in PATH!
     */
    const char *path = NULL; /* TODO: Inlocuiti cu apelul getenv() */
    if (path != NULL) {
        printf("[+] PATH (fragment)   : %.60s...\n", path);
    }

    printf("============================================\n\n");
}

/**
 * Functie care analizeaza un fisier text primit ca parametru:
 * - deschide fisierul in mod citire ("r")
 * - citeste fisierul linie cu linie folosind fgets()
 * - contorizeaza numarul de linii, numarul de cuvinte si numarul total de caractere
 * - afiseaza fiecare linie numerotata
 * - la final inchide fisierul si afiseaza statisticile
 *
 * Returneaza:
 *   0 (succes) daca fisierul a fost procesat cu succes
 *  -1 (eroare) daca fisierul nu a putut fi deschis
 */
int analyze_file(const char *filename) {
    /* TODO 4: Deschideti fisierul 'filename' in modul "r" (read) folosind fopen().
     * Daca pointerul returnat este NULL, afisati un mesaj de eroare pe stderr
     * si returnati -1.
     */
    FILE *file = NULL; /* TODO: apelati fopen(filename, "r") */

    /* Verificare TODO 4:
    if (file == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut deschide fisierul '%s' pentru citire!\n", filename);
        return -1;
    }
    */

    printf("=== Analiza Fisierului: %s ===\n", filename);

    char buffer[MAX_LINE_LEN];
    int line_count = 0;
    int word_count = 0;
    int char_count = 0;

    /* TODO 5: Cititi linie cu linie folosind fgets(buffer, sizeof(buffer), file).
     * Pentru fiecare linie citita:
     *   - incrementati line_count
     *   - adaugati lungimea liniei (strlen) la char_count
     *   - numarati cuvintele de pe linie (separatori: spatii, tab-uri, newline)
     *   - afisati linia sub forma: [Linia XX] text
     */

    /* TODO 6: Inchideti fisierul cu fclose(file). */

    printf("--- Statistici Finale ---\n");
    printf("[+] Numar total de linii     : %d\n", line_count);
    printf("[+] Numar total de cuvinte   : %d\n", word_count);
    printf("[+] Numar total de caractere : %d\n", char_count);
    printf("=========================\n");

    return 0;
}

int main(int argc, char *argv[]) {
    /* TODO 7: Verificati daca programul a primit cel putin un argument in linia de comanda.
     * Observatie: argv[0] contine intotdeauna numele binarului apelat (ex: ./exercitiu).
     * Daca argc < 2, apelati print_usage(argv[0]) si returnati EXIT_FAILURE (cod 1).
     */

    /* TODO 8: Daca argumentele sunt valide:
     * 1. Apelati print_environment_info();
     * 2. Apelati analyze_file(argv[1]);
     * 3. Daca analyze_file a returnat eroare (-1), returnati EXIT_FAILURE.
     * 4. Altfel, afisati un mesaj de finalizare si returnati EXIT_SUCCESS (cod 0).
     */

    /* Placeholder temporar pentru compilare initiala */
    print_usage(argv[0]);
    return EXIT_FAILURE;
}
