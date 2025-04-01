CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11

SRCS = main1.cpp lex.cpp error.cpp buffio.cpp token.cpp tree.cpp makeInstructions.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = ncc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
