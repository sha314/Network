
# Sometimes we want to run the program for multiple lengths. This bash script can make things easier. It will generate 
# a unique log file for each lengths and each time you run it.
En=2000
m=5
M=11


for N in 20000 150000 450000 1200000 2000000 3000000
do
	./networks $m $N $M $En >> log$m$N$M$En$(date +"%Y%m%d-%H%M%S") &
	echo $L
done

