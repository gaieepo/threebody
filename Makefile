all:
	clang++ -F /Library/Frameworks -framework sfml-window -framework sfml-graphics -framework sfml-system main.cpp -o main

clean:
	rm main
