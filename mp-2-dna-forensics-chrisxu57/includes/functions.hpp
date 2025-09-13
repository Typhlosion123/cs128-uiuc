#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <vector>
#include <map>

#include "utilities.hpp"

std::string ProfileDNA(const std::string& dna_database,
                       const std::string& dna_sequence);

std::vector<std::string> ParseHeader(const std::string& file_name);

std::map<std::string, std::vector<int>> ReadDate(const std::string& file_name);

int LongestSTR(const std::string& sequence, const std::string& STR);

#endif