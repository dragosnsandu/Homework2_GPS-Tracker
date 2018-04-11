build: 
	gcc -g main.c -o gps -lm
clean:
	rm gps
checker:
	./checker.sh
run:
	./gps coordonate.in avarii.in Locatie2 coordonate.out rezultat.out