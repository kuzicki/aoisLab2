#include "Logical.h"
#include <set>
#include <string>
#include <stack>
#include <array>
#include <vector>
#include <bitset>

namespace Logical {
	const std::set<char> sign = { '&', '|', '!', '-', '~' };

	int priority(const char operation) {
		switch (operation) {
		case '!':
			return 3;
		case '&':
			return 2;
		case '|':
			return 1;
		case '-':
		case '~':
			return 0;
		}
		return -1;
	}

	bool calculate(const std::string& RPN, std::array<bool, 100> inputVars) {
		bool operand1 = false, operand2 = false;
		bool result = false;
		std::stack<char> st;
		int resultPos = 123;
		char curChar;
		for (int i = 0; i < RPN.length(); i++) {
			curChar = RPN[i];
			if (sign.find(curChar) == sign.end()) {
				st.push(curChar);
				result = inputVars[(int)curChar - 97];
				continue;
			}

			if (curChar == '!') {
				operand1 = inputVars[(int)st.top() - 97];
				st.pop();
				inputVars[resultPos - 97] = !operand1;
				st.push((char)resultPos);
				resultPos++;
				continue;
			}

			operand1 = inputVars[(int)st.top() - 97];
			st.pop();
			operand2 = inputVars[(int)st.top() - 97];
			st.pop();
			switch (curChar) {
			case '&': result = operand1 && operand2; break;
			case '|': result = operand1 || operand2; break;
			case '-': result = operand1 || !operand2; break;
			case '~': result = operand1 == operand2; break;
			}
			inputVars[resultPos - 97] = result;
			st.push((char)resultPos);
			resultPos++;
		}
		return result;
	}

	std::string RPN(const std::string& str, std::set<char>& vars) {
		std::stack<int> st;
		char curChar;
		std::string result = "";
		for (int i = 0; i < str.size(); i++) {
			curChar = str[i];
			if (curChar == ' ')
				continue;
			if (curChar == '(') {
				st.push(curChar);
				continue;
			}
			if (curChar == ')') {
				while (!st.empty() && st.top() != '(') {
					result += st.top();
					st.pop();
				}
				if (st.top() == '(')
					st.pop();
				continue;
			}
			if (sign.find(curChar) != sign.end()) {
				if (curChar == '-') {
					while (!st.empty() && priority(st.top()) >= priority(curChar)) {
						result += st.top();
						st.pop();
					}
					st.push(curChar);
					i++;
					continue;
				}

				while (!st.empty() && priority(st.top()) >= priority(curChar)) {
					result += st.top();
					st.pop();
				}
				st.push(curChar);
				continue;
			}
			if (curChar >= 'a' && curChar <= 'z') {
				vars.insert(curChar);
				result += curChar;
				continue;
			}
		}
		while (!st.empty()) {
			if (st.top() != '(')
				result += st.top();
			st.pop();
		}
		return result;
	}

	std::vector<std::vector<bool>> getTable(const std::string& RPN, const std::set<char>& variables) {
		std::vector<std::vector<bool>> table;
		std::vector<bool> currentRow;
		int permutationsAmount = variables.size();
		std::array<bool, 100> inputVars{ false };

		for (int mask = 0; mask != (1 << permutationsAmount); mask++) {
			std::bitset<100> bits(mask);
			auto variables_it = variables.end();
			for (int i = 0; i < permutationsAmount; i++) {
				variables_it--;
				inputVars[(int)*variables_it - 97] = bits[i];
				currentRow.insert(currentRow.begin(), bits[i]);
			}
			bool rowResult = calculate(RPN, inputVars);
			currentRow.push_back(rowResult);
			table.push_back(currentRow);
			currentRow.clear();
			std::fill(inputVars.begin(), inputVars.end(), false);
		}
		return table;
	}

	std::string getPCNF(std::vector<std::vector<bool>> table, const std::set<char>& variables) {
		std::vector<char> vars;
		for (auto vars_it = variables.begin(); vars_it != variables.end(); vars_it++) {
			vars.push_back(*vars_it);
		}
		std::string result = "";
		for (int i = 0; i < table.size(); i++) {
			if (!table[i][table[i].size() - 1]) {
				result += "(";
				for (int j = 0; j < table[i].size() - 1; j++) {
					result += (table[i][j] == false) ? "" : "!";
					std::string variableChar{ vars[j] };
					result += std::string(variableChar) + "|";
				}
				result.erase(--result.end());
				result += ")&";
			}
		}
		result.erase(--result.end());
		return result;
	}

	std::string getPDNF(std::vector<std::vector<bool>> table, const std::set<char>& variables) {
		std::vector<char> vars;
		for (auto vars_it = variables.begin(); vars_it != variables.end(); vars_it++) {
			vars.push_back(*vars_it);
		}
		std::string result = "";
		for (int i = 0; i < table.size(); i++) {
			if (table[i][table[i].size() - 1]) {
				result += "(";
				for (int j = 0; j < table[i].size() - 1; j++) {
					result += (table[i][j] == false) ? "!" : "";
					std::string variableChar{ vars[j] };
					result += variableChar + "&";
				}
				result.erase(--result.end());
				result += ")|";
			}
		}
		result.erase(--result.end());
		return result;
	}

	std::string getDecimalFormC(const std::vector<std::vector<bool>>& table) {
		std::vector<int> numberForms;
		for (int i = 0; i < table.size(); i++) {
			if (table[i][table[i].size() - 1] == false) {
				std::bitset<100> bits(0);
				int k = 0;
				for (int j = table[i].size() - 2; j >= 0; j--) {
					bits[k++] = table[i][j];
				}
				numberForms.push_back(bits.to_ulong());
			}
		}

		std::string result = "(";
		for (auto& elem : numberForms) {
			result += std::to_string(elem) + ",";
		}
		result.erase(--result.end());
		result += ") &";
		return result;
	}

	std::pair<long long, std::string> getIndexForm(const std::vector<std::vector<bool>>& table) {
		std::string binaryIndexForm = "";
		for (int i = 0; i < table.size(); i++) {
			binaryIndexForm += (table[i][table[i].size() - 1] == false) ? "0" : "1";
		}
		long long result = 0;
		int k = 0;
		for (int i = binaryIndexForm.size() - 1; i >= 0; i--) {
			result += (binaryIndexForm[i] == '1') ? pow(2, k) : 0;
			k++;
		}
		return std::pair<long long, std::string>(result, binaryIndexForm);
	}

	std::string getDecimalFormD(const std::vector<std::vector<bool>>& table) {
		std::vector<int> numberForms;
		for (int i = 0; i < table.size(); i++) {
			if (table[i][table[i].size() - 1] == true) {
				std::bitset<100> bits(0);
				int k = 0;
				for (int j = table[i].size() - 2; j >= 0; j--) {
					bits[k++] = table[i][j];
				}
				numberForms.push_back(bits.to_ulong());
			}
		}

		std::string result = "(";
		for (auto& elem : numberForms) {
			result += std::to_string(elem) + ",";
		}
		result.erase(--result.end());
		result += ") |";
		return result;
	}
}