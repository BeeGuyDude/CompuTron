#include "computron.h"

#define CATCH_CONFIG_MAIN

int main() {
	//Instantiate all working memory of CompuTron
	std::array<int, memorySize> memory{0};
	int accumulator{0};
	size_t instructionCounter{0};
	int instructionRegister{0};
	size_t operationCode{0};
	size_t operand{0};

	const std::vector<int> inputs{4, 5};
  
	load_from_file(memory, "p1.txt");
	
	execute(memory, &accumulator, 
        &instructionCounter, &instructionRegister,
        &operationCode, &operand, inputs);

	dump(memory, accumulator, 
        instructionCounter, instructionRegister,
        operationCode, operand);
}
