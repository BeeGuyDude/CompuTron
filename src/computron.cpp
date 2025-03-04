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
    	case 00: throw std::runtime_error("A (blank) variable was attempted to be decoded!");
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
		default: throw std::runtime_error("A command was run with an invalid OpCode!");
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
    	
		//Zero check to skip over variables if contained inside the program?
		if (*opCodePtr == 0) {
			(*icPtr)++;
			break;
		}

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
        	//std::cout << "[WRITE] Contents of " << std::setfill('0') << std::setw(2) 
        	//          << *opPtr << " : " << memory[*opPtr] << "\n";
			(*icPtr)++;		//Increment instruction counter
			break;
      
		case Command::load:
        	//Load the value from the memory location pointed to by 'opPtr' into the accumulator (acPtr)
        	//Increment the instruction counter (icPtr) to point to the next instruction
        	(*acPtr) = memory[*opPtr];
			(*icPtr)++;		//Increment instruction counter
			break;

		case Command::store:
			// Store the value in the accumulator (acPtr) into the memory location pointed to by 'opPtr'
			// Increment the instruction counter (icPtr) to move to the next instruction
			memory[*opPtr] = *acPtr;
			(*icPtr)++;		//Increment instruction counter
			break;

		case Command::add:
			// Add the value in the accumulator (acPtr) to the value in memory at the location pointed to by 'opPtr' and store the result in 'word'
			// If the result is valid, store it in the accumulator and increment the instruction counter
			// / If the result is invalid, throw a runtime error 
			word = *acPtr + memory[*opPtr];
			if (std::abs(word) < 10000) {
				*acPtr = word;	//Set the accumulator to the result
				(*icPtr)++;		//Increment instruction counter
			} else {
				throw std::runtime_error("The result of an addition operation overflowed the buffer size!");
			}
			break;

		case Command::subtract:
			word = *acPtr - memory[*opPtr];
			if (std::abs(word) < 10000) {
				*acPtr = word;	//Set the accumulator to the result
				(*icPtr)++;		//Increment instruction counter
			} else {
				throw std::runtime_error("The result of a subtraction operation overflowed the buffer size!");
			}
			break;

      	case Command::multiply:
			word = *acPtr * memory[*opPtr];
			if (std::abs(word) < 10000) {
				*acPtr = word;	//Set the accumulator to the result
				(*icPtr)++;		//Increment instruction counter
			} else {
				throw std::runtime_error("The result of a multiplication operation overflowed the buffer size!");
			}
			break;

      	case Command::divide:
			word = *acPtr / memory[*opPtr];
			
			//Check if the division operation is possible, throw error if not
			if (std::abs(*acPtr) < std::abs(memory[*opPtr]) || memory[*opPtr] == 0) {
				throw std::runtime_error("An invalid division operation was attempted!");
			}

			if (std::abs(word) < 10000) {
				*acPtr = word;	//Set the accumulator to the result
				(*icPtr)++;		//Increment instruction counter
			} else {
				throw std::runtime_error("The result of a division operation overflowed the buffer size!");
			}
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
		//Edge case checking 
		if (absWord == 0) {
			return true;
		} else if (absWord == 99999) {
			return true;
		} else {
			return false;
		}
	}
}

//Debug dump all CompuTron member variables based on current command
void dump(std::array<int, memorySize>& memory, int accumulator, 
         size_t instructionCounter, size_t instructionRegister,
         size_t operationCode, size_t operand) {
	//Output block of the working variables in order
	//std::cout << "Registers\n"
	//		  << "accumulator\t\t" << accumulator << "\n"
	//		  << "instructionCounter\t" << instructionCounter << "\n"
	//		  << "instructionRegister\t" << instructionRegister << "\n"
	//		  << "operationCode\t\t" << operationCode << "\n"
	//		  << "operand\t\t\t" << operand << "\n\n";
	
	output("accumulator", 4, accumulator, true);
	output("instructionCounter", 2, instructionCounter, false);
	output("instructionRegister", 4, instructionRegister, true);
	output("operationCode", 2, operationCode, false);
	output("operand", 2, operand, false);

	//Output the entire working memory iteratively
	std::cout << "Memory:\n";
	
	for (int i = 0; i < 10; i++) std::cout << "\t" << i;
	
	//Row loop
	for (size_t index = 0; index < memorySize; index++) {
		//Check if column header
		if (index % 10 == 0) {
			//Print tens place of memory address
			std::cout << "\n" << index - (index % 10);
		}
		
		//Print item at address
		output("", 4, memory[index], true);
	}
	//Add newline after memory dump
	std::cout << "\n";	
}

//Output one specific word with a sign and width, and potentially a string label
void output(std::string label, int width, int value, bool sign) {
	//Print label and separator
	std::cout << label << "\t";

	//Check if sign is included
	if (sign) {
		if (value < 0) {
			std::cout << "-";
		} else {
			std::cout << "+";
		}
	}

	//Check if the value is of the correct width (and pad it if not), and print it
	std::string valueStr = std::to_string(value);
	if (valueStr.length() < (unsigned long int)width) {
		//Left pad the value with zeroes
		for (size_t i = 0; i < width - valueStr.length(); i++) std::cout << "0";
	}
	std::cout << valueStr;
	if (label != "") std::cout << "\n";
}

