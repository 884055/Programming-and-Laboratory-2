all:
	g++ Lezione9_Calculator/memcheck.cpp -o main_executable

debug_executable: Lezione21_Template/list.cpp
	g++ Lezione21_Template/list.cpp -g -o debug_executable

exercise: Esercitazione_temp/esercitazione.cpp
	g++ Esercitazione_temp/esercitazione.cpp -g -o debug_executable

memcheck: debug_executable
	valgrind --track-origins=yes --leak-check=full ./debug_executable

cachemiss: debug_executable 
	valgrind --tool=callgrind --cache-sim=yes ./debug_executable

clean:
	rm main_executable debug_executable
