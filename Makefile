# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
SRCDIR = src
INCDIR = include
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/board.cpp \
          $(SRCDIR)/game.cpp \
          $(SRCDIR)/Pieces.cpp \
          $(SRCDIR)/SpecialMoves.cpp \
          $(SRCDIR)/Player.cpp \
          main.cpp

# Object files
OBJECTS = $(OBJDIR)/board.o \
          $(OBJDIR)/game.o \
          $(OBJDIR)/Pieces.o \
          $(OBJDIR)/SpecialMoves.o \
          $(OBJDIR)/Player.o \
          $(OBJDIR)/main.o

# Target executable
TARGET = chess

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile source files to object files
$(OBJDIR)/board.o: $(SRCDIR)/board.cpp $(INCDIR)/Board.h $(INCDIR)/Pieces.h $(INCDIR)/Position.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/game.o: $(SRCDIR)/game.cpp $(INCDIR)/Game.h $(INCDIR)/Board.h $(INCDIR)/SpecialMoves.h $(INCDIR)/Player.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/Player.o: $(SRCDIR)/Player.cpp $(INCDIR)/Player.h $(INCDIR)/Pieces.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/Pieces.o: $(SRCDIR)/Pieces.cpp $(INCDIR)/Pieces.h $(INCDIR)/Position.h $(INCDIR)/Board.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/SpecialMoves.o: $(SRCDIR)/SpecialMoves.cpp $(INCDIR)/SpecialMoves.h $(INCDIR)/Board.h $(INCDIR)/Pieces.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.cpp $(INCDIR)/Game.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets
.PHONY: all run clean

