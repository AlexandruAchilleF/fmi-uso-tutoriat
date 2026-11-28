# Tutoriatul 6: Drepturi rwx, biți speciali și useri

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Idei, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)  
> **Merge pe:** Linux și macOS

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic](#2-breviar-teoretic)
   - [2.1. Citim o linie din `ls -l`](#21-citim-o-linie-din-ls--l)
   - [2.2. `chmod`: litere și cifre](#22-chmod-litere-și-cifre)
   - [2.3. `umask`: drepturile fișierelor noi](#23-umask-drepturile-fișierelor-noi)
   - [2.4. Biții speciali: SUID, SGID, sticky](#24-biții-speciali-suid-sgid-sticky)
   - [2.5. În C: `stat()` și `getpwuid()`](#25-în-c-stat-și-getpwuid)
3. [Tabel Sinoptic](#3-tabel-sinoptic)
4. [Exercițiul C: un „mini `ls -l`”](#4-exercițiul-c-un-mini-ls--l)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Capcane Uzuale de Examen & Erori Frecvente](#6-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe, fiecare student va fi capabil:
- Să citească drepturile dintr-o linie `ls -l` (proprietar, grup, ceilalți).
- Să schimbe drepturi cu `chmod`, atât cu litere (`u+x`) cât și cu cifre (`644`).
- Să calculeze drepturile unui fișier nou din `umask`.
- Să recunoască biții speciali `s` și `t` (SUID, SGID, sticky) și să spună la ce folosesc.
- Să citească metadatele unui fișier în C cu `stat()` și numele proprietarului cu `getpwuid()`.

---

## 2. Breviar Teoretic

### 2.1. Citim o linie din `ls -l`

```text
-    rwx  r-x  ---   1      student     studenti  36      Nov 28 11:16  script.sh
tip  u    g    o     nume   proprietar  grup      mărime  data          numele
```

- **u** (user) = proprietarul, **g** (group) = grupul fișierului, **o** (others) = toți ceilalți.
- Sistemul alege **o singură** grupă: ești proprietarul? atunci contează doar `u`. Altfel, ești în grup? atunci `g`. Altfel `o`.
- Proprietarul și grupul sunt ținute în i-nod ca **numere** (uid, gid). Numele vin din `/etc/passwd` și `/etc/group`. Vedeți ce sunteți cu `id`.

| | la un fișier | la un director |
| :--- | :--- | :--- |
| `r` | poți citi conținutul | poți lista numele (`ls`) |
| `w` | poți modifica conținutul | poți crea și șterge nume în el |
| `x` | poți rula fișierul | poți intra în el (`cd`) |

> [!NOTE]
> `root` (uid 0) trece de verificările pentru `r` și `w` oricum.

---

### 2.2. `chmod`: litere și cifre

Fiecare grupă e o cifră în octal: **r = 4, w = 2, x = 1**, adunate.

| Drepturi | Cifre | Folosit pentru |
| :--- | :--- | :--- |
| `rw-r--r--` | `644` | fișiere text |
| `rwxr-xr-x` | `755` | scripturi, programe, directoare |
| `rw-------` | `600` | fișiere private |
| `rwxr-x---` | `750` | doar proprietarul și echipa |

```bash
chmod 600 nota.txt          # cu cifre: setezi tot odată
chmod u+x,go+r nota.txt     # cu litere: adaugi (+) sau scoți (-) doar ce spui
chmod u-w nota.txt          # nici proprietarul nu mai poate scrie
```

```bash
$ chmod u-w nota.txt
$ echo "nota: 4" >> nota.txt
-bash: nota.txt: Permission denied
```

---

### 2.3. `umask`: drepturile fișierelor noi

Un fișier nou pornește de la `666` (`rw-rw-rw-`), un director nou de la `777`. **Biții din `umask` se scot.**

| umask | fișier nou | director nou |
| :--- | :--- | :--- |
| `022` (macOS, multe servere) | `644` `rw-r--r--` | `755` `rwxr-xr-x` |
| `002` (Ubuntu, user obișnuit) | `664` `rw-rw-r--` | `775` `rwxrwxr-x` |
| `077` | `600` `rw-------` | `700` `rwx------` |

```bash
$ umask
0002
$ umask 077 && touch secret.txt && ls -l secret.txt
-rw------- 1 student student 0 Nov 28 11:17 secret.txt
```

> [!TIP]
> Fișierele pornesc de la 666, nu 777, ca să nu fie executabile din greșeală. `x` îl puneți voi, cu `chmod +x`.

---

### 2.4. Biții speciali: SUID, SGID, sticky

O a patra cifră, în fața celorlalte trei: `chmod 4755`, `chmod 1777`.

| Bit | Cifra | Unde apare | Ce face | Exemplu |
| :--- | :--- | :--- | :--- | :--- |
| **SUID** | `4` | `s` în locul lui `x` de la **u** | programul rulează cu drepturile **proprietarului lui** | `-rwsr-xr-x root /usr/bin/passwd` |
| **SGID** | `2` | `s` în locul lui `x` de la **g** | pe un director: fișierele noi primesc grupul directorului | `drwxrwsr-x proiect/` |
| **Sticky** | `1` | `t` în locul lui `x` de la **o** | pe un director: fiecare șterge doar fișierele lui | `drwxrwxrwt root /tmp` |

```bash
$ ls -l /tmp/al_anei.txt
-rw-rw-rw- 1 ana ana 14 Nov 28 11:17 /tmp/al_anei.txt
$ echo "am scris si eu" >> /tmp/al_anei.txt      # merge: o are rw-
$ rm /tmp/al_anei.txt
rm: cannot remove '/tmp/al_anei.txt': Operation not permitted   # sticky bit
```

> [!WARNING]
> Literă **mare** (`S`, `T`) = bitul special e pus, dar `x`-ul de sub el lipsește. `chmod 4644 prog` dă `-rwSr--r--`: de obicei o greșeală.

---

### 2.5. În C: `stat()` și `getpwuid()`

```c
#include <sys/stat.h>
#include <pwd.h>

struct stat st;
if (stat("demo/a.txt", &st) == -1) { perror("demo/a.txt"); return 1; }

mode_t m = st.st_mode;                    /* tipul + cei 12 biți de drepturi */
if (S_ISDIR(m)) { /* e director */ }
if (m & S_IRUSR) { /* proprietarul poate citi */ }

struct passwd *pw = getpwuid(st.st_uid);  /* uid -> linia din /etc/passwd */
if (pw != NULL) printf("%s\n", pw->pw_name);
```

| Câmp din `struct stat` | Ce ține |
| :--- | :--- |
| `st_mode` | tipul fișierului și drepturile (de ex. `0100644`) |
| `st_uid`, `st_gid` | proprietarul și grupul, ca numere |
| `st_size` | mărimea în octeți |
| `st_nlink` | câte nume are (Tutoriatul 05) |
| `st_ino` | numărul i-nodului (Tutoriatul 05) |

| Mască | Octal | | Mască | Octal | | Mască | Octal |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `S_IRUSR` | `0400` | | `S_IRGRP` | `0040` | | `S_IROTH` | `0004` |
| `S_IWUSR` | `0200` | | `S_IWGRP` | `0020` | | `S_IWOTH` | `0002` |
| `S_IXUSR` | `0100` | | `S_IXGRP` | `0010` | | `S_IXOTH` | `0001` |
| `S_ISUID` | `4000` | | `S_ISGID` | `2000` | | `S_ISVTX` | `1000` |

> [!IMPORTANT]
> `stat` nu are nevoie de drept de citire pe fișier: `./exercitiu /etc/shadow` merge, deși `cat /etc/shadow` nu. Ajunge să poți intra (`x`) în directoarele de pe drum.

---

## 3. Tabel Sinoptic

| Sintaxă | Pe scurt | Manual |
| :--- | :--- | :--- |
| `ls -l`, `ls -ld dir` | drepturi, proprietar, grup, mărime | `man 1 ls` |
| `id` | uid-ul, gid-ul și grupurile tale | `man 1 id` |
| `chmod 644 f`, `chmod u+x f` | schimbă drepturile | `man 1 chmod` |
| `umask`, `umask 077` | ce drepturi NU primesc fișierele noi | `help umask` |
| `chmod 4755`, `2775`, `1777` | SUID, SGID, sticky | `man 1 chmod` |
| `stat(cale, &st)` | citește i-nodul; `-1` la eroare | `man 2 stat` |
| `S_ISDIR(m)`, `m & S_IRUSR` | tipul, câte un bit de drept | `man 7 inode` |
| `getpwuid(uid)` | `struct passwd *`, `NULL` dacă nu există | `man 3 getpwuid` |
| `chmod(cale, 0644)`, `umask(077)` | aceleași lucruri, din C | `man 2 chmod`, `man 2 umask` |

---

## 4. Exercițiul C: un „mini `ls -l`”

Programul `exercitiu.c` primește unul sau mai multe fișiere și afișează câte o linie pentru fiecare:

```text
Utilizare: ./exercitiu fisier...
<drepturi> <octal> <proprietar> <mărime> <nume>
```

Ce trebuie să facă:

1. **Verifică argumentele:** măcar un fișier, altfel mesaj de utilizare și cod `1`.
2. **Citește i-nodul** cu `stat`. La `-1`: `perror(argv[i])`, ține minte codul `1` și treci la următorul fișier (`continue`).
3. **Construiește șirul de drepturi** (`drwxr-xr-x`) din `st_mode`, cu `S_ISDIR` și cele 9 măști. *Bonus:* `s`, `s`, `t` pentru biții speciali.
4. **Află numele proprietarului** cu `getpwuid`. Dacă întoarce `NULL`, afișezi uid-ul ca număr.

Scheletul compilează și rulează din prima, dar afișează `----------` și `?`. Linia cu `printf` e deja scrisă.

```bash
$ make demo                # face folderul demo/ cu drepturi diferite
$ ./exercitiu demo/*
-rw-r--r-- 0644 student       6 demo/a.txt
-rwsr-xr-x 4755 student      36 demo/prog
drwxrwxrwt 1777 student    4096 demo/public
-rwxr-xr-x 0755 student      36 demo/script.sh
-rw------- 0600 student      13 demo/secret.txt
drwxr-xr-x 0755 student    4096 demo/sub

$ ./exercitiu demo/a.txt lipsa.txt; echo "cod: $?"
-rw-r--r-- 0644 student       6 demo/a.txt
lipsa.txt: No such file or directory
cod: 1
```

---

## 5. Structura Directorului & Ghid Practic de Rulare

```text
tutoriat-06/
├── README.md                      # Prezentul breviar și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── img/                           # Capturile de terminal folosite în slide-uri
├── schelet/                       # Codul de lucru pentru studenți
│   ├── Makefile                   # Compilare, demo/, teste
│   └── exercitiu.c                # Mini ls -l, cu instrucțiuni /* TODO */
├── solutii/                       # Soluțiile complete
│   ├── Makefile
│   └── exercitiu.c
└── kahoot/                        # Quiz-ul săptămânal (păstrat local, nu e public)
```

Folderul `demo/` nu e în git (git păstrează doar bitul `x`, nu și restul drepturilor): îl face `make demo`, iar `make test` și `make run` îl refac singure.

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-06/schelet
   ```
2. **Deschideți `exercitiu.c`** și completați `/* TODO 1..4 */` (și bonusul din TODO 3).
3. **Compilați și testați:**
   ```bash
   make            # compilează cu -Wall -Wextra -std=c11 -pedantic -g
   make test       # 5 teste: fără argumente, demo/*, comparație cu ls -ld, fișier lipsă, /tmp
   ```
4. **Încercați de mână:** `chmod 640 demo/a.txt; ./exercitiu demo/a.txt`.
5. **Curățați fișierele generate:** `make clean` (șterge și `demo/`).

Pe macOS: o singură dată `xcode-select --install`, apoi aceleași comenzi. Grupul va fi `staff`, umask-ul `022`, iar `/tmp` e un symlink spre `/private/tmp` (de aceea testul folosește `/tmp/`, cu slash).

---

## 6. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `if (m & S_IRUSR == 1)`
- **Cauză:** `==` se face înaintea lui `&`, iar `S_IRUSR` e `0400`, nu `1`.
- **Remediere:** `if (m & S_IRUSR)`: orice rezultat diferit de 0 e adevărat.

### Capcana 2: director fără `x`
- **Cauză:** `chmod 644 dir` scoate dreptul de a intra în director: `cd: Permission denied`.
- **Remediere:** directoarele au de obicei `755` (sau `700`).

### Capcana 3: `printf("%s", st.st_uid)`
- **Cauză:** `st_uid` e un număr, nu un nume.
- **Remediere:** `getpwuid(st.st_uid)`, verificând `NULL` înainte de `pw->pw_name`.

### Capcana 4: `S` sau `T` mare
- **Cauză:** bitul special e pus, dar `x`-ul de sub el lipsește (`chmod 4644`).
- **Remediere:** `chmod 4755`.

### Capcana 5: „am dat `chmod 777` și acum merge”
- **Cauză:** merge, dar oricine poate modifica fișierul. Ascunde problema reală.
- **Remediere:** dați doar ce trebuie: `644` la fișiere, `755` la scripturi și directoare.

### Capcana 6: `%d` pentru `st_size`
- **Cauză:** `st_size` e `off_t`, care poate fi `long long`.
- **Remediere:** `printf("%lld", (long long)st.st_size)`.
