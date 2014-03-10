#!/bin/bash

t1=`./x <<< "2 0 0 0 0"`
if [ "$t1" = "0 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 0 1 0 0"`
if [ "$t1" = "0 1" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 0 1 0 1"`
if [ "$t1" = "1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 1 0 0 0"`
if [ "$t1" = "1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 1 0 1 0"`
if [ "$t1" = "(1) 0 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 1 1 0 0"`
if [ "$t1" = "1 1" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 1 1 0 1"`
if [ "$t1" = "(1) 0 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "1 0 0"`
if [ "$t1" = "0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "1 1 1"`
if [ "$t1" = "(1) 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "2 1 1 1 1"`
if [ "$t1" = "(1) 1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "3 1 1 1 1 1 1"`
if [ "$t1" = "(1) 1 1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "9 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1"`
if [ "$t1" = "(1) 1 1 1 1 1 1 1 1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "9 1 1 0 1 0 1 1 0 0 0 1 0 0 0 1 0 1 0"`
if [ "$t1" = "(1) 0 0 0 1 1 0 1 1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi

t1=`./x <<< "20 0 1 1 0 0 1 0 1 1 1 1 0 0 1 0 0 0 0 0 0 1 1 0 0 0 0 1 1 0 1 0 1 1 1 1 1 0 1 1 0"`
if [ "$t1" = "(1) 0 0 1 0 1 0 0 1 0 1 0 0 0 0 1 1 0 1 1 0" ]; then
	echo "OK"
else
	echo "FAIL"
fi
