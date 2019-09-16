//
//  Pande Nyoman Ardelysti Kardi
//  WordPair.h
//  CS132_HW6
//  3/14/2019
//

#ifndef WordPair_h
#define WordPair_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>

class WordPair
{
public:
    WordPair(std::string = "unknown", char = '~');
    ~WordPair();
    
    std::string getKey() const {return wordKey;};
    char getValue() const {return characterValue;};
    bool operator<(const WordPair&) const;
    friend std::ostream& operator<<(std::ostream&, const WordPair&);
    
private:
    std::string wordKey;
    char characterValue;
    
};

#endif /* WordPair_h */
