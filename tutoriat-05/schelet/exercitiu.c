/**
 * USO - Tutoriat 05: I-noduri, link-uri si directoare in C
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru un "mini ls -i": afiseaza continutul unui
 *       director, cate o intrare pe linie, sub forma
 *             <numarul i-nodului> <nume>
 *       folosind opendir(), readdir() si closedir().
 *
 *       - un director e doar o lista de perechi (nume, numar de i-nod)
 *       - doua nume cu acelasi i-nod sunt acelasi fisier (hard link)
 *       - fisierele ascunse (care incep cu '.') nu se afiseaza, la fel ca la ls
 *
 * Exemple de rulare:
 *       ./exercitiu demo       -> o linie pentru fiecare fisier din demo/, cod 0
 *       ./exercitiu            -> la fel, pentru directorul curent (".")
 *       ./exercitiu lipsa      -> lipsa: No such file or directory, cod 1
 *       ./exercitiu a b        -> mesaj de utilizare, cod 1
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>      /* printf, fprintf, perror */
#include <dirent.h>     /* DIR, struct dirent, opendir, readdir, closedir */

int main(int argc, char *argv[]) {
    /* TODO 1: Daca am primit mai mult de un argument (argc > 2), afiseaza pe stderr:
     *             Utilizare: ./exercitiu [director]
     *         si intoarce 1. Fara argument (argc == 1) lucram in directorul curent ".". */
    const char *dir = (argc == 2) ? argv[1] : ".";

    /* TODO 2: Deschide directorul:
     *             DIR *d = opendir(dir);
     *         Daca opendir intoarce NULL: perror(dir); return 1;
     *         (opendir intoarce NULL la eroare, nu -1 ca open!) */
    (void)dir; /* sterge linia asta dupa ce rezolvi TODO 2 */

    /* TODO 3: Citeste intrarile una cate una:
     *             struct dirent *e;
     *             while ((e = readdir(d)) != NULL) { ... }
     *         In bucla:
     *           - daca e->d_name[0] == '.', sari peste ea cu continue
     *             (asa scapam de ".", ".." si de fisierele ascunse)
     *           - altfel afiseaza:  printf("%lu %s\n", (unsigned long)e->d_ino, e->d_name);
     *         d_ino = numarul i-nodului, d_name = numele din director. */

    /* TODO 4: Inchide directorul cu closedir(d). */

    return 0;
}
