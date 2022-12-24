# Specify the target file and the object files that it depends on
main: main.o lex.o parserInt.o val.o
	# Compile the target file and link the object files
	g++ main.o lex.o parserInt.o val.o -o main

# Compile the main.cpp file and generate the main.o object file
main.o: main.cpp
	g++ -c main.cpp

# Compile the lex.cpp file and generate the lex.o object file
lex.o: lex.cpp
	g++ -c lex.cpp

# Compile the parserInt.cpp file and generate the parserInt.o object file
parserInt.o: parserInt.cpp
	g++ -c parserInt.cpp

# Compile the val.cpp file and generate the val.o object file
val.o: val.cpp
	g++ -c val.cpp

# Clean up the object files and the target file
clean:
	rm -f *.o main

# Run the target file, passing in the filename as the input
run: main
	./main $(filename)