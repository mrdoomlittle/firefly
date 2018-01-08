#!/bin/sh
if ! [ -d ~/.vim ]; then
    mkdir ~/.vim
fi

if ! [ -d ~/.vim/syntax ]; then
    mkdir ~/.vim/syntax
fi

cp .vim/syntax/* ~/.vim/syntax
