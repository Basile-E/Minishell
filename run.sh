#!/usr/bin/env bash

# Script pour lancer minishell sous Valgrind avec les bons flags

# Nom de l’exécutable (change si besoin)
BIN=./minishell

# Fichier de suppressions (readline, etc.)
SUPP=./readline.supp

if [ ! -x "$BIN" ]; then
    echo "❌ $BIN introuvable ou non exécutable. Compile d'abord : make"
    exit 1
fi

if [ ! -f "$SUPP" ]; then
    echo "⚠️  Fichier de suppressions $SUPP introuvable, je continue sans."
    SUPP=""
else
    SUPP="--suppressions=$SUPP"
fi

valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --track-fds=yes \
    --trace-children=yes \
    --quiet \
    $SUPP \
    --log-file=valgrind.log \
    "$BIN"

