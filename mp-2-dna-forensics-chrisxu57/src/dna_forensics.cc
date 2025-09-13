#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "functions.hpp"
#include "utilities.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) return 1;

  const std::string kDatabaseDNA = argv[1];
  const std::string kSequenceDNA = argv[2];

  std::string result = ProfileDNA(kDatabaseDNA, kSequenceDNA);

  std::cout << result << std::endl;
}