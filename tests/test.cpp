#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "computron.h"

TEST_CASE("File Processing Tests", "[CompuTron]") {
	std::array<int, memorySize> memory{0};
	int accumulator{0};
	size_t instructionCounter{0};
	int instructionRegister{0};
	size_t operationCode{0};
	size_t operand{0};

	//Attempt to ingest an existing file properly
	load_from_file(memory, "p1.txt");
	REQUIRE(memory[2] == 2007);

	//Attempt to ingest an empty file
	std::array<int, memorySize> emptyReadTestArray{0};
	load_from_file(memory, "empty.txt");
	REQUIRE(emptyReadTestArray[0] == 0);
	//REQUIRE_THROWS_AS(load_from_file(memory, "empty.txt"), std::runtime_error);
	
	//Attempt to ingest a nonexistent file
	REQUIRE_THROWS_AS(load_from_file(memory, "nonexistent.txt"), std::runtime_error);
}

TEST_CASE("Individual Command Execution Tests", "[CompuTron]") {
	//Read Command
	
	//Write Command

	//Load Command

	//Store Command

	//Add Command

	//Subtract Command

	//Multiply Command

	//Divide Command

	//Branch Command

	//BranchNeg Command

	//BranchZero Command

	//Halt Command
}
