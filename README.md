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
