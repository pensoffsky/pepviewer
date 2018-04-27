#!/bin/bash

cd ..
tar -cf pepviewer09.tar pepviewer09
gzip pepviewer09.tar

echo Backupfile: pepviewer09-`date +%Y.%m.%d-%Hh%Mm`-$1.tar.gz
mv pepviewer09.tar.gz safepepviewer09/pepviewer09-`date +%Y.%m.%d-%Hh%Mm`-$1.tar.gz
echo Backup fertig!
