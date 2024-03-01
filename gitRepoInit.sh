#! /bin/bash

[ $# -ne 1 ] && echo "$0 needs a repo name and only that" && exit 1

echo "# $1" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/TamerABK/$1.git
git push -u origin main
