//
//  Pande Nyoman Ardelysti Kardi
//  WordPair.cpp
//  CS132_HW6
//  3/14/2019
//

#include "WordPair.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

WordPair::WordPair(string newKey, char newValue)
{
    wordKey = newKey;
    characterValue = newValue;
}

WordPair::~WordPair() {}

bool WordPair::operator<(const WordPair &right) const
{
    if (wordKey != right.getKey()) return wordKey < right.getKey();
    return characterValue < right.characterValue;
}

// Friend function
std::ostream& operator<<(std::ostream& os, const WordPair &newPair)
{
    os << "<" << newPair.getKey() << ", " << newPair.getValue() << ">";
    return os;
}
