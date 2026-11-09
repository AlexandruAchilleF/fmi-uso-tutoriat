# Tutoriatul 3: Shell Scripting în Bash (noțiunile de bază)

> **Disciplina:** Utilizarea Sistemelor de Operare (USO)  
> **Instituție:** Facultatea de Matematică și Informatică, Universitatea din București  
> **Timp estimat:** 90 minute (15 min Idei, 60 min Live Terminal, 10 min Quiz Kahoot, 5 min Concluzii)

---

## Cuprins

1. [Obiective de Învățare](#1-obiective-de-învățare)
2. [Breviar Teoretic](#2-breviar-teoretic)
   - [2.1. Ce este un script](#21-ce-este-un-script)
   - [2.2. Variabile](#22-variabile)
   - [2.3. Argumentele scriptului: `$1`, `$#`, `$@`, `$0`](#23-argumentele-scriptului-1---0)
   - [2.4. `$( )` și `$(( ))`](#24---și--)
   - [2.5. `if` și `test` (`[ ]`)](#25-if-și-test--)
   - [2.6. Bucla `for`](#26-bucla-for)
   - [2.7. Bucla `while read`](#27-bucla-while-read)
3. [Tabel Sinoptic](#3-tabel-sinoptic)
4. [Exercițiul C: media notelor, apelată dintr-un script](#4-exercițiul-c-media-notelor-apelată-dintr-un-script)
5. [Structura Directorului & Ghid Practic de Rulare](#5-structura-directorului--ghid-practic-de-rulare)
6. [Capcane Uzuale de Examen & Erori Frecvente](#6-capcane-uzuale-de-examen--erori-frecvente)

---

## 1. Obiective de Învățare

La finalul acestei ședințe, fiecare student va fi capabil:
- Să scrie și să ruleze un script Bash (`#!/bin/bash`, `bash script.sh`, `chmod +x`).
- Să folosească variabile și argumentele scriptului: `$1`, `$#`, `$@`, `$0`.
- Să pună rezultatul unei comenzi într-o variabilă cu `$( )` și să facă calcule cu `$(( ))`.
- Să ia decizii cu `if [ ... ]` și să citească un fișier linie cu linie cu `while read`.
- Să apeleze un program C dintr-un script și să folosească ce afișează (`stdout`) și codul lui de ieșire (`$?`).

---

## 2. Breviar Teoretic

### 2.1. Ce este un script

Un **script** este un fișier text cu comenzi, pe care shell-ul le execută una după alta, exact ca și cum le-ați scrie voi în terminal.

```bash
#!/bin/bash
# salut.sh: primul script
echo "Salut, $USER!"
echo "Azi este $(date +%A)"
```

| Cum îl rulezi | Ce trebuie |
| :--- | :--- |
| `bash salut.sh` | nimic în plus |
| `./salut.sh` | o dată `chmod +x salut.sh` (drept de execuție) și prima linie `#!/bin/bash` |

> [!NOTE]
> Prima linie, `#!/bin/bash`, se numește **shebang**. Îi spune sistemului cu ce program să ruleze fișierul atunci când îl porniți cu `./salut.sh`. Restul liniilor care încep cu `#` sunt comentarii.

---

### 2.2. Variabile

```bash
nume=ana              # atribuire: FĂRĂ spații în jurul lui =
echo $nume            # ana
echo "Salut, $nume"   # Salut, ana
echo 'Salut, $nume'   # Salut, $nume   (ghilimelele simple nu înlocuiesc nimic, ca în Tutoriatul 02)
```

> [!WARNING]
> `nume = ana` **nu** este o atribuire. Shell-ul înțelege „rulează comanda `nume` cu argumentele `=` și `ana`” și răspunde `nume: command not found`.

Toate variabilele sunt **text**. `x=5` pune în `x` caracterul `5`, nu numărul 5. Pentru calcule folosim `$(( ))`.

---

### 2.3. Argumentele scriptului: `$1`, `$#`, `$@`, `$0`

Când rulați `bash note.sh note.txt extra`, scriptul primește:

| Variabilă | Valoare | Echivalent în C |
| :--- | :--- | :--- |
| `$0` | `note.sh` (numele scriptului) | `argv[0]` |
| `$1`, `$2` | `note.txt`, `extra` | `argv[1]`, `argv[2]` |
| `$#` | `2` (câte argumente, fără `$0`) | `argc - 1` |
| `$@` | toate argumentele: `note.txt extra` | `argv[1] ... argv[argc-1]` |
| `$?` | codul de ieșire al **ultimei** comenzi | valoarea din `return` / `exit()` |

---

### 2.4. `$( )` și `$(( ))`

| Formă | Ce face | Exemplu | Rezultat |
| :--- | :--- | :--- | :--- |
| `$( comanda )` | rulează comanda și pune în loc **ce a afișat** | `n=$(wc -l < note.txt)` | `n` devine `6` |
| `$(( expresie ))` | calcul cu **numere întregi** | `echo $(( 7 * 3 ))` | `21` |
| | împărțirea taie zecimalele | `echo $(( 29 / 3 ))` | `9` (nu 9.67) |
| | restul împărțirii | `echo $(( 29 % 3 ))` | `2` |
| | în interior nu e nevoie de `$` | `i=$(( i + 1 ))` | crește `i` cu 1 |

> [!TIP]
> Bash nu știe numere cu virgulă. Dacă aveți nevoie de zecimale, lăsați calculul pe seama unui program (de exemplu, programul nostru C).

---

### 2.5. `if` și `test` (`[ ]`)

```bash
if [ $# -ne 1 ]; then
    echo "Utilizare: $0 <fisier>" >&2     # >&2 trimite mesajul pe stderr
    exit 1
fi
```

`[` este de fapt o **comandă** (alt nume pentru `test`). De aceea are nevoie de **spații** în jur: `[ $# -ne 1 ]`. Ca orice comandă, lasă un cod de ieșire: `0` = adevărat, `1` = fals. `if` rulează ramura `then` când codul este `0`.

| Test | Adevărat când | Exemplu |
| :--- | :--- | :--- |
| `-f f` | `f` există și e fișier obișnuit | `[ -f note.txt ]` |
| `-d d` | `d` există și e director | `[ -d schelet ]` |
| `-x f` | `f` poate fi executat | `[ -x ./exercitiu ]` |
| `-z "$s"` | textul `s` e gol | `[ -z "$1" ]` |
| `"$a" = "$b"` | texte egale (`!=` diferite) | `[ "$nume" = ana ]` |
| `-eq` `-ne` | numere egale / diferite | `[ $? -eq 0 ]` |
| `-lt` `-le` `-gt` `-ge` | `<` `<=` `>` `>=` pentru numere | `[ $nota -ge 5 ]` |
| `!` | negație | `[ ! -f "$1" ]` |

> [!IMPORTANT]
> În `[ ]`, `<` și `>` **nu** compară numere: sunt redirectări! `[ 3 > 2 ]` creează un fișier numit `2`. Pentru numere folosiți `-lt`, `-gt` etc.

Orice comandă poate sta după `if`, nu doar `[ ]`:

```bash
if grep -q root jurnal.txt; then echo "root a încercat să intre"; fi
```

---

### 2.6. Bucla `for`

`for` ia pe rând fiecare cuvânt dintr-o listă:

```bash
for nume in ana bogdan cristi; do
    echo "Salut, $nume"
done

for f in *.txt; do                    # shell-ul înlocuiește *.txt cu numele fișierelor
    echo "$f are $(wc -l < "$f") linii"
done

for i in 1 2 3 4 5; do echo $(( i * i )); done
```

---

### 2.7. Bucla `while read`

Pentru a citi un fișier **linie cu linie**:

```bash
while read -r nume n1 n2 n3; do
    echo "$nume: $(( (n1 + n2 + n3) / 3 ))"
done < note.txt
```

- `read` citește **o linie** și o împarte în cuvinte (după spații): primul cuvânt în `nume`, al doilea în `n1` etc. Dacă sunt mai multe cuvinte decât variabile, restul liniei ajunge în ultima variabilă.
- `< note.txt` se pune **după `done`**: tot ciclul citește din fișier.
- Când nu mai sunt linii, `read` întoarce cod nenul și bucla se oprește.
- `-r` oprește interpretarea lui `\` din text. Puneți-l mereu.

---

## 3. Tabel Sinoptic

| Sintaxă | Pe scurt | Exemplu |
| :--- | :--- | :--- |
| `#!/bin/bash` | prima linie: cu ce se rulează scriptul | |
| `x=valoare` | atribuire, fără spații | `nume=ana` |
| `$x`, `"$x"` | valoarea variabilei (între ghilimele duble: sigur) | `echo "$nume"` |
| `$1`, `$#`, `$@`, `$0` | argumentele scriptului | `[ $# -eq 1 ]` |
| `$?` | codul de ieșire al ultimei comenzi | `echo $?` |
| `$( cmd )` | ce a afișat comanda | `n=$(wc -l < f)` |
| `$(( expr ))` | calcul cu întregi | `i=$(( i + 1 ))` |
| `if [ ... ]; then ... else ... fi` | decizie | `if [ -f "$1" ]; then ...` |
| `for x in lista; do ... done` | buclă pe o listă | `for f in *.c; do ...` |
| `while read -r a b; do ... done < f` | buclă pe liniile unui fișier | |
| `exit n` | oprește scriptul cu codul `n` | `exit 1` |
| `>&2` | mesajul pe stderr | `echo "Eroare" >&2` |

---

## 4. Exercițiul C: media notelor, apelată dintr-un script

Programul `exercitiu.c` primește notele **ca argumente** și calculează media:

```text
Utilizare: ./exercitiu <nota1> [nota2 ...]
```

Ce trebuie să respecte, ca să poată fi folosit dintr-un script:

1. **Notele vin în `argv`.** `argv[1]` e prima notă, exact cum în script `$1` e primul argument. Funcția `citeste_nota()` (deja scrisă) transformă textul `"9"` în numărul `9` și respinge `"abc"` sau `"11"`.
2. **Doar media pe stdout**, cu 2 zecimale. Așa scriptul o poate prinde cu `media=$(./exercitiu 9 10 8)`.
3. **Erorile pe stderr** (fără note, notă invalidă), ca să nu ajungă în variabila scriptului.
4. **Codul de ieșire are sens:** `0` = promovat (media ≥ 5), `1` = restant, `2` = eroare. Scriptul îl citește cu `$?`.

```bash
$ ./exercitiu 9 10 8; echo "cod: $?"
9.00
cod: 0

$ ./exercitiu 4 5 3; echo "cod: $?"
4.00
cod: 1

$ ./exercitiu 7 abc; echo "cod: $?"
./exercitiu: nota invalida: 'abc' (accept numere intregi de la 1 la 10)
cod: 2
```

Pe lângă programul C, fișierul `note.sh` conține **3 exerciții scurte** de scripting, fiecare cu un indiciu. Ultimul folosește programul vostru: pentru fiecare student din `note.txt` îl apelează pe `./exercitiu` și decide, după `$?`, dacă a promovat.

---

## 5. Structura Directorului & Ghid Practic de Rulare

```text
tutoriat-03/
├── README.md                      # Prezentul breviar și ghid
├── slides.html                    # Slide-urile interactive (deschide în browser, navighezi cu ← →)
├── slides.pdf                     # Export static al slide-urilor, pentru printat
├── img/                           # Capturile de terminal folosite în slide-uri
├── schelet/                       # Codul de lucru pentru studenți
│   ├── Makefile                   # Compilare, teste, script, valgrind
│   ├── exercitiu.c                # Media notelor, cu instrucțiuni /* TODO */
│   ├── note.sh                    # 3 exerciții de scripting (de completat)
│   └── note.txt                   # Un student pe linie: nume nota1 nota2 nota3
├── solutii/                       # Soluțiile complete
│   ├── Makefile
│   ├── exercitiu.c
│   ├── note.sh                    # Toate cele 3 exerciții rezolvate și explicate
│   └── note.txt
└── kahoot/                        # Quiz-ul săptămânal (păstrat local, nu e public)
```

### Pași de urmat la laborator:

1. **Intrați în directorul scheletului:**
   ```bash
   cd tutoriat-03/schelet
   ```
2. **Exercițiile 1–2 din `note.sh`** (doar Bash). Rulați după fiecare:
   ```bash
   bash note.sh note.txt
   ```
3. **Deschideți `exercitiu.c`** și completați `/* TODO 1..5 */`.
4. **Compilați și testați:**
   ```bash
   make            # compilează cu -Wall -Wextra -std=c11 -pedantic -g
   make test       # 5 teste: fără note, promovat, restant, zecimale, notă invalidă
   ```
5. **Exercițiul 3 din `note.sh`**, care folosește programul vostru:
   ```bash
   make script     # echivalent cu: bash note.sh note.txt
   ```
   Rezultatul final trebuie să fie `Promovati: 4 din 6`.
6. **Bonus:** `make valgrind` (programul nu alocă memorie, deci raportul trebuie să fie curat). Valgrind îl facem pe larg în Tutoriatul 04.
7. **Curățați fișierele generate:** `make clean`.

---

## 6. Capcane Uzuale de Examen & Erori Frecvente

### Capcana 1: `nume = ana` → `nume: command not found`
- **Cauză:** cu spații, shell-ul crede că `nume` e o comandă.
- **Remediere:** `nume=ana`, lipit.

### Capcana 2: `[$# -ne 1]` → `[2: command not found`
- **Cauză:** `[` este o comandă și trebuie despărțită prin spații de argumente, inclusiv de `]`.
- **Remediere:** `[ $# -ne 1 ]`.

### Capcana 3: `[ $a > $b ]` „merge”, dar creează un fișier
- **Cauză:** în `[ ]`, `>` este redirectare.
- **Remediere:** `[ $a -gt $b ]` pentru numere.

### Capcana 4: `$(( 29 / 3 ))` dă `9`
- **Cauză:** Bash lucrează doar cu numere întregi.
- **Remediere:** calculul cu zecimale se face în alt program (C, `bc`, `awk`).

### Capcana 5: `[ -f $1 ]` greșește când numele are spații sau lipsește
- **Cauză:** fără ghilimele, `$1` gol dispare, iar un nume cu spații devine mai multe argumente.
- **Remediere:** `[ -f "$1" ]`. Variabilele între ghilimele duble, mereu.

### Capcana 6: `$?` nu mai e codul programului
- **Cauză:** `$?` se schimbă după **fiecare** comandă, inclusiv după `echo`.
- **Remediere:** testați-l imediat după comandă sau salvați-l: `cod=$?`.
