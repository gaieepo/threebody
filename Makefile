all:
	# clang++ -F /Library/Frameworks -c main.cpp
	clang++ -framework sfml-window -framework sfml-graphics -framework sfml-system main.cpp -o main
