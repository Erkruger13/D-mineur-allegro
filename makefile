# Nom de l'exécutable
TARGET = run

# Compilateur
CXX = g++

# Flags de compilation
CXXFLAGS = -Wall -Wextra -std=c++11

# Bibliothèques Allegro
LIBS = -lallegro -lallegro_dialog -lallegro_primitives -lallegro_font -lallegro_ttf

# Fichiers source
SRCS = main.cpp

# Fichiers objets
OBJS = $(SRCS:.cpp=.o)

# Règle par défaut : compile ET exécute
all: $(TARGET) run_program

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compilation des fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Exécution du programme
run_program:
	./$(TARGET)

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean run_program