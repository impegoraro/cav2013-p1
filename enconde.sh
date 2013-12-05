#!/bin/bash

file=$1
output="$2normal-golomb"
echo "$file" `ls -sh "$file"`  > results.log
echo "y u v m predictor index size quant" >> results.log
for lin in `seq 0 6`
do
	for i in 2 4 8 16
	do
		./src/utils/encodeDecode -s "$file" -d "../tmp/$output-linear-$lin-m-$i.gmb" -m "$i" -l "$lin" -q 2
		./src/utils/yuvComp "$file" "../tmp/$output-linear-$lin-m-$i.gmb" >> results.log
		echo "$i" "linear" "$lin" `ls -sh "../tmp/$output-linear-$lin-m-$i.gmb"` "2" >> results.log  
	done 
done


for i in 2 4 8 16
do
	./src/utils/encodeDecode -s "$file" -d "../tmp/$output-nonlinear-$lin-m-$i.gmb" -m "$i" -n -q 2
	./src/utils/yuvComp "$file" "../tmp/$output-linear-$lin-m-$i.gmb" >> results.log
		echo "$i" "nonlinear" "0" `ls -sh "../tmp/$output-linear-$lin-m-$i.gmb"` "2"  >> results.log 
done 
