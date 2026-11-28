# Tutoriatul 4: Apeluri sistem în C (`open`, `read`, `write`, `close`) și Valgrind

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Idei, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)  
> **Merge pe:** Linux și macOS (exercițiile nu au nevoie de Valgrind)

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic](#2-breviar-teoretic)
   - [2.1. Funcție de bibliotecă vs apel sistem](#21-funcție-de-bibliotecă-vs-apel-sistem)
   - [2.2. Descriptori de fișier](#22-descriptori-de-fișier)
   - [2.3. `open`, `read`, `write`, `close`](#23-open-read-write-close)
   - [2.4. Erori: `errno` și `perror`](#24-erori-errno-și-perror)
   - [2.5. Valgrind (doar de citit, pe Linux)](#25-valgrind-doar-de-citit-pe-linux)
3. [Tabel Sinoptic](#3-tabel-sinoptic)
4. [Exercițiul C: un „mini cp”](#4-exercițiul-c-un-mini-cp)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Capcane Uzuale de Examen & Erori Frecvente](#6-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe, fiecare student va fi capabil:
- Să explice diferența dintre o **funcție de bibliotecă** (`printf`, `fopen`) și un **apel sistem** (`write`, `open`).
- Să spună ce este un **descriptor de fișier** și de ce primul fișier deschis primește de obicei numărul `3`.
- Să copieze un fișier folosind doar `open`, `read`, `write` și `close`.
- Să trateze erorile cu `perror` și să înțeleagă mesaje ca `No such file or directory` sau `Permission denied`.
- Să citească un raport Valgrind simplu: memorie alocată și neeliberată (`definitely lost`) și fișiere lăsate deschise.

---

## 2. Breviar Teoretic

### 2.1. Funcție de bibliotecă vs apel sistem

Programul vostru nu poate scrie singur pe disc sau pe ecran: trebuie să-i ceară **nucleului** (kernel). Cererea aceasta se numește **apel sistem**.

```text
programul vostru  ──►  printf("Salut\n")       funcție de bibliotecă (man 3 printf)
                           │
                           ▼
                       write(1, "Salut\n", 6)   apel sistem (man 2 write)
                           │
                           ▼
                        kernel  ──►  ecranul
```

| | Funcție de bibliotecă | Apel sistem |
| :--- | :--- | :--- |
| Exemple | `printf`, `fopen`, `fgets`, `fclose` | `write`, `open`, `read`, `close` |
| Pagina de manual | secțiunea 3: `man 3 printf` | secțiunea 2: `man 2 write` |
| Fișierul e ținut minte ca | `FILE *f` (o structură) | `int fd` (un număr) |
| Cine o face | biblioteca C, în programul vostru | nucleul |

> [!NOTE]
> Funcțiile de bibliotecă sunt construite **peste** apelurile sistem. `printf` pregătește textul și, la final, cheamă `write`. Pe Linux puteți vedea asta cu `strace` (o captură reală e în slide-uri).

---

### 2.2. Descriptori de fișier

Un **descriptor de fișier** (`fd`) este un **număr mic** prin care programul se referă la un fișier deschis. Orice program pornește cu trei deja deschise:

| fd | Nume | Unde duce de obicei |
| :--- | :--- | :--- |
| `0` | stdin | tastatura |
| `1` | stdout | ecranul |
| `2` | stderr | ecranul (pentru erori) |

`open` dă mereu **cel mai mic număr liber**. Cum 0, 1 și 2 sunt ocupate, primul fișier deschis primește `3`, următorul `4`. Dacă închideți `3` și deschideți altul, primește din nou `3`.

> [!TIP]
> Sunt aceleași numere ca în Tutoriatul 02: `2> erori.txt` înseamnă „ce scrie programul pe descriptorul 2 merge în fișier”.

---

### 2.3. `open`, `read`, `write`, `close`

```c
#include <fcntl.h>    /* open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC */
#include <unistd.h>   /* read, write, close */

int fd = open("mesaj.txt", O_RDONLY);          /* -1 la eroare */
char buf[64];
ssize_t n = read(fd, buf, sizeof(buf));        /* câți octeți a citit; 0 = sfârșit */
write(1, buf, n);                              /* scrie n octeți pe stdout */
close(fd);
```

| Apel | Ce face | Întoarce |
| :--- | :--- | :--- |
| `open(cale, O_RDONLY)` | deschide pentru citire | descriptorul (`3`, `4`…) sau `-1` |
| `open(cale, O_WRONLY \| O_CREAT \| O_TRUNC, 0644)` | deschide pentru scriere; creează fișierul dacă lipsește, îl golește dacă există | descriptorul sau `-1` |
| `read(fd, buf, cât)` | citește **cel mult** `cât` octeți în `buf` | câți a citit; `0` la sfârșitul fișierului; `-1` la eroare |
| `write(fd, buf, n)` | scrie `n` octeți din `buf` | câți a scris; `-1` la eroare |
| `close(fd)` | închide fișierul, descriptorul devine liber | `0` sau `-1` |

Copierea unui fișier e o buclă simplă:

```c
while ((n = read(fd_sursa, buf, 64)) > 0) {
    write(fd_dest, buf, n);       /* n, nu 64: ultima bucată e de obicei mai mică */
}
```

> [!IMPORTANT]
> `read` și `write` lucrează cu **octeți**, nu cu șiruri de caractere. Nu pun `'\0'` la final și nu se opresc la `'\n'`.

---

### 2.4. Erori: `errno` și `perror`

Când un apel sistem eșuează, întoarce `-1` și pune în variabila globală **`errno`** un cod care spune **de ce**. `perror` transformă codul în text:

```c
int fd = open(argv[1], O_RDONLY);
if (fd == -1) {
    perror(argv[1]);        /* afișează pe stderr: "lipsa.txt: No such file or directory" */
    return 1;
}
```

| Situație | `errno` | Mesajul de la `perror` |
| :--- | :--- | :--- |
| fișierul nu există | `ENOENT` | `No such file or directory` |
| n-aveți drept de scriere / citire | `EACCES` | `Permission denied` |
| ați dat un director în loc de fișier | `EISDIR` | `Is a directory` |

> [!WARNING]
> Verificați `errno` **doar** după ce apelul a întors `-1`. Dacă apelul a reușit, valoarea din `errno` nu înseamnă nimic.

---

### 2.5. Valgrind (doar de citit, pe Linux)

**Valgrind** rulează programul și urmărește memoria și fișierele. La final, spune ce ați uitat. Merge **doar pe Linux** (nu pe macOS cu procesor Apple), așa că **nu îl folosim în exerciții**. Trebuie doar să știți să citiți un raport.

```bash
valgrind --leak-check=full --track-fds=yes ./program
```

Exemplu de program cu trei greșeli:

```c
char *buf = malloc(100);               /* 1. malloc fără free          */
int fd = open("mesaj.txt", O_RDONLY);  /* 2. open fără close           */
buf[100] = 'x';                        /* 3. un octet după final       */
```

| Ce scrie Valgrind | Ce înseamnă | Cum repari |
| :--- | :--- | :--- |
| `Invalid write of size 1 ... (greseli.c:7)` | ați scris în afara zonei alocate, la linia 7 | indicii merg de la `0` la `99` |
| `FILE DESCRIPTORS: 4 open (3 std)` + `Open file descriptor 3: mesaj.txt` | un fișier a rămas deschis | `close(fd)` |
| `definitely lost: 100 bytes in 1 blocks` | memorie alocată și neeliberată | `free(buf)` |
| `All heap blocks were freed` și `ERROR SUMMARY: 0 errors` | totul e în regulă | |

---

## 3. Tabel Sinoptic

| Sintaxă | Pe scurt | Manual |
| :--- | :--- | :--- |
| `int fd = open(cale, O_RDONLY)` | deschide pentru citire | `man 2 open` |
| `open(cale, O_WRONLY \| O_CREAT \| O_TRUNC, 0644)` | deschide pentru scriere, creează / golește | `man 2 open` |
| `ssize_t n = read(fd, buf, cât)` | citește cel mult `cât` octeți | `man 2 read` |
| `write(fd, buf, n)` | scrie `n` octeți | `man 2 write` |
| `close(fd)` | închide | `man 2 close` |
| `0`, `1`, `2` | stdin, stdout, stderr | |
| `perror("text")` | `text: <mesajul pentru errno>` pe stderr | `man 3 perror` |
| `errno` | codul ultimei erori | `man 3 errno` |
| `FILE *f = fopen(...)` | varianta din bibliotecă (peste `open`) | `man 3 fopen` |

---

## 4. Exercițiul C: un „mini cp”

Programul `exercitiu.c` copiază un fișier în altul, folosind doar apeluri sistem:

```text
Utilizare: ./exercitiu <sursa> <destinatie>
```

Ce trebuie să facă:

1. **Verifică argumentele:** exact două, altfel mesaj de utilizare și cod `1`.
2. **Deschide sursa** cu `O_RDONLY`. La eroare: `perror(argv[1])` și cod `1`.
3. **Deschide destinația** cu `O_WRONLY | O_CREAT | O_TRUNC, 0644`. La eroare: `perror`, închide sursa, cod `1`.
4. **Copiază** cu bucla `read` / `write`, câte 64 de octeți.
5. **Închide** ambii descriptori. Abia apoi afișează `Am copiat N octeti din ... in ...`.

```bash
$ ./exercitiu mesaj.txt copie.txt; echo "cod: $?"
Am copiat 204 octeti din mesaj.txt in copie.txt
cod: 0

$ ./exercitiu lipsa.txt copie.txt; echo "cod: $?"
lipsa.txt: No such file or directory
cod: 1
```

Pe lângă programul C, `verifica.sh` are **2 exerciții scurte** de Bash care folosesc programul vostru: prind eroarea într-un fișier (`2>`) și verifică copia cu `cmp`.

---

## 5. Structura Directorului & Ghid Practic de Rulare

```text
tutoriat-04/
├── README.md                      # Prezentul breviar și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── img/                           # Capturile de terminal folosite în slide-uri
├── schelet/                       # Codul de lucru pentru studenți
│   ├── Makefile                   # Compilare, teste, script
│   ├── exercitiu.c                # Mini cp, cu instrucțiuni /* TODO */
│   ├── verifica.sh                # 2 exerciții de Bash (de completat)
│   └── mesaj.txt                  # Fișierul pe care îl copiem
└── solutii/                       # Soluțiile complete
    ├── Makefile
    ├── exercitiu.c
    ├── verifica.sh
    └── mesaj.txt
```

### Ce vă trebuie

- **Linux / WSL:** `sudo apt install build-essential` (aveți deja din tutoriatele trecute).
- **macOS:** o singură dată `xcode-select --install` (instalează `gcc`/`clang` și `make`). Terminalul și `bash` sunt deja acolo.

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-04/schelet
   ```
2. **Deschideți `exercitiu.c`** și completați `/* TODO 1..5 */`.
3. **Compilați și testați:**
   ```bash
   make            # compilează cu -Wall -Wextra -std=c11 -pedantic -g
   make test       # 5 teste: fără argumente, copiere, cmp, sursă lipsă, destinație imposibilă
   ```
4. **Exercițiile din `verifica.sh`**, care folosesc programul vostru:
   ```bash
   make script     # echivalent cu: bash verifica.sh
   ```
   La final trebuie să vedeți `OK: copie.txt este identic cu mesaj.txt`.
5. **Curățați fișierele generate:** `make clean`.

---

## 6. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `write(fd, buf, 64)` în loc de `write(fd, buf, n)`
- **Cauză:** ultima bucată citită are de obicei mai puțin de 64 de octeți. Restul din `buf` sunt resturi de la citirea anterioară.
- **Remediere:** scrieți exact cât a întors `read`: `write(fd, buf, n)`.

### Capcana 2: `open` fără `O_CREAT` → `No such file or directory` la destinație
- **Cauză:** fără `O_CREAT`, `open` nu creează fișierul.
- **Remediere:** `O_WRONLY | O_CREAT | O_TRUNC` și al treilea argument, drepturile: `0644`.

### Capcana 3: `0644` scris ca `644`
- **Cauză:** `644` fără `0` în față e un număr zecimal, deci drepturi aiurea.
- **Remediere:** drepturile se scriu în octal, cu `0` în față: `0644` = `rw-r--r--`.

### Capcana 4: nu verificați dacă `open` a întors `-1`
- **Cauză:** programul merge mai departe cu `fd = -1`, iar `read(-1, ...)` dă `Bad file descriptor`.
- **Remediere:** `if (fd == -1) { perror(...); return 1; }` imediat după `open`.

### Capcana 5: uitați `close`
- **Cauză:** în programe mici nu se vede nimic, dar un program care deschide multe fișiere rămâne fără descriptori.
- **Remediere:** fiecare `open` reușit are un `close`. Pe Linux, `valgrind --track-fds=yes` vi-l arată pe cel uitat.

### Capcana 6: `printf` pentru erori
- **Cauză:** `printf` scrie pe stdout (`1`), deci eroarea ajunge în fișier la `./exercitiu ... > out.txt`.
- **Remediere:** `perror(...)` sau `fprintf(stderr, ...)`, care scriu pe descriptorul `2`.
