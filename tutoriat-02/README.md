# Tutoriatul 2: Utilitare Text, Filtre, Despecializare și Redirectări

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Teorie & Diagrame, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic & Diagrame](#2-breviar-teoretic--diagrame)
   - [2.1. Fluxurile standard: stdin, stdout, stderr](#21-fluxurile-standard-stdin-stdout-stderr)
   - [2.2. Redirectări](#22-redirectări)
   - [2.3. Conducte (pipe-uri) și filosofia UNIX](#23-conducte-pipe-uri-și-filosofia-unix)
   - [2.4. Filtre de text: grep, sort, uniq, wc, cut, tr, head, tail](#24-filtre-de-text-grep-sort-uniq-wc-cut-tr-head-tail)
   - [2.5. Despecializarea (quoting): ghilimele și backslash](#25-despecializarea-quoting-ghilimele-și-backslash)
   - [2.6. Codul de ieșire în pipe-uri și operatorii && și ||](#26-codul-de-ieșire-în-pipe-uri-și-operatorii--și-)
3. [Tabel Sinoptic: Filtre și Operatori](#3-tabel-sinoptic-filtre-și-operatori)
4. [Exercițiul C: un mini-grep care se comportă ca un filtru UNIX](#4-exercițiul-c-un-mini-grep-care-se-comportă-ca-un-filtru-unix)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Bonus: Primul contact cu Valgrind](#6-bonus-primul-contact-cu-valgrind)
7. [Capcane Uzuale de Examen & Erori Frecvente](#7-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe practice, fiecare student va fi capabil:
- Să explice ce sunt **descriptorii standard** `0` (stdin), `1` (stdout) și `2` (stderr) și de ce erorile au un canal separat.
- Să folosească redirectările `<`, `>`, `>>`, `2>`, `2>&1`, `&>` și „gaura neagră” `/dev/null`.
- Să construiască **conducte** (`|`) din filtre mici: `grep`, `sort`, `uniq`, `wc`, `cut`, `tr`, `head`, `tail`.
- Să înțeleagă de ce `uniq` are aproape mereu nevoie de un `sort` în fața lui.
- Să aleagă corect între **ghilimele simple**, **ghilimele duble** și **backslash** pentru a opri interpretarea caracterelor speciale de către shell.
- Să scrie în C un program care **se comportă ca un filtru UNIX**: citește din fișier sau din stdin, scrie datele pe stdout, mesajele pe stderr și întoarce un cod de ieșire cu sens.

---

## 2. Breviar Teoretic & Diagrame

### 2.1. Fluxurile standard: stdin, stdout, stderr

Orice proces Linux pornește cu **trei fișiere deja deschise**, identificate prin numere mici numite **descriptori de fișier**:

```text
                      +---------------------------+
   tastatură  ──(0)──>|                           |──(1)──> ecran   (stdout: datele, rezultatul)
   stdin              |         PROCES            |
                      |   (ex: grep, sort, ./a)   |──(2)──> ecran   (stderr: erori, diagnostic)
                      +---------------------------+
```

| Descriptor | Nume | În C (`stdio.h`) | În C (`unistd.h`) | Implicit legat de |
| :---: | :--- | :--- | :--- | :--- |
| `0` | intrarea standard | `stdin` | `STDIN_FILENO` | tastatura (terminalul) |
| `1` | ieșirea standard | `stdout` | `STDOUT_FILENO` | ecranul (terminalul) |
| `2` | ieșirea de eroare | `stderr` | `STDERR_FILENO` | ecranul (terminalul) |

> [!IMPORTANT]
> **De ce avem două ieșiri care ajung amândouă pe ecran?** Ca să le putem separa! Când trimiteți rezultatul unui program într-un fișier sau într-un pipe, vreți acolo **doar datele**. Mesajele de eroare trebuie să ajungă în continuare la om, pe ecran. De aceea un program bine scris pune erorile cu `fprintf(stderr, ...)` sau `perror()`, niciodată cu `printf()`.

---

### 2.2. Redirectări

Redirectarea este făcută de **shell, înainte de a porni comanda**: shell-ul deschide fișierul și îl „leagă” de descriptorul cerut. Programul nu știe și nu-l interesează unde scrie de fapt.

| Sintaxă | Efect | Exemplu |
| :--- | :--- | :--- |
| `cmd < f` | stdin (0) citește din fișierul `f` | `wc -l < date.in` |
| `cmd > f` | stdout (1) scrie în `f` (**îl golește întâi** / îl creează) | `ls > lista.txt` |
| `cmd >> f` | stdout (1) **adaugă** la finalul lui `f` | `date >> jurnal.txt` |
| `cmd 2> f` | stderr (2) scrie în `f` | `gcc prog.c 2> erori.txt` |
| `cmd 2>> f` | stderr (2) adaugă la finalul lui `f` | `make 2>> build_erori.txt` |
| `cmd > f 2>&1` | stdout în `f`, apoi stderr **acolo unde e acum stdout** (tot în `f`) | `make > tot.txt 2>&1` |
| `cmd &> f` | prescurtare Bash pentru `> f 2>&1` | `make &> tot.txt` |
| `cmd 2> /dev/null` | aruncă erorile (`/dev/null` înghite tot ce primește) | `find / -name passwd 2> /dev/null` |

```text
   ./exercitiu -i linux date.in  > gasite.out  2> erori.out

                      +-----------+──(1)──> gasite.out   (liniile găsite)
        date.in ────> | exercitiu |
                      +-----------+──(2)──> erori.out    ([filtru] 4 din 12 linii...)
```

> [!WARNING]
> **Ordinea contează la `2>&1`!** Shell-ul citește redirectările de la stânga la dreapta.
> - `cmd > f 2>&1` → stdout merge în `f`, apoi stderr merge „unde e stdout acum” = `f`. ✔ Ambele în fișier.
> - `cmd 2>&1 > f` → stderr merge „unde e stdout acum” = ecranul, apoi stdout merge în `f`. ✘ Erorile rămân pe ecran.

---

### 2.3. Conducte (pipe-uri) și filosofia UNIX

> *„Scrieți programe care fac un singur lucru și îl fac bine. Scrieți programe care lucrează împreună. Scrieți programe care lucrează cu fluxuri de text, pentru că aceasta este o interfață universală.”* — Doug McIlroy

Operatorul `|` leagă **stdout-ul** comenzii din stânga de **stdin-ul** comenzii din dreapta. Toate comenzile din pipe pornesc **în același timp**, ca procese separate, iar datele curg printre ele pe măsură ce sunt produse.

```text
  cut -d' ' -f3 jurnal.txt  |  sort  |  uniq -c  |  sort -nr  |  head -n 3

  +-----+  ana   +------+  ana   +---------+ 6 ana  +----------+ 6 ana  +------+
  | cut |──────> | sort |──────> | uniq -c |──────> | sort -nr |──────> | head |──> ecran
  +-----+ bogdan +------+  ana   +---------+ 3 bog. +----------+ 5 root +------+
     |     root            ana                                              
     v      ...             ...       stderr-ul fiecărei comenzi merge tot pe ecran
   (2) ─────────────────────────────────────────────────────────────────────────> ecran
```

> [!NOTE]
> Pipe-ul transportă **doar stdout**. Dacă o comandă din mijloc scrie o eroare pe stderr, ea apare pe ecran și nu „strică” datele care curg mai departe. Dacă vreți să treacă și stderr prin pipe: `cmd 2>&1 | less` (sau `cmd |& less` în Bash).

---

### 2.4. Filtre de text: grep, sort, uniq, wc, cut, tr, head, tail

Un **filtru** este un program care citește text (din fișierele primite ca argumente sau, dacă nu primește niciunul, de la stdin), îl transformă și scrie rezultatul pe stdout.

Pentru exemple folosim fișierul `jurnal.txt`, cu 5 câmpuri separate prin spațiu:

```text
2026-10-27 08:02:11 ana login ok
2026-10-27 08:06:02 root ssh esuat
...
```

#### `grep`: păstrează liniile care conțin un tipar

| Opțiune | Efect | Exemplu |
| :--- | :--- | :--- |
| (nimic) | liniile care conțin tiparul | `grep esuat jurnal.txt` |
| `-i` | ignoră diferența între litere mari și mici | `grep -i linux date.in` |
| `-v` | inversează: liniile care **nu** conțin tiparul | `grep -v ok jurnal.txt` |
| `-n` | afișează și numărul liniei | `grep -n sudo jurnal.txt` |
| `-c` | afișează doar **câte** linii se potrivesc | `grep -c esuat jurnal.txt` → `9` |
| `-w` | doar cuvinte întregi (`ana` nu prinde `banana`) | `grep -w ana jurnal.txt` |
| `-r` | caută recursiv în toate fișierele dintr-un director | `grep -rn TODO schelet/` |
| `^` / `$` | ancore: început / sfârșit de linie (expresii regulate) | `grep '^2026-10-28' jurnal.txt` |

**Codul de ieșire al lui `grep`:** `0` = a găsit cel puțin o linie, `1` = n-a găsit nimic, `2` = eroare (ex: fișier inexistent). Exercițiul nostru C respectă exact aceeași convenție.

#### `sort` și `uniq`: ordonare și eliminarea dublurilor

| Comandă | Efect |
| :--- | :--- |
| `sort f` | ordonează liniile alfabetic |
| `sort -n` | ordonează **numeric** (`2 < 10`; alfabetic ar fi `10 < 2`!) |
| `sort -r` | ordine inversă (descrescătoare) |
| `sort -u` | ordonează și păstrează o singură apariție a fiecărei linii |
| `sort -t: -k3 -n` | ordonează numeric după câmpul 3, cu `:` ca separator |
| `uniq` | elimină dublurile **vecine** (linii identice una după alta) |
| `uniq -c` | numără câte linii identice vecine erau |
| `uniq -d` | afișează doar liniile care apar de mai multe ori |

> [!WARNING]
> **`uniq` vede doar liniile vecine!** Pe intrarea `ana / bogdan / ana`, `uniq` nu elimină nimic, pentru că cele două `ana` nu sunt una lângă alta. De aceea rețeta clasică este **`sort | uniq -c`**: `sort` aduce dublurile alături, `uniq -c` le numără.

#### `wc`, `cut`, `tr`, `head`, `tail`

| Comandă | Efect | Exemplu |
| :--- | :--- | :--- |
| `wc -l` / `-w` / `-c` | numără linii / cuvinte / octeți | `wc -l jurnal.txt` → `25 jurnal.txt` |
| `wc -l < f` | același număr, **fără numele fișierului** | `wc -l < jurnal.txt` → `25` |
| `cut -d' ' -f3` | extrage câmpul 3, câmpurile fiind separate de spațiu | utilizatorii din jurnal |
| `cut -d: -f1,7` | câmpurile 1 și 7, separate de `:` | `cut -d: -f1,7 /etc/passwd` |
| `cut -c1-10` | caracterele 1..10 ale fiecărei linii | data din jurnal |
| `tr 'a-z' 'A-Z'` | înlocuiește caracter cu caracter (citește **doar** din stdin) | `tr 'a-z' 'A-Z' < date.in` |
| `tr -d '0-9'` | șterge caracterele din set | elimină cifrele |
| `head -n 5` | primele 5 linii | `head -n 5 jurnal.txt` |
| `tail -n 5` | ultimele 5 linii | `tail -n 5 jurnal.txt` |
| `tail -f f` | urmărește fișierul pe măsură ce crește (ieși cu `Ctrl+C`) | `tail -f /var/log/syslog` |

---

### 2.5. Despecializarea (quoting): ghilimele și backslash

Înainte să pornească o comandă, shell-ul **interpretează** unele caractere: `$` (variabile), `*` `?` `[ ]` (măști), spațiul (separă argumentele), `|` `<` `>` `;` `&` (operatori). **Despecializarea** înseamnă să-i spunem shell-ului „acest caracter e doar un caracter, nu-l interpreta”.

| Formă | Ce mai interpretează shell-ul înăuntru | Exemplu | Rezultat (cu `HOME=/home/student`) |
| :--- | :--- | :--- | :--- |
| fără ghilimele | tot: `$`, măști, spații | `echo $HOME *.c` | `/home/student exercitiu.c` |
| `"..."` duble | **doar** `$`, `` ` `` și `\` | `echo "$HOME *.c"` | `/home/student *.c` |
| `'...'` simple | **nimic** | `echo '$HOME *.c'` | `$HOME *.c` |
| `\c` backslash | despecializează **un singur** caracter | `echo \$HOME \*.c` | `$HOME *.c` |

Exemple care apar des:

```bash
grep 'ana login' jurnal.txt     # tiparul are spațiu: fără ghilimele, grep ar căuta "ana" în fișierul "login"
grep '^2026' jurnal.txt         # ^ și * în tipare: puneți-le mereu între ghilimele simple
echo "Utilizator: $USER"        # vrem variabila expandată, dar textul păstrat într-un singur argument
echo 'Pretul este $5 * 2'       # vrem textul exact așa cum e scris
touch "fisier cu spatii.txt"    # un singur fișier, nu trei
```

> [!TIP]
> **Regula practică:** puneți tiparele pentru `grep` și seturile pentru `tr` **mereu între ghilimele simple**, iar variabilele **mereu între ghilimele duble** (`"$fisier"`). Așa nu vă surprinde niciodată shell-ul.

---

### 2.6. Codul de ieșire în pipe-uri și operatorii `&&` și `||`

- Codul de ieșire al unui pipe (`echo $?`) este codul **ultimei** comenzi din pipe.
- `cmd1 && cmd2` → rulează `cmd2` **doar dacă** `cmd1` a reușit (cod `0`).
- `cmd1 || cmd2` → rulează `cmd2` **doar dacă** `cmd1` a eșuat (cod nenul).
- `cmd1 ; cmd2` → rulează `cmd2` oricum.

```bash
grep -q root jurnal.txt && echo "root a încercat să intre"
make || echo "Compilarea a eșuat, citește erorile de mai sus"
./exercitiu -c esuat jurnal.txt 2> /dev/null; echo "cod: $?"
```

---

## 3. Tabel Sinoptic: Filtre și Operatori

| Simbol / Comandă | Pe scurt | Exemplu |
| :--- | :--- | :--- |
| `\|` | stdout-ul din stânga devine stdin-ul din dreapta | `ls -l \| wc -l` |
| `<` | citește stdin din fișier | `sort < nume.txt` |
| `>` | scrie stdout în fișier (suprascrie) | `sort nume.txt > sortat.txt` |
| `>>` | adaugă stdout la finalul fișierului | `echo gata >> jurnal.txt` |
| `2>` | scrie stderr în fișier | `gcc x.c 2> erori.txt` |
| `2>&1` | stderr merge unde merge stdout | `make > log.txt 2>&1` |
| `/dev/null` | fișierul care înghite tot | `cmd 2> /dev/null` |
| `grep` | păstrează liniile care conțin tiparul | `grep -in linux date.in` |
| `sort` | ordonează liniile | `sort -nr` |
| `uniq` | elimină / numără dublurile vecine | `sort \| uniq -c` |
| `wc` | numără linii, cuvinte, octeți | `wc -l` |
| `cut` | extrage coloane | `cut -d' ' -f3` |
| `tr` | înlocuiește / șterge caractere | `tr 'a-z' 'A-Z'` |
| `head` / `tail` | primele / ultimele linii | `head -n 3` |
| `tee` | scrie și în fișier, și pe stdout (o „ramificație” în pipe) | `make \| tee build.txt` |
| `less` | afișează pagină cu pagină (`q` ca să ieși) | `man bash \| less` |

---

## 4. Exercițiul C: un mini-grep care se comportă ca un filtru UNIX

Programul `exercitiu.c` este o versiune simplificată a lui `grep`, cu opțiunile `-i`, `-v`, `-n`, `-c`:

```text
Utilizare: ./exercitiu [-i] [-v] [-n] [-c] <tipar> [fisier]
```

Ce trebuie să respecte, exact ca un utilitar UNIX adevărat:

1. **Citește din fișier sau din stdin.** Dacă lipsește `[fisier]` (sau este `-`), citește de la intrarea standard. Astfel merge și în `cat date.in | ./exercitiu linux`, și în `./exercitiu linux < date.in`.
2. **Datele pe stdout, restul pe stderr.** Liniile găsite merg pe stdout; mesajul de utilizare, erorile și statistica `[filtru] 4 din 12 linii potrivite` merg pe stderr. Așa, `./exercitiu linux date.in | wc -l` numără doar liniile găsite.
3. **Cod de ieșire ca la `grep`:** `0` = a găsit ceva, `1` = n-a găsit nimic, `2` = eroare (argumente greșite, fișier inexistent).
4. **Nu închide ce n-a deschis.** `fclose()` se apelează doar pe fișierul deschis cu `fopen()`, nu pe `stdin`.

Exemple de rulare cu soluția completă:

```bash
$ ./exercitiu -n Linux date.in
1:Linux este un nucleu de sistem de operare scris in C.
5:Kernelul Linux gestioneaza procesele si memoria.
[filtru] 2 din 12 linii potrivite (sursa: date.in)

$ cat date.in | ./exercitiu -c -i linux 2> /dev/null
4

$ ./exercitiu -v -i linux < date.in 2> /dev/null | wc -l
8

$ ./exercitiu windows date.in; echo $?
[filtru] 0 din 12 linii potrivite (sursa: date.in)
1
```

Pe lângă programul C, fișierul `pipeline.sh` conține **13 exerciții cu pipe-uri și redirectări** pe `jurnal.txt` și `date.in`, fiecare cu un indiciu. Înlocuiți fiecare `todo` cu comanda voastră și rulați scriptul cu `bash pipeline.sh`.

---

## 5. Structura Directorului & Ghid Practic de Rulare

```text
tutoriat-02/
├── README.md                      # Prezentul breviar teoretic și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── schelet/                       # Codul de lucru pentru studenți
│   ├── Makefile                   # Compilare, teste, pipeline, valgrind
│   ├── exercitiu.c                # Mini-grep cu instrucțiuni /* TODO */
│   ├── pipeline.sh                # 13 exerciții cu pipe-uri și redirectări (de completat)
│   ├── date.in                    # Text de test pentru exercitiu.c
│   └── jurnal.txt                 # Jurnal de autentificări pentru exercițiile cu pipe-uri
├── solutii/                       # Soluțiile complete demonstrate la tablă
│   ├── Makefile
│   ├── exercitiu.c                # Mini-grep complet implementat și documentat
│   ├── pipeline.sh                # Toate cele 13 exerciții rezolvate și explicate
│   ├── date.in
│   └── jurnal.txt
└── kahoot/                        # Quiz-ul săptămânal (păstrat local, nu e public)
```

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-02/schelet
   ```
2. **Încălzire cu pipe-uri:** deschideți `pipeline.sh`, rezolvați exercițiile 1–11 și rulați:
   ```bash
   bash pipeline.sh
   ```
3. **Deschideți `exercitiu.c`** și completați secțiunile `/* TODO 1..9 */`. Parsarea argumentelor este deja scrisă.
4. **Compilați și testați:**
   ```bash
   make            # compilează cu -Wall -Wextra -std=c11 -pedantic -g
   make test       # 6 teste: fără argumente, fișier, pipe, redirectări, negăsit, fișier lipsă
   make pipeline   # rulează pipeline.sh, inclusiv exercițiile 12–13 care folosesc programul vostru
   ```
5. **Comparați cu ce face `grep`:** rezultatul vostru trebuie să fie identic.
   ```bash
   ./exercitiu -i linux date.in 2> /dev/null > al_meu.out
   grep -i linux date.in > grep.out
   diff al_meu.out grep.out && echo "Identic!"
   ```
6. **Curățați fișierele generate:**
   ```bash
   make clean
   ```

---

## 6. Bonus: Primul contact cu Valgrind

**Valgrind** rulează programul vostru într-un mediu controlat și urmărește fiecare octet de memorie alocat și fiecare fișier deschis. Îl vom folosi pe larg în [Tutoriatul 04](../README.md#planul-celor-10-tutoriate), când lucrăm cu `malloc()`, `open()` și `close()`; azi îl încercăm doar o dată, pe programul nostru.

```bash
make valgrind
# echivalent cu:
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./exercitiu -i linux date.in
```

Cu soluția corectă, raportul se termină cu:

```text
==594== FILE DESCRIPTORS: 3 open (3 std) at exit.
==594== All heap blocks were freed -- no leaks are possible
==594== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

**Experiment:** comentați apelul `fclose(in)` din `main()`, recompilați și rulați din nou `make valgrind`. Valgrind vă arată exact ce ați uitat și pe ce linie l-ați deschis:

```text
==665== FILE DESCRIPTORS: 4 open (3 std) at exit.
==665== Open file descriptor 3: date.in
==665==    by 0x1099AC: main (exercitiu.c:189)
...
==665== 472 bytes in 1 blocks are still reachable in loss record 1 of 1
==665==    by 0x48EAF5E: __fopen_internal (iofopen.c:65)
==665==    by 0x1099AC: main (exercitiu.c:189)
```

- **`4 open (3 std)`**: pe lângă 0, 1, 2 a rămas deschis descriptorul `3`, adică `date.in`.
- **`472 bytes ... still reachable`**: structura `FILE` alocată de `fopen()` n-a mai fost eliberată, pentru că doar `fclose()` o eliberează.
- Linia `main (exercitiu.c:189)` apare doar pentru că am compilat cu **`-g`**. Fără `-g`, Valgrind vede doar adrese.

> [!NOTE]
> Valgrind face programul de 10–50 de ori mai lent. Este o unealtă de verificare, nu de rulare zilnică.

---

## 7. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `sort date.txt > date.txt` lasă fișierul gol
- **Cauză:** Shell-ul deschide (și **golește**) `date.txt` pentru `>` *înainte* să pornească `sort`. Când `sort` citește, fișierul e deja gol.
- **Remediere:** scrieți într-un fișier nou și apoi redenumiți: `sort date.txt > tmp.txt && mv tmp.txt date.txt`, sau `sort -o date.txt date.txt`.

### Capcana 2: `uniq` „nu merge”
- **Cauză:** `uniq` elimină doar dublurile **vecine**.
- **Remediere:** `sort | uniq` (sau direct `sort -u`).

### Capcana 3: `cmd 2>&1 > f` lasă erorile pe ecran
- **Cauză:** redirectările se aplică de la stânga la dreapta; `2>&1` a copiat destinația lui stdout *de dinainte* de `> f`.
- **Remediere:** `cmd > f 2>&1` sau, în Bash, `cmd &> f`.

### Capcana 4: `grep ana login jurnal.txt` → `grep: login: No such file or directory`
- **Cauză:** fără ghilimele, spațiul separă argumentele. `grep` caută `ana` în fișierele `login` și `jurnal.txt`.
- **Remediere:** `grep 'ana login' jurnal.txt`.

### Capcana 5: `sort -n` vs `sort`
- **Cauză:** alfabetic, `10` vine înaintea lui `9` (se compară caracterul `1` cu `9`).
- **Remediere:** pentru numere folosiți `sort -n`; pentru „cel mai mare primul”, `sort -nr`.

### Capcana 6: mesajele de eroare ale programului vostru ajung în fișierul de rezultate
- **Cauză:** le-ați afișat cu `printf()`, adică pe stdout.
- **Remediere:** erorile se scriu cu `fprintf(stderr, ...)` sau `perror()`.

### Capcana 7: ordinea dintre stdout și stderr pare amestecată
- **Cauză:** când stdout merge într-un fișier sau pipe, `printf()` adună textul într-un **buffer** și îl scrie la final; stderr nu are buffer și apare imediat. Pe ecran (terminal), stdout se golește la fiecare `\n`, deci ordinea pare normală.
- **De reținut:** nu vă bazați pe ordinea dintre cele două fluxuri. Dacă aveți nevoie, apelați `fflush(stdout)`.
