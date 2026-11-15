/**
 * USO - Tutoriat 04: Apeluri sistem: open, read, write, close
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutia completa pentru un "mini cp": copiaza un fisier in altul
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
    /* 1. Avem exact doua argumente: sursa si destinatia? */
    if (argc != 3) {
        fprintf(stderr, "Utilizare: %s <sursa> <destinatie>\n", argv[0]);
        return 1;
    }

    /* 2. Deschidem sursa doar pentru citire. open() intoarce un numar mic (descriptor),
     *    de obicei 3, pentru ca 0, 1 si 2 sunt deja ocupate de stdin, stdout, stderr.
     *    La eroare intoarce -1 si pune codul erorii in errno. */
    int fd_sursa = open(argv[1], O_RDONLY);
    if (fd_sursa == -1) {
        perror(argv[1]);                  /* ex: "lipsa.txt: No such file or directory" */
        return 1;
    }

    /* 3. Deschidem destinatia pentru scriere: o cream daca nu exista (O_CREAT),
     *    o golim daca exista (O_TRUNC). 0644 = rw-r--r-- */
    int fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1) {
        perror(argv[2]);
        close(fd_sursa);                  /* nu uitam ce am deschis deja */
        return 1;
    }

    /* 4. Citim bucati si le scriem. read() intoarce cati octeti a citit:
     *        > 0  -> am citit ceva, il scriem
     *        0    -> am ajuns la sfarsitul fisierului (EOF), ne oprim
     *        -1   -> eroare */
    char buf[DIM_BUFFER];
    long total = 0;
    int cod = 0;
    ssize_t n;
    while ((n = read(fd_sursa, buf, DIM_BUFFER)) > 0) {
        if (write(fd_dest, buf, n) != n) {   /* scriem exact cat am citit, nu DIM_BUFFER */
            perror("write");
            cod = 1;
            break;
        }
        total += n;
    }
    if (n == -1) {
        perror("read");
        cod = 1;
    }

    /* 5. Fiecare open() reusit are nevoie de close(). */
    close(fd_sursa);
    close(fd_dest);

    if (cod == 0) {
        printf("Am copiat %ld octeti din %s in %s\n", total, argv[1], argv[2]);
    }
    return cod;
}
