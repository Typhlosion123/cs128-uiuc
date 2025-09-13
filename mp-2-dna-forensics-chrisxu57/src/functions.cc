#include "functions.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <string>


std::vector<std::string> ParseHeader(const std::string& file_name) {
  std::ifstream ifs(file_name);
  std::string header;
  if (std::getline(ifs, header)) {
    return utilities::GetSubstrs(header, ',');
  }
  return {};
}

std::map<std::string, std::vector<int>> ReadData(const std::string& file_name) {
  std::ifstream ifs(file_name);
  std::string line;
  std::getline(ifs, line); //hypothetically skips the header...?
  std::map<std::string, std::vector<int>> data;

  while (std::getline(ifs,line)) {
    std::vector<std::string> parsed = utilities::GetSubstrs(line, ',');
    std::string name = parsed[0];
    std::vector<int> amount;
    for (unsigned int i = 1; i < parsed.size(); i++) {
      amount.push_back(std::stoi(parsed[i]));
    }
    data[name] = amount;
  }
  return data;
}

int LongestSTR(const std::string& sequence, const std::string& STR) {
  int result = 0;
  unsigned int str_length = STR.length();
  for (unsigned int i = 0; i < sequence.length(); i++) {
    int str_count = 0;
    while(sequence.substr(i, str_length) == STR) {
      str_count++;
      i += str_length;
    }
    if (str_count > result) {
      result = str_count;
    }
  }
  return result;
}


std::string ProfileDNA(const std::string& dna_database,
                       const std::string& dna_sequence) {
  std::cout << "Input dna_database : " << dna_database << std::endl;
  std::cout << "Input dna_sequence : " << dna_sequence << std::endl;

  std::vector<std::string> header = ParseHeader(dna_database);
  std::map<std::string, std::vector<int>> data = ReadData(dna_database);

  std::vector<int> str_counts;
  str_counts.reserve(header.size());
  for (unsigned int i = 1; i < header.size(); i++) { 
    str_counts.push_back(LongestSTR(dna_sequence, header[i]));
  }


  std::vector<std::string> possible_matches;
  for (auto pair : data) {
    bool is_match = true;
    for (unsigned int k = 0; k < str_counts.size(); k++) {
      if (pair.second[k] != str_counts[k]) {
        is_match = false;
        break;
      }
    }
    if (is_match) {
      possible_matches.push_back(pair.first);
    }
  } 

  if (possible_matches.size() == 1) {
    return possible_matches[0];
  }

  return "No match";

}


