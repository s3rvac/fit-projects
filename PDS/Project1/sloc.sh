#!/bin/bash

echo -n 'With comments && with empty lines:       '
cat *.cc *.h | wc -l

echo -n 'With comments && without empty lines:    '
egrep -v "^[[:space:]]*$" *.cc *.h | wc -l

echo -n 'Without comments && without empty lines: '
cat *.cc *.h | grep -v "//" | grep -v "^$" | egrep -v "/\*" | egrep -v "^[[:space:]]*\*" | wc -l
