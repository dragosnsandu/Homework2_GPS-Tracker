<b>IMPORTANT: I don't have the copyrights over the <i>checker.sh</i> file and all of the test from <i>in</i> and <i>ref</i> folders or avarii.in file</b>, I just uploaded them in order to make a potentially check of the code easier.

# Homework2_GPS-Tracker
Find the shortest distance between starting point and ending point by using graphs concepts. More information about what this homework is about here: http://elf.cs.pub.ro/sda-ab/wiki/teme/tema2 

To find the shortest path, the following operations are executed:
1. Based on GPS coordinates it's calculated the exact position of every location
2. The cost between each location is calculated and stored in a matrix called 'cost'.
3. The 'cost' matrix is updated with the COST_MAX value if the path (cost) between two locations is broken.
4. Floyd-Marshall on 'cost' matrix in order to determine the lower cost path. 'cost' matrix is overwritten.

# How to run the GPS Tracker
<code>make build</code> - this will compile the main.c file and create 'gps' file.<br>
<code>make checker</code> 
It should run all the tests and return a perfect score - 100 points.
