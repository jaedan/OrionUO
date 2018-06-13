#!/usr/bin/env bash

readonly BASEDIR=$(readlink -f $(dirname $0))/..
cd $BASEDIR

# exit on errors
set -e

rc=0

if hash clang-format; then
	echo -n "Checking coding style..."
	rm -f format.log
	touch format.log
	git ls-files '*.[ch]' '*.cpp' '*.cc' '*.cxx' '*.hh' '*.hpp' ':!OrionUO/Resource.h' ':!*Dependencies/*' | \
		xargs -n8 -P8 clang-format -i -style=file >> format.log
	if [ -s format.log ]; then
		echo " errors detected"
		git diff
		echo "Incorrect code style detected in one or more files."
		echo "The files have been automatically formatted."
		echo "Remember to add the files to your commit."
		rc=1
	else
		echo " OK"
	fi
	rm -f format.log
else
	echo "You do not have clang-format installed so your code style is not being checked!"
fi

echo -n "Checking comment style..."
git grep --line-number -e '//---' -- '*.cpp' '*.h' > comment.log || true
if [ -s comment.log ]; then
	echo " Line break fillers detected."
	cat comment.log
	rc=1
else
	echo " OK"
fi
rm -f comment.log

echo -n "Checking for PVS-Studio comments..."
git grep --line-number -e '// PVS-Studio' -- '*.cpp' '*.h' > pvs.log || true
git grep --line-number -e '// This is an open source non-commercial' -- '*.cpp' '*.h' > pvs.log || true
if [ -s pvs.log ]; then
	echo " PVS Studio comments detected."
	cat pvs.log
	rc=1
else
	echo " OK"
fi
rm -f pvs.log

exit $rc
