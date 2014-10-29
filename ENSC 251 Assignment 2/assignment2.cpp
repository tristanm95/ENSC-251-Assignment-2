//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>

using namespace std;

//Removes all inline comments from the tokenList including the // marker
//Returns the number of comments removed
int removeInlineComments(TokenList &tokenList);

//Remove all block comments from the tokenList including /* and */ markers
//Returns the number of block comments removed (where each pair of /* and */ count as one comment)
int removeBlockComments(TokenList &tokenList);

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;
	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("test.cpp");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while(!sourceFile.eof()) 
	{
		string lineA, lineB;

		getline(sourceFile, lineA);

		//while the current line ends with a line-continuation \ append the next line to the current line
		while(lineA.length() > 0 && lineA[lineA.length()-1] == '\\') {
			lineA.erase(lineA.length()-1, 1);
			getline(sourceFile, lineB);
			lineA += lineB;
		}

		tokenizer.setString(&lineA);
		while(!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
		//Re-insert newline that was removed by the getline function
		tokens.append("\n");
	}

	removeInlineComments(tokens);
	removeBlockComments(tokens);

	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
	Token *t = tokens.getFirst();
	while(t) {
		cout << t->getStringRep() << " ";
		t = t->getNext();
	}

	return 0;
}


//Removes all inline comments from the tokenList including the // marker
//Returns the number of comments removed
int removeInlineComments(TokenList &tokenList)
{
	Token *tmp = tokenList.getFirst();
	Token *tmp1 = tokenList.getFirst();

	while (tmp->getNext() != NULL)
	{
		if (tmp->getStringRep() == "//")
		{
			do
			{
				tmp1 = tmp;
				tmp = tmp->getNext();
				tokenList.deleteToken(tmp1);
			} while (tmp->getStringRep() != "\n");
		}

		tmp = tmp->getNext();
	}

	return 1;
}

//Remove all block comments from the tokenList including /* and */ markers
//Returns the number of block comments removed (where each pair of /* and */ count as one comment)
int removeBlockComments(TokenList &tokenList)
{
	Token *t = tokenList.getFirst();
	Token *temp1 = t;

	while (t->getNext() != NULL)
	{
		if (t->getStringRep() == "/*")
		{
			do
			{
				temp1 = t;
				t = t->getNext();
				tokenList.deleteToken(temp1);
			} while (t->getStringRep() != "*/");
		}
		t = t->getNext();
	}
	return 1;
}

