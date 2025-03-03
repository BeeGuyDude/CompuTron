#include "computron.h"

#include <fstream>
#include <iomanip>
#include <stdexcept>

void load_from_file(std::array<int, memorySize>& memory, const std::string& filename) {
	//Initialize variables necessary for iterating through instructions
	constexpr int sentinel{-99999};		// terminates reading after -99999
  	size_t i{0}; 
  	std::string line;
  	int instruction;
	
	//Configure the file stream 
	std::ifstream inputFile(filename); 
  	if (!inputFile) {
  		throw std::runtime_error("invalid_input");
	}

  	while (std::getline(inputFile, line)) {
    	instruction = std::stoi(line); 
    	if (instruction == sentinel) break; 
 		
		std::cout << "Processing instruction: " << line << "\n";
    	//Check if the instruction is valid using the validWord function
    	if (validWord(instruction)) {
			//If the instruction is valid, store it in memory at position 'i' and increment 'i'
			memory[i] = instruction;
			i++;
		} else {	
			//If the instruction is invalid, throw a runtime error with the message "invalid_input"
  			throw std::runtime_error("invalid_input");
		}
	}
  	inputFile.close();
}

//Convert an opcode to a valid command and return it
Command opCodeToCommand(size_t opCode) {
	switch(opCode) {
    	case 10: return Command::read;
      	case 11: return Command::write;
		case 20: return Command::load;
		case 21: return Command::store;
		case 30: return Command::add;
		case 31: return Command::subtract;
		case 32: return Command::divide;
		case 33: return Command::multiply;
  		case 40: return Command::branch;
		case 41: return Command::branchNeg;
		case 42: return Command::branchZero;
		case 43: return Command::halt;
		default: 
			throw std::runtime_error("A command was run with an invalid OpCode!");
	};
}

void execute(std::array<int, memorySize>& memory, 
            int* const acPtr, size_t* const icPtr, 
            int* const irPtr, size_t* const opCodePtr, 
            size_t* const opPtr,
            const std::vector<int>& inputs) {
  
	size_t inputIndex{0}; // Tracks input
  
  	do {
    	//Populate instruction register from memory
		*irPtr = memory[*icPtr];
		
		//Separate out opCode and operand to do calculations with
		*opCodePtr = *irPtr / 100; 	//Left two digits
		*opPtr = *irPtr % 100; 		//Right two digits
    	
		switch(int word{}; opCodeToCommand(*opCodePtr)) {
      	case Command::read:
        	word = inputs[inputIndex];
			// Assign the value of 'word' to the memory location pointed to by 'opPtr'
			// Increment the instruction counter (icPtr) to point to the next instruction
			memory[*opPtr] = word;	//Read the input into memory
			inputIndex++;	//Increment the input to the next one
			(*icPtr)++;		//Increment instruction counter
			break;
		case Command::write:
        	//Dereference 'icPtr' to access the instruction counter and increment its value by 1
         	// use the below cout if needed but comment before submission
        	std::cout << "[WRITE] Contents of " << std::setfill('0') << std::setw(2) 
        	          << *opPtr << " : " << memory[*opPtr] << "\n";
			(*icPtr)++;		//Increment instruction counter
			break;
      
		case Command::load:
        	//Load the value from the memory location pointed to by 'opPtr' into the accumulator (acPtr)
        	//Increment the instruction counter (icPtr) to point to the next instruction
        	(*icPtr)++;		//Increment instruction counter
			break;

		case Command::store:
			// Store the value in the accumulator (acPtr) into the memory location pointed to by 'opPtr'
			// Increment the instruction counter (icPtr) to move to the next instruction
			(*icPtr)++;		//Increment instruction counter
			break;

		case Command::add:
			// Add the value in the accumulator (acPtr) to the value in memory at the location pointed to by 'opPtr' and store the result in 'word'
			// If the result is valid, store it in the accumulator and increment the instruction counter
			// / If the result is invalid, throw a runtime error 
			(*icPtr)++;		//Increment instruction counter
			break;

		case Command::subtract:
        	// Subtract the value in memory at the location pointed to by 'opPtr' from the value in the accumulator (acPtr) and store the result in 'word'
        	// If the result is valid, store it in the accumulator and increment the instruction counter
        	// / If the result is invalid, throw a runtime error 
        	(*icPtr)++;		//Increment instruction counter
			break;

      	case Command::multiply:
        	// as above do it for multiplication
        	(*icPtr)++;		//Increment instruction counter
			break;

      	case Command::divide:
         	// as above do it for division
        	(*icPtr)++;		//Increment instruction counter
			break;

      	case Command::branch:
        	*icPtr = *opPtr;
        	break;
        
      	case Command::branchNeg:
        	*acPtr < 0 ? *icPtr = *opPtr : ++(*icPtr);
        	break;
        
      	case Command::branchZero:
        	*acPtr == 0 ? *icPtr = *opPtr : ++(*icPtr);
        	break;
        
      	case Command::halt:
			//Do nothing and break
        	break;
		
      	default:
        	//Should theoretically never be called
        	break;
    	};
	} while (opCodeToCommand(*opCodePtr) != Command::halt);		//Can be modified if needed
};

//Check if a given command word is properly formatted (NOT if its opCode is valid)
bool validWord(int word) {
	//Check if the word is of the correct length
	int absWord = std::abs(word);
	if (absWord > 999 && absWord < 10000) {
		return true;
	} else {
		//TODO: Integrate this edge case into the main logic better
		if (absWord == 0) return true;
		return false;
	}
}

//Debug dump all CompuTron member variables based on current command
void dump(std::array<int, memorySize>& memory, int accumulator, 
         size_t instructionCounter, size_t instructionRegister,
         size_t operationCode, size_t operand) {
	//Output block of the working variables in order
	std::cout << "Registers\n"
			  << "accumulator\t\t" << accumulator << "\n"
			  << "instructionCounter\t" << instructionCounter << "\n"
			  << "instructionRegister\t" << instructionRegister << "\n"
			  << "operationCode\t\t" << operationCode << "\n"
			  << "operand\t\t\t" << operand << "\n\n";

	//Output the entire working memory
		
}

//Output the working memory of the CompuTron as a block
void output(std::string label, int width, int value, bool sign) {
	
}

