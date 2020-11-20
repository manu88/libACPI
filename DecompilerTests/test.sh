
rm *.aml 2> /dev/null
rm *.dsl 2> /dev/null
cp ../TestSuite/*.aml .

for filename in *.aml; do
	echo "Test file $filename"
	if ./decompiler $filename; then
		filename2="$(basename $filename .aml)"

		iasl "$filename2.dsl" &> /dev/null
		retVal=$?
		if [ $retVal -ne 0 ]; then
			echo "iasl error for file $filename $retVal"
			exit retVal
		fi
	else
    	echo "Decompiler error for file $filename" 
	fi
done
