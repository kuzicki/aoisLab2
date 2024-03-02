#pragma once
#include <string>
#include <array>
#include <set>
#include <vector>

namespace Logical {
	int priority(const char operation);
	bool calculate(const std::string& RPN, std::array<bool, 100> inputVars);
	std::string RPN(const std::string& str, std::set<char>& vars);
	std::vector<std::vector<bool>> getTable(const std::string& RPN, const std::set<char>& variables);
	std::string getPCNF(std::vector<std::vector<bool>> table, const std::set<char>& variables);
	std::string getPDNF(std::vector<std::vector<bool>> table, const std::set<char>& variables);
	std::string getDecimalFormC(const std::vector<std::vector<bool>>& table);
	std::string getDecimalFormD(const std::vector<std::vector<bool>>& table);
	std::pair<long long, std::string> getIndexForm(const std::vector<std::vector<bool>>& table);
}