CXX = g++
CXXFLAGS += -Wall -Wextra -Werror -ggdb -std=c++17

SRC_DIR = ./src
BIN_DIR = ./bin

SOURCES = $(SRC_DIR)/ContainmentAlgorithm.cpp $(SRC_DIR)/ContainmentAlgorithmBuilder.cpp

main: init $(SRC_DIR)/*
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp $(SOURCES) -o $(BIN_DIR)/main

tests: init $(SRC_DIR)/*
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/tests.cpp $(SOURCES) -o $(BIN_DIR)/tests

# urghhhh
singlefile: init $(SRC_DIR)/*
	echo "// You are NOT supposed to look at this automatically generated file!!!\n"\
		> $(BIN_DIR)/main.cpp
	echo "// https://github.com/Mrkol/hw_geometry\n"\
		>> $(BIN_DIR)/main.cpp
	cat $(SRC_DIR)/Geometry.hpp\
		$(SRC_DIR)/ContainmentAlgorithm.hpp $(SRC_DIR)/ContainmentAlgorithm.cpp\
		$(SRC_DIR)/ContainmentAlgorithmBuilder.hpp $(SRC_DIR)/ContainmentAlgorithmBuilder.cpp\
		$(SRC_DIR)/main.cpp >> $(BIN_DIR)/main.cpp

	sed -i '/#include "/d' $(BIN_DIR)/main.cpp
	sed -i '/#pragma once/d' $(BIN_DIR)/main.cpp
	sed -i '/#ifndef/d' $(BIN_DIR)/main.cpp
	sed -i '/#define/d' $(BIN_DIR)/main.cpp
	sed -i '/#endif/d' $(BIN_DIR)/main.cpp


init:
	mkdir -p $(BIN_DIR)

