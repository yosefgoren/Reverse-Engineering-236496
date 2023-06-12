CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

TARGET := mandec

SRCS := dec.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
