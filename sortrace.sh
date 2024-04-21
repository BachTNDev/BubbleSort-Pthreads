# this file is called sortrace.sh
# it must have execute privilege set to run
# run it as a background task like this:
# $ rm sortrace.log # start with fresh log file
# $ sortrace.sh >> sortrace.log & # this may take an hour

c++ generate.cpp -o generate
c++ mysort.cpp -o mysort -pthread

echo Generating 1000000 random numbers
sleep 1
./generate 1000000 -100000 100000 # you have to write generate.cpp
sleep 1
echo Starting system sort
sleep 1
{ time sort -n numbers.dat > systemsort.out; } 2>> sortrace.log
sleep 1
echo Starting mysort
echo Starting Array bubble sort with 1000000
sleep 1
echo Ending bubble sort
{ time ./mysort numbers.dat mysort.out; } 2>> sortrace.log # you have to write mysort.cpp
sleep 1
sort -c -n mysort.out 2>> sortrace.log # verify file is sorted