#ifndef PARSERCLASSES_H_//Checks to see if this file has already been declared
#define PARSERCLASSES_H_

/*Interface:
This .h file is a base parser class that performs a lexical analysis on an input cpp file and splits them into
separate tokens
Authors: Tristan May(301224317), Simon Zhong
Changelog:
	October 25, 2014:
		Token class:
			Added accessor functions
			Added mutator functions
			Added delete function
		TokenList
			Added token delete function
			Added token append function & overloaded append function
	October 27, 2014:
		Added SetString function in Tokenizer
	October 28, 2014:
		Added base cases for preparenexttoken
		Added special cases for double operators(exl. ->)
		Added special case for // comments
		Added special cases for the \n, \t and char(32)
	October 29, 2014:
		Debugging of #include, #ifndef, #def*/

//Use only the string library DO NOT add any other libraries
#include <string>

using namespace std;

//Common character array for delimiters (excluding whitespace, #)
const char DELIM[17] = { ':', '+', '-', '(', ')', '[', ']', '.', '-', '!', '~', '\"', '*', '&', '{', '}', ';'};

//Token class for a doubly-linked list of string tokens (node class)
class Token {
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value

	//Allow TokenList class to access Token member variables in private member space
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL
	Token() : next(NULL), prev(NULL) { }

	//Constructor with string initialization, pointers initialized to NULL
	Token(const string &stringRep) : next(NULL), prev(NULL), stringRep(stringRep) { }

	//Returns the Token's *next member
	Token* getNext ( ) const {return next;}

	//Sets the Token's *next member
	void setNext (Token* next ) {Token::next = next;}

	//Returns the Token's *prev member
	Token* getPrev ( ) const {return prev;}

	//Sets the Token's *prev member
	void setPrev (Token* prev ){Token::prev = prev;}

	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep ( ) const {return stringRep;}

	//Sets the token's stringRep variable
	void setStringRep (const string& stringRep ) {Token::stringRep = stringRep;}
};

//A doubly-linked list class consisting of Token elements
class TokenList {
private:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)
	
	friend int removeInlineComments(TokenList &tokenList);//Friend functions to remove comment nodes (inline)

	friend int removeBlockComments(TokenList &tokenList);//Friend functions to remove comment nodes (block comments)

public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(NULL), tail(NULL) { }

	//Returns a pointer to the head of the list
	Token* getFirst() const
	{return(head);}

	//Returns a pointer to the tail of the list
	Token* getLast() const
	{return(tail);}

	//Precondition: String is a non-Null value
	/*PostCondition: Creates a new token for the string input, str
		Appends this new token to the TokenList
		On return from the function, it will be the last token in the list*/
	void append(const string &str);

	//Precondition: Token pointer is not NULL
	/*PostCondition: Appends the token to the TokenList if not null
		On return from the function, it will be the last token in the list*/
	void append(Token *token);

	//Precondition: Token Pointer is not NULL
	/*Removes the token from the linked list if it is not null
		Deletes the token
		On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.*/
	void deleteToken(Token *token);
};

//A class for tokenizing a string of C++  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool processingInlineComment; //True if processing an In-line comment //
	bool processingBlockComment;  //True if processing a Block Comment /* */
	bool processingIncludeStatement; //True if processing an include statement <> ""
	bool complete; //True if finished processing the current string

	bool isDouble;
	bool isSingle;

	int lineNum;

	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed

	/*Include any helper functions here
		e.g. trimming whitespace, comment processing*/

	void resetComplete();

	/*Computes a new tokenLength for the next token
		Modifies: size_t tokenLength, and bool complete
		(Optionally): may modify offset
		Does NOT modify any other member variable of Tokenizer*/
	void prepareNextToken();

public:
	//Default Constructor
	Tokenizer() : processingInlineComment(), processingBlockComment(), processingIncludeStatement(), complete()
	{
		str = NULL;
		offset = 0;
		tokenLength = 0;
		lineNum = 0;
	}

	/*Sets the current string to be tokenized
		Resets all Tokenizer state variables
		Calls Tokenizer::prepareNextToken() as the last statement before returning.*/
	void setString(string *str);

	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const 
	{
		if (complete)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*Returns the next token. Hint: consider the substr function
		Updates the tokenizer state
		Updates offset, resets tokenLength, updates processingABC member variables
		Calls Tokenizer::prepareNextToken() as the last statement before returning.*/
	string getNextToken();
};




#endif /* PARSERCLASSES_H_ */
