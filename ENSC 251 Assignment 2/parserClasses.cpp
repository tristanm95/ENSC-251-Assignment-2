//Use only the following libraries:
#include "parserClasses.h"
#include <string>

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

//****TokenList class function definitions******

//Precondition: the input string is not Null
//PostCondition: Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str)
{
	Token *t1;
	t1 = new Token;

	if(tail == NULL && head == NULL)//Checks if the list is empty
	{
		//Sets the new node's prev and next to NULL
		t1->next = NULL;
		t1->prev = NULL;
		//Sets both Head and Tail of the new list to the new node t1
		head = t1;
		tail = t1;
	}
	
	else//If a list already exists, else statement will execute
	{
		//Sets the new token's prev and next to the current tail pointer
		t1->next = NULL;
		t1->prev = tail;

		//Sets the current tail pointer's node's next to the new token
		tail->next = t1;

		//Sets the tail pointer to the new token
		tail = t1;
	}
	//Sets the new token's string to the string
	t1->stringRep = str;
}

//PreCondition: token pointer is a non-null pointer
//Postcondition: Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token)
{
	
	if(tail == NULL && head == NULL)//Checks if the list is empty
	{
		//Sets the new token's pointers to NULL
		token->next = NULL;
		token->prev = NULL;
		//Sets the head and the tail pointer of the list to the new node
		head = token;
		tail = token;
	}

	else
	{
		//Sets the token's pointers to tail
		token->next = tail;
		token->prev = tail;

		//Sets the tails next pointer to token
		tail->next = token;

		//Updates the tail pointer
		tail = token;
	}
}

//PreCondition: Token pointer is not Null
//PostCondition: Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
	//Initialize Variables
	bool found = false;
	Token *temp = NULL;
	Token *tempB = NULL;
	Token *tempF = NULL;

	//Set a temp pointer to head (do not change head, risk of list loss)
	temp = head;

	do//Traverses through the list checking for the token to be deleted.
	{
		if(token == NULL)
		{
			break;//Break out of the while loop if the token does not exist
		}
		else
		{
			if(temp == token)
			{
				if(temp == head)
				{
					//Sets the head to the next/previous link in the list
					head = head->next;
					head->prev = head->next;

					delete temp;//Deletes the top token
					found = true;
				}

				else if(temp == tail)
				{
					//Sets the tail to the previous/next link in the list
					tail = tail->next;
					tail->next = tail->prev;

					delete temp;//Deletes the bottom token
					found = true;
				}

				else
				{
					//sets the next and previous tokens to temp to new pointers
					tempB = temp->prev;
					tempF = temp->next;

					//Disconnects the temp token from the linked list without losing the pointer
					tempB->next = temp->next;
					tempF->prev = temp->prev;

					delete temp;//Deletes the Token
					found = true;
				}//end of deletion else
			}//end of if temp matches token

			else
			{
				if(temp->next != NULL)
				{
					temp = temp->next;//Traverse through the list until either the token is found, or it doesn't exit
				}
				found = false;
			}//end of equivalence checking if/else
		}//end of Null Checking if/else
	}//end of the Do-While loop
	while((token != NULL) && !(found));
}

//****Tokenizer class function definitions******

//PostCondition: Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{
	//Initialize variables
	complete = false;
	tokenLength = 0;
	int strLength = str->length();
	bool isDelim = false;
	int i = offset;
	int j = 0;
	int k = 0;

	
    for(i = offset; i < strLength; i++)//Opening for loop for the next token, i is the current string position
    {
        for(j = 0; j < 17; j++)//Checks for any special characters that are stored in the DELIM char array
        {
			if((*str)[i] == DELIM[j] && (*str)[i + 1] == DELIM[j])//Checks for a double character operator
			{
				isDouble = true;
				return;
			}

			else if((*str)[i] == DELIM[j])//Checks for a single character operator
			{
				isSingle = true;
				return;
			}

			else
			{
				continue;//Continues to the next character stored in Delim
			}
	}//End Delim For block
		
		if(((*str)[i] == '\t') || ((*str)[i] == ' '))//Checks for any whitespace
		{
			return;
		}

		else if((*str)[i] == '\n')//Checks for the newline character, adds 1 and set complete to true
		{
			Tokenizer::complete = true;
			tokenLength = tokenLength + 1;
			return;//exit out of prepareNextString
		}

		else if ((*str)[i] == '/' && (*str)[i + 1] == '/')//Checks for inline comments
		{
			for(k = i; k < strLength; k++)//Adds 1 to the tokenLength until the newline character
			{
				if((*str)[k] == '\n')
				{
					return;
				}

				else
				{
					tokenLength = tokenLength + 1;
					continue;
				}
			}//end for block
		return;
		}

		else if ((*str)[i] == '/' && (*str)[i + 1] == '*')//Checks for a block comment
		{
			for (k = i; k < strLength; k++)//adds 1 to token length as it goes through the block comment
			{
				if ((*str)[k] == '*' && (*str)[k + 1] == '/')
				{
					return;
				}

				else
				{
					tokenLength = tokenLength + 1;
					continue;
				}
			}
			return;
		}

		else if ((*str)[i] == '#')//Looks for precompiler commands
		{
			for (k = i; k < strLength; k++)//Adds 1 to tokenLength until whitespace is reached
			{
				if ((*str)[k] == ' ' || (*str)[k] == '\t')
				{
					return;
				}
				else
				{
					tokenLength = tokenLength + 1;
					continue;
				}
			}
		}

		else if((*str)[i] == '<' && (*str)[i + 1] != ' ')//Checks for <> declarations
		{
			for(k = i; k < strLength; k++)//Counts and adds one until the end > is reached
			{
				if((*str)[k] == '>')
				{
					tokenLength = tokenLength + 1;
					return;
				}
				else
				{
					tokenLength = tokenLength + 1;
					continue;
				}
			}
		}
		
		else//If it is a number or letter, add one to token length and check the next variable in the string
		{
			tokenLength = tokenLength + 1;
			continue;
		}//End If/Else if/else block

	}//End main for loop
}//end prepareNextToken function

//Precondition: string pointer is not NULL
//Postcondition: Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *strIN)
{
	//Reset all variables in preparation for tokenizing
	bool processingInlineComment = false;
	bool processingBlockComment = false;
	bool processingIncludeStatement = false;
	bool complete = false;
	offset = 0;
	
	str = strIN;
	str->append("\n");//Add \n character removed by the getline() function
	prepareNextToken();//Calls prepareNextToken for tokenizing
	return;
}

//PreCondition: prepareNextToken has been called and str is not NULL
//PostCondition: Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken()
{
	complete = false;
	offset = offset + tokenLength;//Sets the new offset after the token
	string tempString;
	
	if ((*str)[offset] == '/' && (*str)[offset + 1] == '/')//Checks if inline comment is preset
	{
		processingInlineComment = true;
		tempString = str->substr(offset, tokenLength);
	}

	else if ((*str)[offset] == '/' && (*str)[offset + 1] == '*')//Checks for Block Comments
	{
		processingBlockComment = true;
		tempString = str->substr(offset, tokenLength);
	}

	else if ((*str)[0] == '#')//Checks for preprocessor commands
	{
		processingIncludeStatement = true;
		tempString = str->substr(offset, tokenLength);
	}

	else if ((*str)[offset] == ' ' || (*str)[offset] == '\t')//Checks for Whitespace
	{
		tempString = str->substr(offset - tokenLength, tokenLength);
		offset = offset + 1;
	}

	else if ((*str)[offset - tokenLength] == '\n')//Checks for the newline character
	{
		Tokenizer::complete = true;//Sets complete at end of line
	}
	
	else
	{
		if (isDouble && tokenLength == 0)//If double operator, tokenLength is 2, offset has two added
		{
			tempString = str->substr(offset, 2);
			offset = offset + 2;
			isDouble = false;
		}

		else if (isSingle && tokenLength == 0)//If single operator, tokenLength is 1, offset has 1 added
		{
			tempString = str->substr(offset, 1);
			offset = offset + 1;
			isSingle = false;
		}
		
		else//Current string is NOT a special case and only a number/hex/variable/etc.
		{
			tempString = str->substr(offset - tokenLength, tokenLength);
		}
	}


	tokenLength = 0;//Reset tokenLength
	Tokenizer::complete = false;//completion is false
	prepareNextToken();//Calls PrepareNextToken
	return tempString;//Returns the last string
}
