/**
 * USO - Tutoriat 06: Metadate, drepturi rwx, biti speciali si useri
 * FMI, Universitatea din Bucuresti
 *
 * Fisier: schelet/exercitiu.c
 * Scop: Schelet de laborator pentru un "mini ls -l": pentru fiecare fisier primit
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
 * Scheletul compileaza si ruleaza din prima, dar afiseaza "----------" si "?".
 * Rezolva TODO-urile in ordine si ruleaza dupa fiecare: make run
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
    /* TODO 1: Daca nu am primit niciun fisier (argc < 2), afiseaza pe stderr:
     *             Utilizare: ./exercitiu fisier...
     *         si intoarce 1. */

    int cod = 0;   /* devine 1 daca macar un fisier da eroare */

    for (int i = 1; i < argc; i++) {
        struct stat st = {0};

        /* TODO 2: Citeste i-nodul fisierului:
         *             if (stat(argv[i], &st) == -1) { ... }
         *         La eroare: perror(argv[i]); cod = 1; continue;
         *         (continue = trecem la urmatorul fisier, nu ne oprim) */

        /* TODO 3: Construieste sirul de drepturi, ca la ls -l.
         *         Pornim de la "----------" si punem litere unde bitul e setat:
         *             mode_t m = st.st_mode;
         *             if (S_ISDIR(m)) drepturi[0] = 'd';
         *             if (m & S_IRUSR) drepturi[1] = 'r';
         *             if (m & S_IWUSR) drepturi[2] = 'w';
         *             if (m & S_IXUSR) drepturi[3] = 'x';
         *         la fel pentru grup (S_IRGRP, S_IWGRP, S_IXGRP -> pozitiile 4..6)
         *         si pentru ceilalti (S_IROTH, S_IWOTH, S_IXOTH -> pozitiile 7..9).
         *
         *         BONUS: bitii speciali iau locul lui x:
         *             S_ISUID -> 's' pe pozitia 3, S_ISGID -> 's' pe 6, S_ISVTX -> 't' pe 9
         *         (litera mare 'S' / 'T' daca x-ul de dedesubt lipseste) */
        char drepturi[11] = "----------";

        /* TODO 4: Afla numele proprietarului:
         *             struct passwd *pw = getpwuid(st.st_uid);
         *         Daca pw != NULL, proprietar = pw->pw_name;
         *         altfel scrie numarul in uid_text si foloseste-l pe el:
         *             snprintf(uid_text, sizeof uid_text, "%u", (unsigned)st.st_uid);
         *             proprietar = uid_text; */
        char uid_text[32];
        const char *proprietar = "?";
        (void)uid_text; /* sterge linia asta dupa ce rezolvi TODO 4 */

        printf("%s %04o %-8s %6lld %s\n",
               drepturi,
               (unsigned)(st.st_mode & 07777),   /* doar cei 12 biti de drepturi */
               proprietar,
               (long long)st.st_size,
               argv[i]);
    }

    return cod;
}
