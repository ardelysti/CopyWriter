//
//  Pande Nyoman Ardelysti Kardi
//  main.cpp
//  CS132_HW6
//  3/14/2019
//

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "WordPair.h"
#include <set>
#include <vector>

using namespace std;

multiset<WordPair> createList(int);
string makeSentence(multiset<WordPair>, int);
WordPair chooseRandomFromMultiset(multiset<WordPair>, string);
char isGood(char x);
string grabLast(string, int);

// my helpers
void endsWithTXT(string&);
bool hasEnded(string, int);

int main()
{
    int level;
    multiset<WordPair> myPairs;
    bool good = true;
    
    /* initialize random seed: */
    srand (time(NULL));
    
    cout << "Welcome to the grand writing artificial intelligence\n";
    cout << "How may we help you in this fine sun-basking morning?\n";
    cout << endl;
    do
    {
        cout << "Please type in the level of search (1-10): ";
        cin >> level;
        cin.ignore();
        
        if (level < 0 || level > 10)
        {
            good = false;
            cout << "\tInvalid level type. Please try again!\n";
        }
        else good = true;
    } while (!good);
    
    // Ask for le file + check for file existence
    try
    {
        myPairs = createList(level);
        cout << endl;
    } catch (string x)
    {
        cout << x << endl;
        return -99;
    }
    
    bool done = false;
    do
    {
        cout << "\n*****************************************************\n";
        cout << "After careful thought, I surmise that the next\n";
        cout << "sentence to your opus should be :\n";
        cout << "*****************************************************\n";
        
        /* // Debugging
        multiset<WordPair>::iterator it;
        
        // Checks my multiset
        for (it = myPairs.begin(); it != myPairs.end(); it++)
        {
            cout << *it << ", ";
        }
        
        cout << endl;
        */
        
        // This will take a lot while for Dracula.txt and Moby Dick.txt
        string sentence;
        sentence = makeSentence(myPairs, level);
        cout << ">> " << sentence << endl;
         
        cout << "\n*****************************************************\n";
        cout << "do you want another? Press enter to quit, anything else to repeat.\n";
        cout << endl << endl;
        
        if (cin.get() == '\n') done = true;
        cin.ignore();
        
    } while (!done);
    
    cout << "Good Riddance!\n";
    
    system("pause");
    return 0;
}

// HELPER METHODS
void endsWithTXT(string &fileName)
{
    string txtENDING = ".txt";
    if (fileName.length() >= txtENDING.length())
    {
        if (0 != fileName.compare (fileName.length() - txtENDING.length(), txtENDING.length(), txtENDING))
            fileName = fileName + txtENDING;
        return;
    }
    
    // else
    fileName += txtENDING;
}

bool hasEnded(string myString, int quotationMarkFreq)
{
    if (myString.length() < 2) return false;
    // In case there's a string with "Mr. Bennett"
    if (quotationMarkFreq > 0 && quotationMarkFreq%2 == 1) return false;
    if (quotationMarkFreq > 0 && quotationMarkFreq%2 == 0)
    {
        switch (myString[myString.length()-2])
        {
            case '?':
                return true;
            case '.':
                return true;
            case '!':
                return true;
        }
    }
    // Checking very last of string passed
    switch (myString[myString.length()-1])
    {
        case '?':
            return true;
        case '.':
            return true;
        case '!':
            return true;
    }
    return false;
}
// END OF HELPER METHODS

// Creates the multiset of WordPair's
multiset<WordPair> createList(int depth)
{
    string fname, line;
    multiset<WordPair> list;
    
    cout << "Please type in the file name you would like your opus based on: ";
    // cin.ignore() // cuts off first letter of fname for some reason
    getline(cin, fname);
    cout << fname;
    endsWithTXT(fname);
    ifstream dataFile(fname); // basically: dataFile.open(fname);
    
    if (!dataFile.is_open()) {throw string("ERROR OPENING FILE!");}

    cout << "File Opened Successfully: " << fname << endl;
    
    // This variable has to be outside otherwise sentences that have
    // more than one line will get cut.
    string prevString = "";
    bool endOfSentence = false;
    int quotationMark = 0;
    
    while (getline(dataFile, line))
    {
        // analyze the characters in one line of the file
        // line.size() - 1, so no out of bounds problems
        for (int i = 0; i < line.size(); i++)
        {
            // if the current character is the beginning of a sentence
            if ( !isspace(line[i]) && endOfSentence && quotationMark == 0)
            {
                prevString = "";
                endOfSentence = false;
            }
            if ( isGood(line[i]) != '~')
            {
                if (!endOfSentence || quotationMark > 0)
                {
                    // cannot compare line[i-1] with line[i] because analyzing per line
                    // if ( !(isspace(line[i-1]) && isspace(line[i]))) {
                    if (line[i] == '\"')
                    {
                        if (quotationMark > 0) quotationMark--;
                        else quotationMark++;
                    }
                    WordPair temp(prevString, line[i]);
                    list.insert(temp);
                    //}
                    
                    // If reaches the end of sentence, reset mapper
                    if (line[i] == '.' || line[i] == '?' || line[i] == '!')
                    {
                        endOfSentence = true;
                    }
                    prevString += line[i];
                    if (endOfSentence && quotationMark == 0)
                    {
                        prevString = "";
                    }
                }
                
            }
            
            if (prevString.length() > depth)
                prevString = grabLast(prevString, depth);
        }
    }
    
    return list;
}

// Returns the predicted sentence
string makeSentence(multiset<WordPair> list, int depth)
{
    string mySentence = "", prevString = "";
    bool sentenceEnded = false;
    int quotationMark = 0;
    
    while (!sentenceEnded) // While sentence incomplete
    {
        WordPair temp;
        
        temp = chooseRandomFromMultiset(list, prevString);
        
        // This will return sentence even if Exact match was not found
        if (isGood(temp.getValue()) == '~') return mySentence;
        
        mySentence += temp.getValue();
        prevString += temp.getValue();
        
        if (temp.getValue() == '\"') quotationMark++;
        if (prevString.length() > depth)
            prevString = prevString.substr(prevString.length()-depth, depth);
        
        // To stop the loop once sentence has been completed
        if (hasEnded(mySentence, quotationMark)) sentenceEnded = true;
    }
    
    return mySentence;
}

// Cannot use vector random shuffle
// Can use vector random access iterator???? YAY
WordPair chooseRandomFromMultiset(multiset<WordPair> li, string prevString)
{
    vector<WordPair> choices;
    multiset<WordPair>::iterator it;
    int random = 0;
    
    for (it = li.begin(); it != li.end(); it++)
    {
        if (it->getKey() == prevString) choices.push_back(*it);
    }
    
    // Prevents returning an empty WordPair Object
    if (choices.size() == 0) return WordPair(prevString, '~');
    
    else if (choices.size() > 0)
    {
        random = ( rand() % choices.size() )  ;
         // DEBUGGING
        //cout << choices[random].getValue() << endl;
        //cout << "Random number: "<< random << endl;
        
    }
   /* // DEBUGGING
    cout << "CHECK " << random <<":" << choices.size()<< endl;
    cout << choices[random]<<endl;
     */

    return choices[random];
} // end of function

// Checks if the passed character is legible or allowed
char isGood(char x)
{
    if((int) x > 255 || x < 0) return '~';
    if (isalnum(x) ) return x;
    if (ispunct(x)) return x;
    if (x == ' ') return x;
    return '~' ;
}

string grabLast(string myString, int depth)
{
    return myString = myString.substr(myString.length()-depth, depth);
}

//********* CreateFile Loop *************//
/*
while (getline(dataFile, line))
{
    // analyze the characters in one line of the file
    // line.size() - 1, so no out of bounds problems
    for (int i = 0; i < line.size(); i++)
    {
        // if the current character is the beginning of a sentence
        if ( !isspace(line[i]) && endOfSentence && quotationMark == 0)
        {
            prevString = "";
            endOfSentence = false;
        }
        if ( isGood(line[i]) != '~')
        {
            if (!endOfSentence || quotationMark > 0)
            {
                // cannot compare line[i-1] with line[i] because analyzing per line
                // if ( !(isspace(line[i-1]) && isspace(line[i]))) {
                if (line[i] == '\"')
                {
                    if (quotationMark > 0) quotationMark--;
                    else quotationMark++;
                }
                WordPair temp(prevString, line[i]);
                list.insert(temp);
                //}
                
                // If reaches the end of sentence, reset mapper
                if (line[i] == '.' || line[i] == '?' || line[i] == '!')
                {
                    endOfSentence = true;
                }
                prevString += line[i];
                if (endOfSentence && quotationMark == 0)
                {
                    prevString = "";
                }
            }
            
        }
        
        if (prevString.length() > depth)
            prevString = prevString.substr(prevString.length()-depth, depth);
            }
}
 */
