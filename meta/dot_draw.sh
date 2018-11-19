#!/bin/sh
echo "Running 'dot_draw.sh' in '$PWD'"
rm -vf ../tests/*.flat.dot.png
for dot in $(find ../tests -name '*.flat.dot'); do
  echo "Calling 'dot' on '$dot'."
  dot -Tpng $dot -o ${dot%.*}.png
done
