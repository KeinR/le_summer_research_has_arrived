#!/usr/bin/bash

name="$1"
graph="$2"

cd ~/documents/summer_research_2022/

python2 graceful-labeling/labeler.py "$graph" > "code/swapper/${name}.json"

cd code/swapper/


mm="${name}.json"

sed -i '$ d' "$mm"

lines=($(wc -l "$mm"))
lines=${lines[0]}
lines=$(($lines / 2 + 1))

sed -i "$lines,\$ d" "$mm"

sed -i 's/$/,/g' "$mm"
head --bytes -2 "$mm" > "${mm}.tmp"
echo -n "[" > "$mm"
cat "${mm}.tmp" >> "$mm"
echo -n "]" >> "$mm"
rm "${mm}.tmp"

python3 swap.py "$name" "$graph"

