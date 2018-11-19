#!/bin/bash
echo "Running 'dot_unflatten.sh' in '$PWD'."
rm -vf ../tests/*.flat.dot
for dot in $(find ../tests -name '*.dot'); do
  echo "Calling 'unflatten' on '$dot'."
  unflatten -l 6 -f $dot > ${dot%.*}.flat.dot
done
