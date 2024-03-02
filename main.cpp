#include <iostream>
#include <iomanip>
#include "Logical.h"

int main() {
	std::string str;
	std::set<char> variables;
	std::getline(std::cin, str);
	std::string RPNresult = Logical::RPN(str, variables);
	
	std::cout << RPNresult << std::endl;
	
	std::cout << '|';
	for (auto variables_it = variables.begin(); variables_it != variables.end(); variables_it++) {
		std::cout << std::setw(5) << *variables_it << '|';
	}
	std::cout << std::setw(5) << str << '|' << std::endl;
	std::vector<std::vector<bool>> truthTable = Logical::getTable(RPNresult, variables);
	for (int i = 0; i < truthTable.size(); i++) {
		std::cout << '|';
		for (int j = 0; j < truthTable[i].size() - 1; j++) {
			std::cout << std::setw(5) << truthTable[i][j] << '|';
		}
		std::cout << std::setw(5) << truthTable[i][truthTable[i].size() - 1] << '|' << std::endl;
	}
	std::pair indexForm = Logical::getIndexForm(truthTable);
	std::cout << "PDNF: " << Logical::getPDNF(truthTable, variables) << "\nPCNF: " << Logical::getPCNF(truthTable, variables) << std::endl;
	std::cout << "index: " << indexForm.first << " - " << indexForm.second << std::endl;
	std::cout << "form: " << Logical::getDecimalFormC(truthTable) << std::endl;
	std::cout << "form: " << Logical::getDecimalFormD(truthTable) << std::endl;//(!(a&b))~(c|(d->e))
}

