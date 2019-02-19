
rm *.aml 2> /dev/null
rm *.aml 2> /dev/null

cp ../TestSuite/*.aml .

for filename in *.aml; do
	echo "Test file $filename"
	if ./Decompiler $filename; then
		filename2="$(basename $filename .aml)"
		if ! iasl   "$filename2.dsl" &> /dev/null ; then
			echo "iasl error for file $filename"
		fi
	else
    	echo "Decompiler error for file $filename" 
	fi
done