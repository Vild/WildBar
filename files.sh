#!/bin/bash

CFILES=`find src/ -iname \*.c -exec printf "%s " "{}" \;`
OBJECTFILES=""
for file in `find src/ -iname \*.c`; do
	OBJECTFILES="$OBJECTFILES obj/`basename $file`.o"
done

for file in `find src/ -iname \*.c`; do
	printf "build %s: cc %s \n" "obj/`basename $file`.o" "$file"
done
echo ""
echo "build bin/${TARGET}: link ${OBJECTFILES}"
echo "default bin/${TARGET}"
