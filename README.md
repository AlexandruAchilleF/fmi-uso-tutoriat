# Tutoriat: Utilizarea Sistemelor de Operare (USO)

Repository-ul oficial cu materialele practice, scheletele de laborator, soluțiile complete și slide-urile introductive pentru tutoriatul de **Utilizarea Sistemelor de Operare (USO)**, susținut la **Facultatea de Matematică și Informatică, Universitatea din București**.

Laboratorul este orientat pe **mediul Linux/UNIX**, programare de sistem în limbajul **C (standard POSIX)** și automatizare prin **Bash scripting**.

---

## Despre Tutoriat

Scopul acestor sesiuni este consolidarea cunoștințelor fundamentale de sisteme de operare și pregătirea intensivă pentru laborator și examenul teoretic.

---

## Structura Repository-ului

Fiecare sesiune de tutoriat are un director dedicat, cu o structură predictibilă și curată:

```text
fmi-uso-tutoriat/
├── README.md                          # Documentul principal de prezentare
├── .gitignore                         # Exclude binarele compilate, fișierele .o și log-urile
│
├── tutoriat-01/
│   ├── README.md                      # Breviar teoretic, comenzi sintetizate și ghid de rulare
│   ├── slides.pdf                     # Diagramele conceptuale prezentate la începutul ședinței
│   ├── schelet/                       # Cod sursă cu marcaje /* TODO */ și date de test
│   │   ├── Makefile                   # Automatizarea compilării
│   │   ├── exercitiu.c
│   │   └── date.in
│   ├── solutii/                       # Rezolvările complete explicate și testate
│   │   ├── Makefile
│   │   └── exercitiu.c
│   └── kahoot/                        # Întrebările din quiz, variantele de răspuns și explicații
│
├── tutoriat-02/
│   └── ...
└── tutoriat-10/
```
---

## Planul Celor 10 Tutoriate

| Sesiune | Tematica Abordata | Concepte si Apeluri Cheie |
| :--- | :--- | :--- |
| **[Tutoriat 01](tutoriat-01/)** | Arhitectura Linux, Kernel/User Mode, CLI de Baza | `cd`, `ls -la`, `mkdir`, `cp`, `mv`, `$PATH`, `gcc -Wall` |
| **Tutoriat 02** | Utilitare Text, Filtre, Despecializare si Redirectari | `grep`, `sort`, `uniq`, `wc`, pipe `\|`, redirectari `<`, `>`, `2>`|
| **Tutoriat 03** | Shell Scripting in Bash: Conditii, Bucle si Automatizari | `if [ ... ]`, `test`, `for`, `while read`, `$1..$n`, `$(( ))`, `$( )`|
| **Tutoriat 04** | Interfata C: Apeluri Sistem vs Biblioteca Standard C | TRAP, `errno`, `perror()`, `open()`, `read()`, `write()`, `FILE*` |
| **Tutoriat 05** | Sistemul de Fisiere EXT: I-noduri, Links si Directoare in C | `lseek()`, `dup2()`, `link()`, `symlink()`, `opendir()`, `readdir()`|
| **Tutoriat 06** | Metadate Fisiere, Drepturi rwx, Biti Speciali si Useri | `stat()`, `chmod()`, SUID, SGID, Sticky bit, `getpwuid()`, `umask()` |
| **Tutoriat 07** | Procese I: Memorie Virtuala, `fork()`, `wait()` si COW | `fork()`, Copy-on-Write, starile proceselor, Zombie, Orfani, `waitpid()`|
| **Tutoriat 08** | Procese II: Familia `exec`, Mediu si Construire Mini-Shell | `execvp()`, `environ`, `getcwd()`, comenzi interne vs externe|
| **Tutoriat 09** | Comunicare Inter-Proces: Semnale POSIX si Tuburi | `sigaction()`, `kill()`, `pipe()`, capete nefolosite, `mkfifo()`|
| **Tutoriat 10** | Memorie Partajata (`mmap`), Recapitulare si Simulare Colocviu | `mmap()`, `msync()`, `munmap()`, exercitii de sinteza|

---

## Ghid Rapid de Utilizare pentru Studenti

### 1. Clonarea repository-ului
Cloneaza repository-ul local pe calculatorul tau sau direct in instanta de Linux:
```bash
git clone [https://github.com/AlexandruAchilleF/fmi-uso-tutoriat.git](https://github.com/AlexandruAchilleF/fmi-uso-tutoriat.git)
cd fmi-uso-tutoriat
```

### 2. Cum lucrezi la o sedinta de tutoriat
In timpul tutoriatului, intra direct in folderul `schelet` al laboratorului curent:
```bash
cd tutoriat-05/schelet
```
* In folder vei gasi fisiere `.c` gata pregatite, unde bibliotecile necesare si verificarile initiale de argumente (`argc`/`argv`) sunt deja scrise.
* Cauta sectiunile marcate cu comentarii de tipul `/* TODO: ... */` si completeaza logica folosind apelurile de sistem discutate.
* Pentru a compila rapid fara comenzi lungi de `gcc`, foloseste fisierul `Makefile` inclus:
  ```bash
  make        # compileaza fisierele cu flag-urile -Wall -Wextra
  ./program   # ruleaza binarul rezultat
  make clean  # sterge fisierele binare temporare
  ```

### 3. Verificarea solutiei
Daca te-ai blocat sau vrei sa compari implementarea ta cu varianta demonstrata la videoproiector, consulta fisierele din folderul `solutii/`:
```bash
cd ../solutii
cat exercitiu.c
```

---

## Cerinte Software si Mediu de Dezvoltare

Toate programele si comenzile sunt dezvoltate pentru mediul Linux (distributii bazate pe Debian/Ubuntu sau WSL2).

Pachetele necesare pentru intregul semestru se pot instala ruland comanda:
```bash
sudo apt update
sudo apt install -y build-essential gcc gdb make valgrind tmux manpages-dev manpages-posix-dev
```

* **build-essential, gcc, make**: compilatorul C si utilitarele de asamblare si legare.
* **manpages-dev, manpages-posix-dev**: documentatia completa offline a apelurilor sistem si functiilor de biblioteca (accesibile prin `man 2 <apel>` sau `man 3 <functie>`).
* **valgrind**: depanarea scurgerilor de memorie si a descriptorilor neinscrisi.
* **tmux**: utilitar pentru impartirea ecranului de terminal intre editor si consola.

---

## Bibliografie si Resurse Recomandate

1. **Suportul de curs USO** — Dragulici Dumitru Daniel, Facultatea de Matematica si Informatica, Universitatea din Bucuresti.
2. **The Linux Programming Interface (TLPI)** — Michael Kerrisk, No Starch Press.
3. **Manualele Linux (Man Pages):**
   * Sectiunea 1: Utilitare si comenzi shell (`man 1 ls`, `man 1 grep`)
   * Sectiunea 2: Apeluri sistem oferite de nucleu (`man 2 open`, `man 2 fork`, `man 2 pipe`)
   * Sectiunea 3: Functii din biblioteca standard C (`man 3 malloc`, `man 3 printf`)

---

## Reguli de Contributie si Feedback

* In cazul in care identifici erori de redactare, probleme de compilare in schelete sau doresti sa propui o implementare alternativa, deschide un **Pull Request** sau semnaleaza situatia in sectiunea de **Issues**.
* Recomandarea este sa rulezi `git pull` inainte de fiecare sedinta de tutoriat pentru a prelua la zi materialele adaugate.

---

## Tutoriat Utilizarea Sistemelor de Operare 2025

*Mai multe materiale pot fi gasite in repo-ul https://github.com/VladCiobotenco/USO-Tutoring .
