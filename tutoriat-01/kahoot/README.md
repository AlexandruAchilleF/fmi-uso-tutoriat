# Quiz Kahoot: Tutoriat 01

> **Tematică:** Concepte Fundamentale de SO & Linia de Comandă Linux (CLI)  
> **Durată recomandată:** 10 minute (5 întrebări, câte 2 minute alocate per întrebare + explicație la tablă)  
> **Format:** 4 variante de răspuns (A, B, C, D), dintre care o singură variantă este corectă.

---

## Întrebarea 1

**De ce comanda `program` dă eroare (`bash: program: command not found`), deși fișierul executabil există în directorul curent, dar comanda `./program` funcționează corect?**

- [ ] **A)** Executabilul nu are permisiuni de citire (`r`), iar prefixul `./` forțează citirea binarului.
- [x] **B)** În Linux, directorul curent (`.`) nu se află în mod implicit în variabila `$PATH` din motive de securitate; apelul fără cale caută binarul exclusiv în directoarele din `$PATH`. *(Răspuns Corect)*
- [ ] **C)** În Linux, orice fișier compilat cu `gcc` trebuie să fie mutat obligatoriu în `/bin` înainte de a putea fi apelat după nume.
- [ ] **D)** Prefixul `./` instruiește nucleul să execute programul în Kernel Mode, pe când apelul simplu rulează în User Mode.

### Explicație Detaliată:
- **De ce B este corect:** Atunci când o comandă este tastată fără specificarea unei căi (fără caractere `/`), interpretorul de comenzi (Bash) caută binarul parcurgând secvențial lista de directoare specificate în variabila de mediu `$PATH` (ex: `/usr/local/bin:/usr/bin:/bin`). Directorul curent (`.`) este omis în mod deliberat din `$PATH` pentru a preveni atacurile de tip *trojan horse* (plasarea unui binar rău-intenționat numit `ls` sau `cd` într-un director partajat precum `/tmp`). Folosind `./program`, furnizăm o cale relativă explicită, indicând nucleului exact locația fișierului.
- **De ce A este greșit:** Execuția necesită dreptul `x` (executare), nu doar `r`. Prefixul `./` nu ocolește sistemul de permisiuni POSIX.
- **De ce C este greșit:** Programele pot fi rulate din orice director dacă li se precizează calea absolută sau relativă.
- **De ce D este greșit:** Niciun program de nivel utilizator nu poate trece în Kernel Mode doar prin sintaxa de apelare; trecerea are loc exclusiv prin instrucțiunea hardware `TRAP` (`syscall`).

---

## Întrebarea 2

**Ce reprezintă intrările ascunse `.` și `..` afișate de comanda `ls -la` în orice director din Linux?**

- [ ] **A)** Fișiere temporare de swap create de nucleu pentru salvarea sesiunii curente de terminal.
- [ ] **B)** Fișiere ascunse de configurare ce rețin permisiunile utilizatorilor și măștile ACL.
- [x] **C)** `.` este o referință fizică (hard link) către directorul curent, iar `..` este o referință fizică (hard link) către directorul părinte. *(Răspuns Corect)*
- [ ] **D)** Două legături simbolice (soft links) create automat de interpretorul Bash la pornire.

### Explicație Detaliată:
- **De ce C este corect:** În sistemul de fișiere UNIX/Linux (EXT2/3/4, VFS), fiecare director este un fișier special ce conține un tabel de asocieri între nume de fișiere și numere de i-noduri. La crearea oricărui director nou (`mkdir`), sistemul creează automat două intrări obligatorii:
  1. `.` (un punct) asociat numărului de i-nod al directorului curent (un hard link către sine);
  2. `..` (două puncte) asociat numărului de i-nod al directorului ierarhic superior (părintele).
  *(Observație avansată: în directorul rădăcină `/`, ambele intrări `.` și `..` pointează către același i-nod 2).*
- **De ce A și B sunt greșite:** Nu sunt fișiere temporare și nici fișiere de permisiuni; sunt intrări reale de structură arborescentă.
- **De ce D este greșit:** Nu sunt legături simbolice (symlinks), ci legături fizice (hard links) gestionate la nivel de sistem de fișiere de nucleu, complet independente de Bash.

---

## Întrebarea 3

**Care este diferența majoră dintre instrucțiunile rulate în Kernel Mode și cele rulate în User Mode?**

- [ ] **A)** În Kernel Mode codul rulează de două ori mai rapid deoarece nucleul dezactivează ceasul intern al procesorului.
- [x] **B)** În Kernel Mode procesorul execută instrucțiuni privilegiate și are acces direct la toate resursele hardware și memoria fizică, pe când în User Mode accesul este restricționat strict la spațiul propriu de adresare virtuală. *(Răspuns Corect)*
- [ ] **C)** În User Mode instrucțiunile sunt compilate Just-In-Time (JIT), în timp ce în Kernel Mode se folosește un interpretor de bytecode.
- [ ] **D)** Nu există nicio diferență la nivel de procesor; delimitarea este realizată pur software prin biblioteca `glibc`.

### Explicație Detaliată:
- **De ce B este corect:** Delimitarea este implementată hardware prin inelele de protecție ale procesorului (ex: Ring 0 pentru Kernel Mode și Ring 3 pentru User Mode). În Ring 0, procesorul poate executa instrucțiuni privileged (cum ar fi manipularea tabelei de paginare CR3, dezactivarea întreruperilor `cli`, citirea/scrierea directă a porturilor I/O). În Ring 3, orice tentativă de a rula o instrucțiune privilegiată generează o excepție hardware (*General Protection Fault*), prevenind compromiterea sau blocarea sistemului de către o aplicație cu erori.
- **De ce A este greșit:** Viteza de ceas a procesorului este aceeași; diferă doar drepturile de acces și setul de instrucțiuni permise.
- **De ce C este greșit:** Ambele moduri execută cod mașină binar nativ.
- **De ce D este greșit:** Protecția este una pur hardware, garantată de circuitele CPU și MMU (Memory Management Unit).

---

## Întrebarea 4

**Dacă executăm comanda `rmdir dir1`, iar comanda returnează eroarea `rmdir: failed to remove 'dir1': Directory not empty`, care este cea mai probabilă cauză și soluția adecvată?**

- [ ] **A)** Utilizatorul nu are drepturi de administrator (`root`); soluția este rularea cu `sudo rmdir dir1`.
- [ ] **B)** Fișierele din interior sunt blocate de antivirus; soluția este repornirea sistemului.
- [x] **C)** Directorul `dir1` nu este gol (conține alte fișiere sau subdirectoare); comanda `rmdir` șterge doar directoare complet goale, iar pentru ștergerea recursivă se folosește `rm -r dir1`. *(Răspuns Corect)*
- [ ] **D)** Fișierul `dir1` este de fapt un fișier obișnuit redenumit; trebuie folosită comanda `unlink dir1`.

### Explicație Detaliată:
- **De ce C este corect:** Comanda de sistem `rmdir` (care apelează apelul de sistem `rmdir(2)`) este proiectată ca o măsură de siguranță: ea refuză categoric să șteargă un director dacă acesta conține alte intrări în afară de cele două implicite (`.` și `..`). Dacă directorul conține fișiere (chiar și ascunse!) sau alte subdirectoare, este necesară comanda `rm -r dir1` (*recursive remove*), care parcurge arborele în post-ordine și șterge recursiv fiecare element.
- **De ce A este greșit:** Nici măcar `root` nu poate șterge un director plin apelând direct `rmdir`, deoarece apelul de sistem `rmdir` din nucleu returnează `ENOTEMPTY` (`EEXIST`).
- **De ce B și D sunt greșite:** Nu au legătură cu mecanismul VFS din Linux.

---

## Întrebarea 5

**Cum tratează sistemul de operare Linux fișierele `Tema.c` și `tema.c` aflate în același director?**

- [ ] **A)** Ca pe un singur fișier; al doilea fișier creat îl va suprascrie automat pe primul.
- [ ] **B)** Linux afișează o eroare de coliziune la salvarea celui de-al doilea fișier (`File name collision`).
- [x] **C)** Ca pe două fișiere distincte, complet independente, fiecare având propriul său i-nod și propriile blocuri de date, deoarece sistemul de fișiere Linux este nativ *case-sensitive*. *(Răspuns Corect)*
- [ ] **D)** Ca pe două legături fizice (hard links) care pointează întotdeauna către același conținut.

### Explicație Detaliată:
- **De ce C este corect:** Sistemele de fișiere Linux native (EXT4, XFS, Btrfs) sunt strict *case-sensitive* (sensibile la diferența dintre majuscule și minuscule). Pentru nucleul Linux, codurile ASCII ale caracterelor `'T'` (0x54) și `'t'` (0x74) sunt diferite, astfel încât string-urile `"Tema.c"` și `"tema.c"` produc hash-uri diferite în tabela de intrări a directorului și sunt asociate la două numere de i-nod complet distincte. *(Spre deosebire de Windows NTFS/FAT32 unde sistemul este case-insensitive).*
- **De ce A și B sunt greșite:** Aceasta este o confuzie frecventă a studenților veniți din mediul Windows, unde cele două fișiere ar intra în conflict.
- **De ce D este greșit:** Legăturile fizice se creează explicit prin apelul `link()` / comanda `ln`, nu automat pe baza similarității de nume.
