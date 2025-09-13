#include "dna_strand.hpp"
#include "node.hpp"

#include <stdexcept>

DNAstrand::~DNAstrand() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

DNAstrand::MatchLocation DNAstrand::FindLastPattern(const char* pattern, unsigned int pattern_length) const {
    MatchLocation location;
    Node* current = head_;
    Node* previous = nullptr;

    while (current != nullptr) {
        Node* temp = current;
        bool found = true;
        for (unsigned int i = 0; i < pattern_length; ++i) {
            if (temp == nullptr || temp->data != pattern[i]) {
                found = false;
                break;
            }
            temp = temp->next;
        }

        if (found) {
            Node* end = current;
            for (unsigned int j = 1; j < pattern_length; ++j) { //1 to under lengths because we always jump 1 less of length 
                if (end == nullptr) {break; } // sanity check
                end = end->next;
            }
            if (end != nullptr) { // sanity check pt. 2, do not want memory leaks or pointing to some non allocated mem
                location.previous = previous;
                location.end = end;
                location.start = current;
            }
        }
        previous = current;
        current = current->next;
        if (found) {
            for (unsigned int k = 1; k < pattern_length && current != nullptr; ++k) { // if found need to jump to the next possible matching pattern
                current = current->next;
                previous = previous->next;
            }
        }

    }

    return location;
}

void DNAstrand::RemoveBases(Node* previous, Node* start, Node* end) {
    if (previous != nullptr) { // check if first
        previous->next = end->next;
    } else {
        head_ = end->next;
    }
    if (end == tail_) { // check if last
        tail_ = previous;
    }

    Node* current = start; // logic to delete all the inbetween ifykykykykyky
    while (current != end) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    delete end;
}

void DNAstrand::SpliceBases(Node* previous, Node* end_and_next, DNAstrand& replacement) {
    if (previous != nullptr) { // if first check
        previous->next = replacement.head_;
    } else {
        head_ = replacement.head_;
    }

    if (replacement.tail_ != nullptr) { //sannity check aghainnnnnn
        replacement.tail_->next = end_and_next;
    }

    if (end_and_next == nullptr) { // check if last
        tail_ = replacement.tail_;
    }

    replacement.head_ = nullptr;
    replacement.tail_ = nullptr;
}

void DNAstrand::SpliceIn(const char* pattern, DNAstrand& to_splice_in) {
    if (pattern == nullptr || pattern[0] == '\0') { //check if empty and shiii
        return;
    }
    if (to_splice_in.head_ == nullptr) {return; } //check if the coming in is empty
    if (this == &to_splice_in) {return; } // check identityal

    unsigned int pattern_length = 0;
    unsigned int i = 0;
    while (pattern[i] != '\0') {
        pattern_length++;
        i++;
    }
    MatchLocation location = FindLastPattern(pattern, pattern_length);
    if (location.start == nullptr) {
        throw std::runtime_error("pattern cannot be located bum (splicein)");
    }

    Node* keeper = location.end->next;
    RemoveBases(location.previous, location.start, location.end);
    SpliceBases(location.previous, keeper, to_splice_in);
}



