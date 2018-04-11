#!/bin/bash

#------------------------------------------------------------------------------
# 
#   CHECKER Tema 2 SDA-AB, 2017
#
#   Author: Iulian-Razvan Matesica <iulian.matesica@gmail.com>
#
#------------------------------------------------------------------------------

# Exec name
EXEC_NAME=gps

# Input data
IN_AVARII_FILE="in/avarii/avarii%d.in"
IN_COORDS_FILE="in/coordonate/coordonate%d.in"
IN_DEST_ARG="Locatie%d"

# Output data
OUT_COORDS_FILE="out/coord_finale/coordonate%d.out"
OUT_RESULT_FILE="out/result/result%d_%s.out"

# Reference data
REF_COORDS_FILE="ref/coord_finale/coordonate%d.out"
REF_RESULT_FILE="ref/result/result%d_%s.out"

NUM_TESTS=10
TIMEOUT=5

# Format: "x=a,y=b,z=c..."
# x, y, z are test indexes
# Each test with an index less or equal to x gets a points.
# Each test with an index less or equal to y gets b points.
POINTS_TEST="10=5"

# Total score
SCORE_TESTS=0

# Delimitor
DELIM="--------------------------------------------------------"

function show_total {
	echo $DELIM
	echo
	printf "NOTA FINALA ..................................... %3spt\n" $1
	echo	
}


function run_tests {
	nota=0
	for i in `seq 1 1 $NUM_TESTS`
	do	
		# Input files
		printf -v in_avarii $IN_AVARII_FILE $i
		printf -v in_coords $IN_COORDS_FILE $i
		# Input locations (destinations)
		in_dest_num=`head -1 $in_coords`
		
		# Output files
		printf -v out_coords $OUT_COORDS_FILE $i
		
		# Reference file
		printf -v ref_coords $REF_COORDS_FILE $i 
	
		test_coords_pass=1
		test_dests_pass=1

		for dest in `seq 2 1 $in_dest_num`
		do	
			# Generate destination argument
			printf -v in_dest_name $IN_DEST_ARG $dest
			# Result+destionation unique result file name
			printf -v out_result $OUT_RESULT_FILE $i $in_dest_name
			# Reference file
			printf -v ref_result $REF_RESULT_FILE $i $in_dest_name
	
			# Run Test
			timeout $TIMEOUT ./$EXEC_NAME $in_coords $in_avarii $in_dest_name $out_coords $out_result > /dev/null 2>&1 | cat
			
			# Compare coordinates
			diff -Z -q $out_coords $ref_coords > /dev/null 2>&1
			if [ $? -ne 0 ];
			then
				test_coords_pass=0
			fi
			
			# Compare results for current destination
			diff -Z -q $out_result $ref_result > /dev/null 2>&1
			if [ $? -ne 0 ];
			then
				test_dests_pass=0
			fi
		done
		
		if [ $test_coords_pass -eq 1 ]; then
			printf "Test %2d coordonate ...................... OK     ( +5pt)\n" $i
			nota=$((nota+5))
		else
			printf "Test %2d coordonate ...................... NOT OK ( +0pt)\n" $i
		fi

		if [ $test_dests_pass -eq 1 ]; then
			printf "        rezultat   ...................... OK     ( +5pt)\n" $i
			nota=$((nota+5))
		else
			printf "        rezultat   ...................... NOT OK ( +0pt)\n" $i
		fi
	done
	
	
	show_total $nota
}

# Prepare output directory
rm -rf out/
mkdir -p out/coord_finale
mkdir -p out/result/

# Build 
make clean &> /dev/null
make

# Run normal tests
echo $DELIM
echo "Executabil = '$EXEC_NAME'"
echo $DELIM

run_tests
