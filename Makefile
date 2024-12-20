CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -ICryption -IInput -Iprocess

MAIN_TARGET = encrypt_decrypt
CRYPTION_TARGET = cryption

MAIN_SRC = main.cpp \
           Cryption/Cryption.cpp \
           Input/IO.cpp \
           Input/readvar.cpp \
           process/ProcessManager.cpp

CRYPTION_SRC = Cryption/Cryptionmain.cpp \
               Cryption/Cryption.cpp \
               Input/IO.cpp \
               Input/readvar.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)

all: $(MAIN_TARGET) $(CRYPTION_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJ)

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(CRYPTION_OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET)

.PHONY: clean all
