
# Flags etc
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall `root-config --cflags`

# Linker flags
LDFLAGS = `root-config --libs`

# sources
SRCS = src/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)


TARGET = STC

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)



# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)