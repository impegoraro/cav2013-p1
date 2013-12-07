#!/bin/bash


for file in `find "$1" -iname "*.yuv"`
do 
	fname=`basename $file`
	dir=`dirname $file`
	dir=`basename $dir`


	# Append the header

	log="../output/logs/$fname-intra.log"
	echo "$file `ls -sh $file | cut -d' ' -f1`"  > "$log"
	echo "Y U V M Predictor Index Size Quant VideoEncodeTime FrameEncodeTime" >> "$log"

	for lin in `seq 0 6`
	do
		for i in 2 4 8 16
		do
			echo "encoding $file..."
			./src/utils/encodeDecode -s "$file" -d "../output/$fname-linear-$lin-m-$i-q-$2.gmb" -m "$i" -l "$lin" -q $2 > "/tmp/encodedecode.log"
			echo "comparing $file with $fname-linear-$lin-m-$i-q-$2.gmb..."
			./src/utils/yuvComp "$file" "../output/$fname-linear-$lin-m-$i-q-$2.gmb" >> "$log"
	
			encTime=`cat /tmp/encodedecode.log | grep Encode | cut -d' ' -f3 | tr -d "\n"`
			encTimeFrame=`cat /tmp/encodedecode.log | grep Encode | cut -d' ' -f6 | tr -d "\n"`
			echo "$i" "linear" "$lin" `ls -sh ../output/$fname-linear-$lin-m-$i-q-$2.gmb | cut -d' ' -f1` "$2" " $encTime" " $encTimeFrame" >> "$log"  
		done 
	done
	
	
	for i in 2 4 8 16
	do
		echo "encoding $file..."
		./src/utils/encodeDecode -s "$file" -d "../output/$fname-nonlinear-$lin-m-$i-q-$2.gmb" -m "$i" -n -q $2 > "/tmp/encodedecode.log"
		echo "comparing $file with $fname-linear-$lin-m-$i.gmb..."
		./src/utils/yuvComp "$file" "../output/$fname-linear-$lin-m-$i-q-$2.gmb" >> "$log"
		
		encTime=`cat /tmp/encodedecode.log | grep Encode | cut -d' ' -f3 | tr -d "\n"`
		encTimeFrame=`cat /tmp/encodedecode.log | grep Encode | cut -d' ' -f6 | tr -d "\n"`
		echo "$i" "nonlinear" "0" `ls -sh ../output/$fname-linear-$lin-m-$i-q-$2.gmb | cut -d' ' -f1` " 2" " $encTime" " $encTimeFrame"  >> "$log" 
	done 

done 