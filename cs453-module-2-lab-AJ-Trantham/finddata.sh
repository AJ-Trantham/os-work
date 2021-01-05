cd /proc/1
for f in `/bin/ls`
do
	echo "Reading file $f"
	cat $f
	printf "\nPress Enter for next file\n"
	read
done
