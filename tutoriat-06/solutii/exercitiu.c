/**
 * USO - Tutoriat 06: Metadate, drepturi rwx, biti speciali si useri
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: solutii/exercitiu.c
 * Scop: Solutia completa pentru un "mini ls -l": pentru fiecare fisier primit
 *       in linia de comanda afiseaza o linie de forma
 *             <drepturi> <octal> <proprietar> <marime> <nume>
 *       de exemplu:
 *             -rw-r--r-- 0644 student       6 demo/a.txt
 *       folosind stat() si getpwuid().
 *
 *       - stat() citeste i-nodul: tip, drepturi, proprietar (uid), marime
 *       - st_mode tine tipul si cei 12 biti de drepturi (rwx x 3 + SUID, SGID, sticky)
 *       - getpwuid() transforma numarul uid in nume de utilizator (din /etc/passwd)
 *
 * Exemple de rulare:
 *       ./exercitiu demo/a.txt       -> -rw-r--r-- 0644 student 6 demo/a.txt, cod 0
 *       ./exercitiu demo/a.txt demo/sub -> cate o linie pentru fiecare, cod 0
 *       ./exercitiu a.txt lipsa      -> linia pentru a.txt + "lipsa: No such file or directory", cod 1
 *       ./exercitiu                  -> mesaj de utilizare, cod 1
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>      /* printf, fprintf, perror, snprintf */
#include <sys/stat.h>   /* struct stat, stat, S_ISDIR, S_IRUSR, ... */
#include <pwd.h>        /* struct passwd, getpwuid */

int main(int argc, char *argv[]) {
    /* 1. Avem nevoie de cel putin un fisier. */
    if (argc < 2) {
        fprintf(stderr, "Utilizare: %s fisier...\n", argv[0]);
        return 1;
    }

    int cod = 0;   /* devine 1 daca macar un fisier da eroare */

    for (int i = 1; i < argc; i++) {
        struct stat st;

        /* 2. stat citeste i-nodul fisierului. -1 = eroare. */
        if (stat(argv[i], &st) == -1) {
            perror(argv[i]);
            cod = 1;
            continue;   /* trecem la urmatorul fisier */
        }

        /* 3. Construim sirul de drepturi, ca la ls -l. */
        char drepturi[11] = "----------";
        mode_t m = st.st_mode;

        if (S_ISDIR(m)) drepturi[0] = 'd';

        if (m & S_IRUSR) drepturi[1] = 'r';   /* proprietarul (u) */
        if (m & S_IWUSR) drepturi[2] = 'w';
        if (m & S_IXUSR) drepturi[3] = 'x';
        if (m & S_IRGRP) drepturi[4] = 'r';   /* grupul (g) */
        if (m & S_IWGRP) drepturi[5] = 'w';
        if (m & S_IXGRP) drepturi[6] = 'x';
        if (m & S_IROTH) drepturi[7] = 'r';   /* ceilalti (o) */
        if (m & S_IWOTH) drepturi[8] = 'w';
        if (m & S_IXOTH) drepturi[9] = 'x';

        /* 3b (bonus). Bitii speciali iau locul lui x: s, s, t. */
        if (m & S_ISUID) drepturi[3] = (m & S_IXUSR) ? 's' : 'S';
        if (m & S_ISGID) drepturi[6] = (m & S_IXGRP) ? 's' : 'S';
        if (m & S_ISVTX) drepturi[9] = (m & S_IXOTH) ? 't' : 'T';

        /* 4. Numele proprietarului. getpwuid intoarce NULL daca uid-ul
         *    nu apare in /etc/passwd: atunci afisam numarul. */
        char uid_text[32];
        const char *proprietar;
        struct passwd *pw = getpwuid(st.st_uid);
        if (pw != NULL) {
            proprietar = pw->pw_name;
        } else {
            snprintf(uid_text, sizeof uid_text, "%u", (unsigned)st.st_uid);
            proprietar = uid_text;
        }

        printf("%s %04o %-8s %6lld %s\n",
               drepturi,
               (unsigned)(m & 07777),      /* doar cei 12 biti de drepturi */
               proprietar,
               (long long)st.st_size,
               argv[i]);
    }

    return cod;
}
