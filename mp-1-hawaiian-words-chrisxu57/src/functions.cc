#include "functions.hpp"
#include "cctype"
#include "string"
#include <iostream>
#include "stdexcept"
#include "vector"

/**
 * TODO: Implement this function
 * @param word word to change to lowercase
 * @return result of changing every character in word to lowercase
 */
std::string StringToLower(const std::string& word) {
  std::string result;
  for (char c : word) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    result += c;
  }
  return result;
}

/**
 * TODO: Implement this function
 * @param v1 first character in potential vowel group
 * @param v2 second character in potential vowel group
 * @return true if v1 and v2 form a valid vowel group
 */
bool IsVowelGroup(const char& v1, const char& v2) {
  return (v1 == 'a' && (v2 == 'i' || v2 == 'e' || v2 == 'o' || v2 == 'u')) ||
    (v1 == 'e' && (v2 == 'i' || v2 == 'u')) ||
    (v1 == 'i' && v2 == 'u') ||
    (v1 == 'o' && (v2 == 'i' || v2 == 'u')) ||
    (v1 == 'u' && v2 == 'i');
}

/**
 * TODO: Complete this function
 * @param v potential vowel character
 * @return true if v is a vowel
 */
bool IsVowel(const char& v) {
  return v == 'a' || v == 'e' || v == 'i' || v == 'o' || v== 'u';
}

/**
 * TODO: Complete this function
 * @param c potential consonant character
 * @return true if c is a consonant
 */
bool IsConsonant(const char& c) {
  return c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' || c == 'n' || c == 'w'; 
}

/**
 * TODO: Implement this function
 * @param c character to check validity of
 * @return true if input c is a valid character in the Hawaiian language, false
 * otherwise
 */
bool IsValidCharacter(const char& c) {
  return IsConsonant(c) || IsVowel(c) || c == ' ' || c == '\'';
}

/**
 * TODO: Implement this function
 * @param word word to check validity of
 * @return true if every character in word is a valid character in the Hawaiian
 * language, false otherwise
 */
bool IsValidWord(const std::string& word) {
    for (char c : word) {
      if (!(IsConsonant(c) || IsVowel(c) || c == '\'' || c == ' ')) {
        return false;
      } 
    }
    return true;
}

/**
 * TODO: Implement this function
 * @param c consonant to get pronunciation of
 * @param prev character before c, used for determining w translation
 *    set to null character as default if no value is passed in
 * @return pronunciation of the consonant c as a char
 */
char ConsonantPronunciation(const char& c, const char& prev) {
  char result = 0;
  if (c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' || c == 'n' || c == '\'') {
    result = c;
    return result; 
  } if (prev == 'i' || prev == 'e') {
    return 'v';
  } 
  return 'w';
 
}

/**
 * TODO: Implement this function
 * @param v1 first vowel in a vowel grouping
 * @param v2 second vowel in a vowel grouping
 * @return the pronunciation of the vowel grouping made up of v1 and v2
 * as a string
 */
std::string VowelGroupPronunciation(const char& v1, const char& v2) {
  switch(v1) {
    case 'a':
      if (v2 == 'i' || v2 == 'e') {
        return "eye";
      } else if (v2 == 'o' || v2 == 'u') {
        return "ow";
      }

    case 'e':
      if (v2 == 'i') {
        return "ay";
      } else {
        return "eh-oo";
      }

    case 'i':
      return "ew";

    case 'o':
      if (v2 == 'i') {
        return "oy";
      } else {
        return "ow";
      }

    case 'u':
      return "ooey";
    
    default:
      return "";
  }
}

/**
 * TODO: Implement this function
 * @param v single vowel to get pronunciation of
 * @return the pronunciation of v as a string
 */
std::string SingleVowelPronunciation(const char& v) {
  switch(v) {
    case 'a':
      return "ah";

    case 'e':
      return "eh";

    case 'i':
      return "ee";

    case 'o':
      return "oh";
    
    case 'u':
      return "oo";

    default:
      return "";
  }
}

/**
 * TODO: Implement this function
 * @param prev first character in set of three passed to function
 * @param curr second character in set of three passed to function
 * @param next third character in set of three passed to function
 * @return pronunciation of curr using next and prev as needed to determine
 * result
 */
std::string ProcessCharacter(const char& prev,
                             const char& curr,
                             const char& next) {
  if (IsVowelGroup(curr, next)) {
    return VowelGroupPronunciation(curr, next);
  } 
  if (IsConsonant(curr)) {
    std::string const_result;
    char c = ConsonantPronunciation(curr, prev);
    const_result = c;
    return const_result;
  } 
  if (curr == '\'') {
    return "\'";
  } 
  if (curr == ' ') {
    return " ";
  }
  return SingleVowelPronunciation(curr);
}

/**
 * TODO: Implement this function
 * @param word string to get pronunciation of
 * @return pronunciation of word
 */
std::string Pronunciation(const std::string& word) {
  std::string result;
  for (unsigned int i = 0; i < word.size(); ++i) {
    if (i == 0) { // translation when doing the first letter
      result += ProcessCharacter('\0', word[0], word[1]);
      if (IsVowel(word[0])) {
        if (IsVowelGroup(word[0], word[1])) {
          result += "=";
          ++i;
        } else if (word[1] == ' ') {
          result += " ";
          ++i;
        } else {
          result += "-";
        }
      }
    } else if (i == word.size() - 1) { // translation for normal letters
      result += ProcessCharacter(word[i - 1], word[i], '\0');
    } else { //translation for the last letter (spaces, ', etc.)
      result += ProcessCharacter(word[i - 1], word[i], word[i + 1]);
      if (IsVowelGroup(word[i], word[i + 1])) {
        result += "-";
        ++i;
      } else if (IsVowel(word[i])) {
        if (word[i + 1] == ' ' || word[i + 1] == '\'') {
          result += word[i + 1];
          ++i;
        } else {
          result += "-";
        }
      }
    }
  }
  if (result[result.size() - 1] == '-') {
    result.pop_back();
  }
  return result;
}

/**
 * TODO: Implement this function
 * @param hawaiian_word word to get pronunciation of
 * @return the pronunciation of hawaiian_word as a string if valid
 *  and throws an invalid_argument error if not valid
 */
std::string GetPronunciation(const std::string& hawaiian_word) {

  std::string result = StringToLower(hawaiian_word);
  if (IsValidWord(result)) {
    return Pronunciation(result);
  }
  throw std::invalid_argument("not valid");
 
}