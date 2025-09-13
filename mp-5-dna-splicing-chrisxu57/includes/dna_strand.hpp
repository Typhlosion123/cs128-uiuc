#ifndef DNA_STRAND_HPP
#define DNA_STRAND_HPP

#include "node.hpp"

class DNAstrand {
public:
  DNAstrand() = default;
  DNAstrand(const DNAstrand& rhs) = delete;
  DNAstrand& operator=(const DNAstrand& rhs) = delete;
  ~DNAstrand();

  void SpliceIn(const char* pattern, DNAstrand& to_splice_in);

private:
  Node* head_ = nullptr;
  Node* tail_ = nullptr;

  struct MatchLocation {
    Node* previous = nullptr;
    Node* start = nullptr;
    Node* end = nullptr;
  };

  MatchLocation FindLastPattern(const char* pattern, unsigned int pattern_length) const;
  void RemoveBases(Node* previous, Node* start, Node* end);
  void SpliceBases(Node* previous, Node* end, DNAstrand& replacement);
  
};

#endif