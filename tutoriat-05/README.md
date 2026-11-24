# Tutoriatul 5: I-noduri, link-uri și directoare în C

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Idei, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)  
> **Merge pe:** Linux și macOS

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic](#2-breviar-teoretic)
   - [2.1. Numele nu e fișierul: i-nodul](#21-numele-nu-e-fișierul-i-nodul)
   - [2.2. Directorul: o listă de perechi nume → i-nod](#22-directorul-o-listă-de-perechi-nume--i-nod)
   - [2.3. Hard link (`ln`)](#23-hard-link-ln)
   - [2.4. Symlink (`ln -s`)](#24-symlink-ln--s)
   - [2.5. Directoare în C: `opendir`, `readdir`, `closedir`](#25-directoare-în-c-opendir-readdir-closedir)
3. [Tabel Sinoptic](#3-tabel-sinoptic)
4. [Exercițiul C: un „mini `ls -i`”](#4-exercițiul-c-un-mini-ls--i)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Capcane Uzuale de Examen & Erori Frecvente](#6-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe, fiecare student va fi capabil:
- Să explice ce este un **i-nod** și de ce numele unui fișier **nu** stă în i-nod.
- Să vadă numerele de i-nod cu `ls -i` și numărul de nume (link-uri) cu `ls -l`.
- Să facă un **hard link** (`ln`) și un **symlink** (`ln -s`) și să spună ce se întâmplă cu fiecare când ștergi originalul.
- Să parcurgă un director în C cu `opendir`, `readdir` și `closedir`.

---

## 2. Breviar Teoretic

### 2.1. Numele nu e fișierul: i-nodul

Pe disc, fiecare fișier are un **i-nod** (index node): o fișă cu tot ce știe sistemul despre el, mai puțin numele.

```text
i-nodul 589875
├── tipul:            fișier obișnuit
├── drepturile:       rw-r--r--
├── proprietarul:     student
├── mărimea:          21 de octeți
├── câte nume are:    2               ← „link count”, coloana a doua din ls -l
└── unde sunt datele: blocurile 1200, 1201...
```

Numărul i-nodului îl vedeți cu `ls -i`:

```bash
$ ls -i demo
589875 a.txt   589875 a_hard.txt   589878 a_soft.txt   589876 b.txt   589874 sub
```

> [!NOTE]
> „Sistemul de fișiere EXT” (ext2, ext3, ext4) e cel folosit de obicei pe Linux. Ideea de i-nod e la fel și pe macOS (APFS): `ls -i` merge identic.

---

### 2.2. Directorul: o listă de perechi nume → i-nod

Un **director** e tot un fișier, dar conținutul lui e o listă simplă:

| nume | i-nod |
| :--- | :--- |
| `.` | 589873 (directorul însuși) |
| `..` | 589866 (părintele) |
| `.ascuns` | 589877 (fișier ascuns) |
| `a.txt` | 589875 |
| `a_hard.txt` | 589875 |
| `a_soft.txt` | 589878 |
| `b.txt` | 589876 |
| `sub` | 589874 |

Când scrieți `cat demo/a.txt`, sistemul caută `a.txt` în lista lui `demo`, găsește `589875` și citește datele i-nodului 589875.

> [!TIP]
> `.` și `..` sunt și ele intrări din listă. De aceea un director gol are deja 2 nume (`ls -ld` arată `2`): numele lui din părinte și `.` din el însuși.

---

### 2.3. Hard link (`ln`)

```bash
ln a.txt a_hard.txt      # un al doilea NUME pentru același i-nod
```

- `a.txt` și `a_hard.txt` au **același număr de i-nod**: sunt **același fișier**, cu două nume.
- Ce scrieți prin unul se vede prin celălalt.
- `rm a.txt` șterge doar un **nume**. Datele dispar abia când nu mai rămâne niciun nume (link count = 0).
- Nu puteți face hard link la un director și nici între două partiții diferite.

---

### 2.4. Symlink (`ln -s`)

```bash
ln -s a.txt a_soft.txt   # un fișier mic care ține minte un DRUM: "a.txt"
```

- `a_soft.txt` are **alt i-nod**, iar în `ls -l` apare cu `l` la început și cu săgeată: `a_soft.txt -> a.txt`.
- Când îl deschideți, sistemul urmează drumul scris în el.
- Dacă ștergeți `a.txt`, symlink-ul rămâne, dar arată spre ceva ce nu mai există: **link rupt** (`No such file or directory`).
- Poate arăta spre directoare și spre alte partiții.

| | Hard link | Symlink |
| :--- | :--- | :--- |
| Comanda | `ln sursa nume` | `ln -s sursa nume` |
| I-nod | **același** | **altul** |
| Ține minte | nimic în plus, e doar un nume | un drum (text) |
| Dacă ștergi originalul | merge mai departe | se rupe |
| Spre director | nu | da |
| În C | `link(vechi, nou)` | `symlink(tinta, nume)` |

> [!WARNING]
> Drumul dintr-un symlink se citește **din folderul în care stă link-ul**. `ln -s original.txt lab/soft.txt` e corect; `ln -s lab/original.txt lab/soft.txt` face un link spre `lab/lab/original.txt`, adică rupt.

---

### 2.5. Directoare în C: `opendir`, `readdir`, `closedir`

```c
#include <dirent.h>

DIR *d = opendir("demo");             /* NULL la eroare (nu -1!) */
struct dirent *e;
while ((e = readdir(d)) != NULL) {    /* NULL = nu mai sunt intrări */
    printf("%lu %s\n", (unsigned long)e->d_ino, e->d_name);
}
closedir(d);
```

| Apel | Ce face | Întoarce |
| :--- | :--- | :--- |
| `opendir(cale)` | deschide directorul | `DIR *`, sau `NULL` la eroare |
| `readdir(d)` | dă **următoarea** intrare din listă | `struct dirent *`, sau `NULL` la final |
| `closedir(d)` | închide directorul | `0`, sau `-1` |

Din `struct dirent` folosim doar două câmpuri: `d_name` (numele) și `d_ino` (numărul i-nodului). Exact perechea din tabelul de la 2.2.

> [!IMPORTANT]
> `readdir` întoarce **și** `.` și `..`, și le dă **în ordinea de pe disc**, nu alfabetic. `ls` sortează și ascunde ce începe cu `.`; programul nostru doar le ascunde.

---

## 3. Tabel Sinoptic

| Sintaxă | Pe scurt | Manual |
| :--- | :--- | :--- |
| `ls -i` | arată numărul i-nodului | `man 1 ls` |
| `ls -l` | coloana a doua = câte nume are fișierul | `man 1 ls` |
| `ln sursa nume` | hard link: încă un nume, același i-nod | `man 1 ln` |
| `ln -s sursa nume` | symlink: fișier nou care ține minte un drum | `man 1 ln` |
| `[ -e f ]`, `[ -L f ]` | există? / e symlink? (un link rupt: `-L` adevărat, `-e` fals) | `man test` |
| `DIR *d = opendir(cale)` | deschide un director, `NULL` la eroare | `man 3 opendir` |
| `struct dirent *e = readdir(d)` | următoarea intrare, `NULL` la final | `man 3 readdir` |
| `e->d_name`, `e->d_ino` | numele și i-nodul intrării | `man 3 readdir` |
| `closedir(d)` | închide directorul | `man 3 closedir` |
| `link(vechi, nou)`, `symlink(tinta, nume)` | `ln` și `ln -s`, din C | `man 2 link`, `man 2 symlink` |

---

## 4. Exercițiul C: un „mini `ls -i`”

Programul `exercitiu.c` afișează conținutul unui director, câte o intrare pe linie: numărul i-nodului și numele.

```text
Utilizare: ./exercitiu [director]       (fără argument: directorul curent)
```

Ce trebuie să facă:

1. **Verifică argumentele:** cel mult unul, altfel mesaj de utilizare și cod `1`.
2. **Deschide directorul** cu `opendir`. La `NULL`: `perror(dir)` și cod `1`.
3. **Citește intrările** cu `readdir` până la `NULL`. Sare peste numele care încep cu `.` și afișează `i-nod nume`.
4. **Închide** directorul cu `closedir`.

```bash
$ make demo                # face folderul demo/ cu un hard link și un symlink
$ ./exercitiu demo
589876 b.txt
589875 a_hard.txt
589878 a_soft.txt
589874 sub
589875 a.txt               # același număr ca a_hard.txt: e același fișier!

$ ./exercitiu lipsa; echo "cod: $?"
lipsa: No such file or directory
cod: 1
```

Numerele de i-nod și ordinea liniilor diferă de la un calculator la altul.


---

## 5. Structura Directorului & Ghid Practic de Rulare

```text
tutoriat-05/
├── README.md                      # Prezentul breviar și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── img/                           # Capturile de terminal folosite în slide-uri
├── schelet/                       # Codul de lucru pentru studenți
│   ├── Makefile                   # Compilare, demo/, teste
│   └── exercitiu.c                # Mini ls -i, cu instrucțiuni /* TODO */
├── solutii/                       # Soluțiile complete
│   ├── Makefile
│   └── exercitiu.c
└── kahoot/                        # Quiz-ul săptămânal (păstrat local, nu e public)
```

Folderul `demo/` nu e în git (git nu păstrează hard link-urile): îl face `make demo`, iar `make test` și `make run` îl refac singure.

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-05/schelet
   ```
2. **Deschideți `exercitiu.c`** și completați `/* TODO 1..4 */`.
3. **Compilați și testați:**
   ```bash
   make            # compilează cu -Wall -Wextra -std=c11 -pedantic -g
   make test       # 5 teste: argumente, listare, comparație cu ls -i, director lipsă, fișier în loc de director
   ```
4. **Curățați fișierele generate:** `make clean` (șterge și `demo/`).

Pe macOS: o singură dată `xcode-select --install`, apoi aceleași comenzi.

---

## 6. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `if (d == -1)` după `opendir`
- **Cauză:** `opendir` întoarce un pointer (`DIR *`), nu un număr. La eroare dă `NULL`.
- **Remediere:** `if (d == NULL) { perror(dir); return 1; }`.

### Capcana 2: în listă apar `.` și `..`
- **Cauză:** `readdir` întoarce toate intrările din director, inclusiv cele două speciale.
- **Remediere:** `if (e->d_name[0] == '.') continue;` (sare și peste fișierele ascunse, ca `ls`).

### Capcana 3: „`readdir` nu sortează”
- **Cauză:** intrările vin în ordinea în care le ține sistemul de fișiere.
- **Remediere:** nu e o greșeală. Dacă vreți ordine, `./exercitiu demo | sort -k2`.

### Capcana 4: symlink rupt din cauza drumului
- **Cauză:** `ln -s lab/original.txt lab/soft.txt` scrie în link drumul `lab/original.txt`, citit din `lab/`.
- **Remediere:** drumul se scrie față de folderul link-ului: `ln -s original.txt lab/soft.txt`.

### Capcana 5: „am șters originalul, deci a dispărut și hard link-ul”
- **Cauză:** confuzie între nume și fișier.
- **Remediere:** `rm` șterge un nume. Fișierul rămâne cât timp are măcar un nume (coloana a doua din `ls -l`).

### Capcana 6: uitați `closedir`
- **Cauză:** la fel ca `close` din Tutoriatul 04, fiecare `opendir` ține ocupat un descriptor.
- **Remediere:** fiecare `opendir` reușit are un `closedir`.
