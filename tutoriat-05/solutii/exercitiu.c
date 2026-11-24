/**
 * USO - Tutoriat 05: I-noduri, link-uri si directoare in C
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutia completa pentru un "mini ls -i": afiseaza continutul unui
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
    /* 1. Cel mult un argument: directorul. Fara argument, folosim "." */
    if (argc > 2) {
        fprintf(stderr, "Utilizare: %s [director]\n", argv[0]);
        return 1;
    }
    const char *dir = (argc == 2) ? argv[1] : ".";

    /* 2. Deschidem directorul. opendir intoarce NULL la eroare. */
    DIR *d = opendir(dir);
    if (d == NULL) {
        perror(dir);
        return 1;
    }

    /* 3. readdir da cate o intrare la fiecare apel si NULL la final. */
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (e->d_name[0] == '.') {
            continue;   /* sarim peste ".", ".." si fisierele ascunse */
        }
        printf("%lu %s\n", (unsigned long)e->d_ino, e->d_name);
    }

    /* 4. Fiecare opendir reusit are un closedir. */
    closedir(d);
    return 0;
}
