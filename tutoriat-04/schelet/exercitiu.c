/**
 * USO - Tutoriat 04: Apeluri sistem: open, read, write, close
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru un "mini cp": copiaza un fisier in altul
 *       folosind direct apelurile sistem open(), read(), write(), close().
 *
 *       - erorile se afiseaza cu perror(), care traduce errno in text
 *         (de exemplu: "No such file or directory")
 *       - fiecare open() reusit are nevoie de close()
 *
 * Exemple de rulare:
 *       ./exercitiu mesaj.txt copie.txt   -> Am copiat 204 octeti ..., cod 0
 *       ./exercitiu lipsa.txt copie.txt   -> eroare cu perror pe stderr, cod 1
 *       ./exercitiu                       -> mesaj de utilizare, cod 1
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>      /* printf, fprintf, perror */
#include <fcntl.h>      /* open si O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC */
#include <unistd.h>     /* read, write, close */

#define DIM_BUFFER 64   /* citim cate 64 de octeti o data */

int main(int argc, char *argv[]) {
    /* TODO 1: Daca nu am primit exact doua argumente (argc != 3), afiseaza pe stderr:
     *             Utilizare: ./exercitiu <sursa> <destinatie>
     *         si intoarce 1. (argc numara si numele programului.) */
    (void)argc; /* sterge linia asta dupa ce rezolvi TODO 1 */

    /* TODO 2: Deschide sursa doar pentru citire:
     *             int fd_sursa = open(argv[1], O_RDONLY);
     *         Daca open intoarce -1: perror(argv[1]); return 1;
     *         perror citeste errno si afiseaza, de exemplu:
     *             lipsa.txt: No such file or directory */
    int fd_sursa = -1;

    /* TODO 3: Deschide destinatia pentru scriere, cu creare si golire:
     *             open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)
     *         Daca intoarce -1: perror(argv[2]), inchide fd_sursa si return 1. */
    int fd_dest = -1;

    char buf[DIM_BUFFER];
    long total = 0;
    int cod = 0;
    ssize_t n = 0;

    /* TODO 4: Copiaza cu o bucla:
     *             while ((n = read(fd_sursa, buf, DIM_BUFFER)) > 0) { ... }
     *         In bucla: write(fd_dest, buf, n) si total += n.
     *         Atentie: scrie n octeti (cat ai citit), nu DIM_BUFFER!
     *         Daca write nu scrie n octeti: perror("write"), cod = 1, break.
     *         Dupa bucla, daca n == -1: perror("read"), cod = 1. */
    (void)buf; (void)n; /* sterge linia asta dupa ce rezolvi TODO 4 */

    /* TODO 5: Inchide ambii descriptori cu close(). Fiecare open() reusit are nevoie de close(). */
    (void)fd_sursa; (void)fd_dest; /* sterge linia asta dupa ce rezolvi TODO 2, 3 si 5 */

    if (cod == 0) {
        printf("Am copiat %ld octeti din %s in %s\n", total, argv[1], argv[2]);
    }
    return cod;
}
