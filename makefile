build: 
	gcc -pthread mat_mul.cpp -o mat_mul -lstdc++

clean:
	rm -f thread
