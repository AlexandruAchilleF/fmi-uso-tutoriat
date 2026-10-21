# Tutoriatul 1: Concepte Fundamentale de SO & Linia de Comandă Linux (CLI)

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Teorie & Diagrame, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic & Diagrame Arhitecturale](#2-breviar-teoretic--diagrame-arhitecturale)
   - [2.1. Arhitectura stratificată a unui sistem de calcul](#21-arhitectura-stratificată-a-unui-sistem-de-calcul)
   - [2.2. Modurile de execuție: Kernel Mode vs User Mode](#22-modurile-de-execuție-kernel-mode-vs-user-mode)
   - [2.3. Sistemul de fișiere Linux: Arborele unic VFS](#23-sistemul-de-fișiere-linux-arborele-unic-vfs)
   - [2.4. Căi în sistemul de fișiere: Absolute vs Relative](#24-căi-în-sistemul-de-fișiere-absolute-vs-relative)
   - [2.5. Terminale fizice (TTY) vs Pseudoterminale (PTS)](#25-terminale-fizice-tty-vs-pseudoterminale-pts)
   - [2.6. Măști de căutare (Wildcards / Globbing)](#26-măști-de-căutare-wildcards--globbing)
   - [2.7. Variabile de mediu și căutarea binarilor ($PATH)](#27-variabile-de-mediu-și-căutarea-binarilor-path)
   - [2.8. Compilatorul GCC și pipeline-ul de compilare C](#28-compilatorul-gcc-și-pipeline-ul-de-compilare-c)
3. [Tabel Sinoptic: Comenzi Esențiale CLI](#3-tabel-sinoptic-comenzi-esențiale-cli)
4. [Configurarea Mediului Personal: Fișierul `~/.bashrc`](#4-configurarea-mediului-personal-fișierul-bashrc)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Capcane Uzuale de Examen & Erori Frecvente](#6-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe practice, fiecare student va fi capabil:
- Să explice **organizarea pe straturi** a sistemului de operare și granița de securitate impusă de inelele hardware ale procesorului (**Kernel Mode** vs **User Mode**).
- Să navigheze fluent și fără ezitare în arborele de fișiere Linux folosind căi absolute și relative.
- Să utilizeze eficient utilitarele de bază pentru manipularea fișierelor (`ls`, `cd`, `mkdir`, `rmdir`, `rm`, `cp`, `mv`, `cat`).
- Să stăpânească **măștile de căutare** (`*`, `?`, `[...]`) și să înțeleagă că expandarea este făcută de Shell, nu de comanda apelată.
- Să înțeleagă rolul variabilelor de mediu (`$HOME`, `$PATH`, `PS1`) și motivul de securitate pentru care directorul curent `.` nu este inclus în mod implicit în `$PATH`.
- Să scrie, să compileze cu flag-uri stricte de diagnostic (`gcc -Wall -Wextra -std=c11`) și să ruleze un prim program de sistem în C, interpretând codul de retur (`$?`).

---

## 2. Breviar Teoretic & Diagrame Arhitecturale

### 2.1. Arhitectura stratificată a unui sistem de calcul

Un sistem de calcul modern este organizat pe straturi de abstractizare pentru a izola complexitatea hardware-ului de utilizatorul final și a asigura stabilitatea:

```text
+-------------------------------------------------------------------+
|                     Aplicații Utilizator                          |
|             (Firefox, Editor C, Scripturi Bash)                   |
+-------------------------------------------------------------------+
|               Interfața cu Linia de Comandă (Shell)               |
|            Utilitare de Sistem (ls, cp, cat, grep, gcc)           |
+===================================================================+  <--- Granița User/Kernel
|                Interfața Apelurilor Sistem (Syscall)              |
+-------------------------------------------------------------------+
|                          NUCLEU (Kernel)                          |
|  - Gestiunea Proceselor & Planificator (Scheduler)                |
|  - Gestiunea Memoriei Virtuale (Paging, MMU)                      |
|  - Sistemul Virtual de Fișiere (VFS - EXT4, Btrfs etc.)           |
|  - Drivere de Dispozitive (Device Drivers) & Stivă Rețea          |
+-------------------------------------------------------------------+
|                         HARDWARE FIZIC                            |
|             (Procesor CPU, Memorie RAM, Discuri, I/O)             |
+-------------------------------------------------------------------+
```

- **Hardware**: Componentele fizice ce execută instrucțiuni și stochează octeți.
- **Kernel (Nucleul)**: Inima sistemului de operare. Rulează permanent în memorie, având control absolut asupra resurselor fizice.
- **Shell**: Interpretul de comenzi (uzual `bash` sau `zsh`) care preia comenzile text de la utilizator, le parsează, le expandează și lansează procese corespunzătoare.
- **Utilitare și Aplicații**: Programe compilate sau scripturi ce rulează exclusiv în spațiul utilizator (User Space).

---

### 2.2. Modurile de execuție: Kernel Mode vs User Mode

Procesoarele din familiile x86/x86_64 și ARM oferă suport hardware pentru **inele de protecție (Privilege Rings)**:

```text
       +--------------------------------------------+
       |   Ring 3: USER MODE (Neprivilegiat)        |
       |   - Aplicații, Shell, Utilitare            |
       |   - Acces strict la memorie VIRTUALĂ       |
       |   - Instrucțiuni critice interzise         |
       |         |                         ^        |
       |  syscall| (TRAP)                  | sysret |
       |         v                         |        |
       |   Ring 0: KERNEL MODE (Privilegiat)        |
       |   - Nucleul Linux                          |
       |   - Acces direct la RAM fizică și Hardware |
       |   - Toate instrucțiunile permise           |
       +--------------------------------------------+
```

| Criteriu | User Mode (Ring 3) | Kernel Mode (Ring 0) |
| :--- | :--- | :--- |
| **Nivel Privilegiu** | Neprivilegiat | Privilegiat (maxim) |
| **Spațiu de Adresare** | Spațiul propriu de memorie virtuală al procesului | Întreaga memorie fizică și spațiul nucleului |
| **Instrucțiuni CPU** | Instrucțiuni de calcul aritmetic, logic, salt etc. | Instrucțiuni directe I/O (`in`/`out`), dezactivare întreruperi (`cli`), reconfigurare MMU |
| **Consecința unei erori** | Procesul primește un semnal (ex: `SIGSEGV` - Crash izolat) | Blocarea întregului sistem (**Kernel Panic**) |

> [!IMPORTANT]
> Programele utilizator nu pot accesa direct hardware-ul (discul, placa de rețea, ecranul). Pentru a realiza orice operație de I/O, un proces utilizator trebuie să execute o instrucțiune specială CPU numită **TRAP** (`syscall` pe x86_64 sau `int 0x80` pe x86 pe 32 biți), transmițând controlul nucleului în mod controlat.

---

### 2.3. Sistemul de fișiere Linux: Arborele unic VFS

Spre deosebire de sistemele Windows (care folosesc litere de discuri separate precum `C:\`, `D:\`), sistemul de operare Linux folosește o **structură unică ierarhică arborescentă**, pornind de la un singur nod rădăcină notat cu `/` (**root**):

```text
                                  / (root)
                                  |
    +---------+---------+---------+---------+---------+---------+
    |         |         |         |         |         |         |
  /bin      /etc      /home     /dev      /usr      /tmp      /var
  (binare   (fisiere  (foldere  (fisiere  (binare   (fisiere  (date
   esentiale config    utilizatori dispozitiv) programe temporare variabile,
   ls, cp)   sistem)    ex: /alex)           secundare) curate la loguri)
                                                        reboot)
```

- **Filosofia UNIX:** *"Everything is a file"* (Totul este un fișier). Dispozitivele hardware, terminalele, chiar și procesele sunt reprezentate ca noduri în sistemul de fișiere (ex: `/dev/null`, `/dev/pts/0`, `/proc/cpuinfo`).
- **Nativ Case-Sensitive:** În Linux, fișierele `Tema.c`, `tema.c` și `TEMA.C` sunt **trei fișiere distincte** ce pot coexista în același director.

---

### 2.4. Căi în sistemul de fișiere: Absolute vs Relative

Orice resursă din sistem poate fi localizată folosind două tipuri de căi:

1. **Calea Absolută (Absolute Path):**
   - Începe **întotdeauna** cu caracterul `/` (rădăcina).
   - Identifică resursa în mod univoc, indiferent de directorul în care ne aflăm în momentul execuției.
   - *Exemplu:* `/home/student/laborator/prog.c`

2. **Calea Relativă (Relative Path):**
   - **Nu** începe cu `/`.
   - Este interpretată raportat la directorul de lucru curent (**Current Working Directory** - afișat de comanda `pwd`).
   - Convenții obligatorii de știut:
     - `.` (punct) = referință directă către **directorul curent** (hard link către sine).
     - `..` (două puncte) = referință către **directorul părinte** (directorul ierarhic superior).
     - `~` (tildă) = alias expandat de shell către directorul utilizatorului (`$HOME`).
   - *Exemplu:* dacă ne aflăm în `/home/student`, calea `laborator/prog.c` este relativă și echivalentă cu `/home/student/laborator/prog.c`. Calea `../student/laborator` urcă în părinte (`/home`) și coboară înapoi.

---

### 2.5. Terminale fizice (TTY) vs Pseudoterminale (PTS)

- **TTY (Teletypewriter / Virtual Console):** Terminale text autentice gestionate direct de nucleu (`/dev/tty1` până la `/dev/tty6`). Pot fi accesate în Linux nativ prin combinația de taste `Ctrl + Alt + F1..F6`.
- **PTS (Pseudo-Terminal Slave):** Emulatoare grafice de terminal (ex: GNOME Terminal, Alacritty, ferestre de `tmux`, sesiuni SSH). Aparate ca `/dev/pts/0`, `/dev/pts/1` etc. Un proces master (serverul X / Wayland / tmux) intermediază comunicarea între utilizator și shell.
- **Comenzi cheie:**
  - `whoami`: afișează utilizatorul curent logat în shell.
  - `tty`: afișează fișierul de dispozitiv al terminalului curent conectat la intrarea standard.
  - `pwd`: (*Print Working Directory*) afișează calea absolută a directorului curent.

---

### 2.6. Măști de căutare (Wildcards / Globbing)

Bash implementează mecanismul de **globbing** pentru filtrarea numelor de fișiere pe baza unor tipare:

| Metacaracter | Semnificație | Exemplu | Rezultat |
| :--- | :--- | :--- | :--- |
| `*` | Se potrivește cu **zero sau mai multe** caractere oricare | `*.c` | Toate fișierele cu extensia `.c` |
| `?` | Se potrivește cu **exact un caracter** oarecare | `tema?.txt` | `tema1.txt`, `temaA.txt` (nu și `tema10.txt`) |
| `[abc]` | Se potrivește cu **un singur caracter** din set | `lab[123].c` | `lab1.c`, `lab2.c`, `lab3.c` |
| `[a-z]` | Se potrivește cu orice caracter din interval | `fisiere[0-9].log` | Fișiere indexate cu o cifră |
| `[!abc]` sau `[^abc]` | Negație: orice caracter care **nu** este în set | `fisier[!0-9].txt` | Fișiere care nu au cifră pe acea poziție |

> [!WARNING]
> **Capcană majoră:** Mecanismul de globbing este realizat de **Shell înainte de execuția comenzii**!  
> Când tastați `rm *.o`, utilitarul `rm` nu vede caracterul `*`; shell-ul caută fișierele pe disc, le expandează (ex: `rm a.o b.o c.o`) și abia apoi apelează `rm`. Dacă nu există nicio potrivire, în mod implicit Bash transmite șirul literal `*.o`.

---

### 2.7. Variabile de mediu și căutarea binarilor ($PATH)

Variabilele de mediu sunt perechi `CHEIE=valoare` transmise proceselor copil la creare:

- `$HOME`: Calea absolută a directorului personal al utilizatorului (ex: `/home/alex`).
- `$USER`: Numele utilizatorului conectat.
- `$PS1`: Șirul de formatare pentru prompt-ul din terminal (ex: `PS1='[\u@\h \W]\$ '`).
- `$PATH`: Listă de directoare separate prin două puncte (`:`) în care nucleul/shell-ul caută fișierele executabile atunci când o comandă este apelată fără specificarea unei căi.

#### De ce comanda `program` eșuează cu `command not found`, dar `./program` funcționează?

Când tastați `ls`, shell-ul parcurge pe rând directoarele din `$PATH` (ex: `/usr/bin/ls`, `/bin/ls`) până găsește binarul.
Când tastați `program`, shell-ul caută `program` **doar în directoarele din `$PATH`**.

```text
   +--------------------------------------------------------------+
   | $PATH = /usr/local/bin : /usr/bin : /bin : /usr/games       |
   +--------------------------------------------------------------+
                                |
     Comanda apelata: "program" |
                                v
     1. Exista /usr/local/bin/program? -> NU
     2. Exista /usr/bin/program?       -> NU
     3. Exista /bin/program?           -> NU
     4. Exista /usr/games/program?     -> NU
                                |
                                v
                   EROARE: "command not found"
```

În Linux, **directorul curent `.` NU se află în mod implicit în `$PATH`**.
Pentru a rula un executabil aflat în directorul curent, trebuie să specificăm explicit calea către el: `./program` (adică *"rulează binarul `program` din directorul curent `.`"*).

> [!IMPORTANT]
> **De ce NU punem `.` în `$PATH`?**  
> Din motive critice de **securitate**! Dacă un atacator sau un coleg plasează un fișier malițios executabil numit `ls` într-un director partajat (ex: `/tmp`), iar utilizatorul are `.` la începutul variabilei `$PATH` și tastează `ls`, shell-ul va rula binarul atacatorului în locul comenzii legitime `/bin/ls`.

---

### 2.8. Compilatorul GCC și pipeline-ul de compilare C

Compilatorul `gcc` transformă fișierele sursă `.c` în executabile binare ELF trecând prin 4 etape fundamentale:

```text
 fisier.c
    |
    | [1. Preprocesor (cpp): expandeaza #include, #define, elimina comentarii]
    v
 fisier.i
    |
    | [2. Compilator (cc1): traduce C in cod de asamblare specific arhitecturii]
    v
 fisier.s
    |
    | [3. Asamblor (as): traduce asamblarea in cod masina binar (cod obiect)]
    v
 fisier.o
    |
    | [4. Link-editor (ld): leaga codul obiect cu libc si functiile de sistem]
    v
 executabil (ELF)
```

#### Flag-uri GCC recomandate la laborator și examen

Pentru a scrie cod de calitate industrială și a preveni bug-urile ascunse, vom folosi întotdeauna:

```bash
gcc -Wall -Wextra -std=c11 -pedantic -g -o program prog.c
```

- `-Wall`: Activează toate avertismentele comune (*Warnings All*) referitoare la variabile neinițializate, comparații dubioase etc.
- `-Wextra`: Activează avertismente suplimentare stricte (variabile nefolosite, comparații signed vs unsigned).
- `-std=c11`: Impune standardul modern ISO C11.
- `-pedantic`: Refuză extensiile non-standard ale compilatorului și semnalează abaterile de la standard.
- `-g`: Păstrează tabela de simboluri de depanare pentru depanatorul `gdb` și instrumentul `valgrind`.
- `-o <nume>`: Denumește fișierul binar executabil generat (dacă se omite, numele implicit este `a.out`).

După execuția oricărui program sau a oricărei comenzi, nucleul transmite un **cod de retur** pe 8 biți (0-255). Valoarea poate fi verificată imediat în shell cu:
```bash
echo $?
```
- Valoarea `0` înseamnă **succes** (`EXIT_SUCCESS`).
- O valoare nenulă (`1`, `2`, `127` etc.) semnalează o **eroare** (`EXIT_FAILURE`).

---

## 3. Tabel Sinoptic: Comenzi Esențiale CLI

| Comandă | Sintaxă & Flag-uri Uzuale | Descriere Detaliată | Exemplu Practic |
| :--- | :--- | :--- | :--- |
| `whoami` | `whoami` | Afișează numele utilizatorului curent | `whoami` |
| `tty` | `tty` | Afișează fișierul dispozitiv de terminal curent | `tty` |
| `pwd` | `pwd` | Afișează calea absolută a directorului curent de lucru | `pwd` |
| `cd` | `cd [cale]` | Schimbă directorul curent de lucru | `cd /usr/include`, `cd ..`, `cd ~`, `cd -` |
| `ls` | `ls [optiuni] [cale]` | Listează intrările dintr-un director | `ls -la`, `ls -lh /var/log` |
| | `-l` | Format detaliat (tip, drepturi, link-uri, user, grup, dimensiune, dată) | `ls -l` |
| | `-a` | Include fișierele ascunse (cele ce încep cu punct `.`) | `ls -a` |
| | `-d` | Listează directorul însuși, nu conținutul său | `ls -ld dir1` |
| | `-h` | Dimensiuni lizibile pentru om (*human-readable*: K, M, G) | `ls -lh` |
| `mkdir` | `mkdir [-p] nume_dir` | Creează unul sau mai multe directoare | `mkdir test` |
| | `-p` | Creează recursiv directoarele părinte dacă nu există deja | `mkdir -p a/b/c/d` |
| `rmdir` | `rmdir nume_dir` | Șterge directoare **exclusiv dacă sunt goale** | `rmdir test` |
| `rm` | `rm [optiuni] fisier` | Șterge fișiere sau arbori de directoare | `rm fisier.txt` |
| | `-r` / `-R` | Ștergere recursivă (obligatoriu pentru directoare cu conținut) | `rm -r folder_vechi/` |
| | `-f` | Forțat (ignoră fișierele inexistente și nu cere confirmare) | `rm -f temp.log` |
| | `-i` | Cere confirmare interactivă înainte de fiecare ștergere | `rm -i *.o` |
| `cp` | `cp [optiuni] sursa dest` | Copiază fișiere sau directoare | `cp prog.c prog_backup.c` |
| | `-r` / `-R` | Copiază recursiv întregul conținut al directorului | `cp -r src/ backup_src/` |
| `mv` | `mv sursa dest` | Mută sau redenumește un fișier/director | `mv vechi.c nou.c`, `mv *.o bin/` |
| `cat` | `cat [fisiere]` | Concatenează și afișează conținutul fișierelor la stdout | `cat date.in`, `cat f1.txt f2.txt` |
| `touch` | `touch fisier` | Creează un fișier gol dacă nu există sau îi actualizează timpii | `touch nou.txt` |
| `export` | `export NUME=valoare` | Exportă o variabilă în mediul proceselor copil | `export CC=gcc` |
| `echo` | `echo $VAR` | Afișează textul sau expansiunea variabilelor | `echo $PATH`, `echo $?` |
| `which` | `which binar` | Localizează comanda executabilă în `$PATH` | `which gcc`, `which ls` |

---

## 4. Configurarea Mediului Personal: Fișierul `~/.bashrc`

Fișierul `~/.bashrc` este executat automat de fiecare dată când este deschis un terminal interactiv nou non-login. Aici putem defini alias-uri utile, culori și putem extinde variabila `$PATH`.

În folderul `solutii/` se găsește scriptul `setup_bashrc.sh` ce adaugă automat configurări recomandate:

```bash
# 1. Alias-uri pentru protectie si navigare rapida
alias rm='rm -i'              # Previne stergerea accidentala
alias cp='cp -i'
alias mv='mv -i'
alias ll='ls -lah --color=auto'
alias ..='cd ..'
alias ...='cd ../..'

# 2. Alias pentru compilare rapida conform standardului USO
alias gcc-uso='gcc -Wall -Wextra -std=c11 -pedantic -g'

# 3. Prompt estetic cu culori si director curent
# [utilizator@host:director]$
export PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# 4. Extinderea PATH cu un folder personal de binare (~/bin)
export PATH="$HOME/bin:$PATH"
```

Pentru a aplica modificările imediat fără a reporni terminalul:
```bash
source ~/.bashrc
```

---

## 5. Structura Directorului & Ghid Practic de Rulare

Repository-ul acestui tutoriat conține următoarele directoare și fișiere:

```text
tutoriat-01/
├── README.md                      # Prezentul breviar teoretic și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── img/                           # Capturi reale de terminal (term-*.png) și CREDITE.md
├── schelet/                       # Codul C de lucru pentru studenti
│   ├── Makefile                   # Automatizarea compilarii
│   ├── exercitiu.c                # Scheletul C cu instructiuni /* TODO */
│   └── date.in                    # Fisier de intrare pentru teste
├── solutii/                       # Solutiile complete demonstrate la tabla
│   ├── Makefile                   # Makefile-ul complet
│   ├── exercitiu.c                # Programul C complet implementat si documentat
│   ├── date.in                    # Datele de intrare demonstrative
│   └── setup_bashrc.sh            # Script Bash de configurare personala a mediului
└── kahoot/                        # Quiz-ul saptamanal complet explicat
    ├── README.md                  # Intrebarile, variantele si explicatiile pe larg
    └── quiz.json                  # Format structurat pentru import rapid
```

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-01/schelet
   ```
2. **Inspectați fișierele prezente:**
   ```bash
   ls -la
   ```
3. **Deschideți fișierul `exercitiu.c`:**
   Completați secțiunile marcate cu `/* TODO: ... */`. Programul exersează:
   - Validarea argumentelor din linia de comandă (`argc`, `argv`).
   - Preluarea variabilelor de mediu din limbajul C cu funcția standard `getenv()`.
   - Deschiderea și citirea dintr-un fișier de date (`date.in`) transmis ca parametru.
   - Returnarea unui cod de succes sau eroare verificabil din consolă.
4. **Compilați folosind utilitarul `make`:**
   ```bash
   make
   ```
5. **Rulați programul:**
   ```bash
   ./exercitiu date.in
   echo $?
   ```
6. **Curățați fișierele binare generate:**
   ```bash
   make clean
   ```

---

## 6. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `bash: ./program: Permission denied`
- **Cauză:** Fișierul nu are setat bitul de execuție `x` în drepturile sale POSIX.
- **Remediere:** `chmod +x program`

### Capcana 2: `bash: program: command not found`
- **Cauză:** Ați uitat prefixul `./`. Directorul curent nu este în `$PATH`.
- **Remediere:** Executați `./program`

### Capcana 3: `rmdir: failed to remove 'dir1': Directory not empty`
- **Cauză:** Comanda `rmdir` este proiectată strict pentru directoare goale.
- **Remediere:** Ștergeți mai întâi conținutul sau folosiți comanda recursivă `rm -r dir1`.

### Capcana 4: Confuzia dintre `.`, `..` și `~`
- `.` = directorul curent.
- `..` = directorul părinte.
- `~` = directorul `$HOME` al utilizatorului.
- `/` = rădăcina absolută a întregului sistem.

### Capcana 5: Sensibilitatea la majuscule (Case Sensitivity)
- Sub Linux, `prog.c`, `Prog.c` și `PROG.C` sunt fișiere complet independente. Comenzile de sistem sunt scrise cu litere mici (`ls`, `cp`, `mv`).
