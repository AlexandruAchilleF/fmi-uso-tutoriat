#!/bin/bash
# ==============================================================================
# USO - Tutoriat 01: Script Demonstrativ de Configurare a Mediului (~/.bashrc)
# Facultatea de Matematica si Informatica, Universitatea din Bucuresti
# ==============================================================================
#
# Acest script configureaza un mediu productiv si sigur pentru laboratorul de USO:
# 1. Creeaza o copie de rezerva (backup) pentru fisierul ~/.bashrc existent
# 2. Creeaza directorul personal de binare ~/bin si il adauga in $PATH
# 3. Adauga alias-uri de siguranta (protectie la stergere accidentala)
# 4. Adauga alias-uri pentru compilare C conform standardului USO
# 5. Configureaza un prompt colorat si informativ (PS1)
# ==============================================================================

set -e

BASHRC_FILE="$HOME/.bashrc"
BACKUP_FILE="$HOME/.bashrc.backup.$(date +%Y%m%d_%H%M%S)"
BIN_DIR="$HOME/bin"

echo "=========================================================="
echo "    Configurare Mediu Personal Linux USO (FMI Bucuresti)  "
echo "=========================================================="

# Pasul 1: Backup fisier existent
if [ -f "$BASHRC_FILE" ]; then
    echo "[+] Cream copie de siguranta: $BACKUP_FILE"
    cp "$BASHRC_FILE" "$BACKUP_FILE"
else
    echo "[+] Fisierul $BASHRC_FILE nu exista. Va fi creat unul nou."
    touch "$BASHRC_FILE"
fi

# Pasul 2: Creare director ~/bin pentru binare personale
if [ ! -d "$BIN_DIR" ]; then
    echo "[+] Cream directorul pentru binare personale: $BIN_DIR"
    mkdir -p "$BIN_DIR"
fi

# Pasul 3: Adaugare configuratii in ~/.bashrc (daca nu au fost deja adaugate)
MARKER="# === CONFIGURATIE USO FMI BUCURESTI ==="

if grep -Fxq "$MARKER" "$BASHRC_FILE"; then
    echo "[i] Configuratia USO exista deja in $BASHRC_FILE. Nicio modificare necesara."
else
    echo "[+] Adaugam configuratiile recomandate in $BASHRC_FILE..."
    cat << 'EOF' >> "$BASHRC_FILE"

# === CONFIGURATIE USO FMI BUCURESTI ===

# 1. Alias-uri de siguranta impotriva stergerilor accidentale
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# 2. Alias-uri pentru navigare si listare eficienta
alias ll='ls -lah --color=auto'
alias la='ls -A --color=auto'
alias l='ls -CF --color=auto'
alias ..='cd ..'
alias ...='cd ../..'

# 3. Compilare C rapida cu flag-urile stricte de diagnostic cerute la USO
alias gcc-uso='gcc -Wall -Wextra -std=c11 -pedantic -g'

# 4. Prompt interactiv estetic cu utilizator, masina, director curent si culori ANSI:
# Verde: utilizator@masina | Albastru: director curent | Alb: caracterul '$'
export PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# 5. Adaugam directorul personal ~/bin in variabila PATH
if [ -d "$HOME/bin" ] && [[ ":$PATH:" != *":$HOME/bin:"* ]]; then
    export PATH="$HOME/bin:$PATH"
fi

# ======================================
EOF
    echo "[+] Configuratiile au fost adaugate cu succes!"
fi

echo ""
echo "Pentru a activa noile setari in sesiunea curenta de terminal, rulati:"
echo "    source ~/.bashrc"
echo "=========================================================="
