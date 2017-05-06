#!/bin/bash

i="1"

while [ $i -lt 1000000 ]
do
echo -en "$i\t"; ./test -x 1 -y 2 -m $i -p 105943 -a -7 -b 10
i=$[$i + 1]
done
