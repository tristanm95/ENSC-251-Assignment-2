//Use only the following libraries:
#include "parserClasses.h"
#include <string>

//Complete the implementation of the following member functions:

//****TokenList class function definitions******

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str)
{
	Token *t1;
	t1 = new Token;

	if(tail == NULL && head == NULL)
	{
		t1->next = NULL;
		t1->prev = NULL;
		head = t1;
		tail = t1;
	}
	else
	{
		//Sets the new token's prev and next to the current tail pointer
		t1->next = tail;
		t1->prev = tail;

		//Sets the current tail pointer's node's next to the new token
		tail->next = t1;

		//Sets the tail pointer to the new token
		tail = t1;
	}
	//Sets the new token's string to the string
	t1->stringRep = str;
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token)
{
	if(tail == NULL && head == NULL)
	{
		token->next = NULL;
		token->prev = NULL;
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

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
	//Initialize Variables
	bool found = false;
	Token *temp = NULL;
	Token *tempB = NULL;
	Token *tempF = NULL;

	temp = head;

	do
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
					temp = temp->next;
				}
				found = false;
			}//end of equivalence checking if/else
		}//end of Null Checking if/else
	}//end of the Do-While loop
	while((token != NULL) && !(found));
}

//****Tokenizer class function definitions******

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{
	complete = false;
	int strLength = str->length();
	bool isDelim = false;
	bool isSpecDelim = false;

	for (int i = 0; i < strLength; i++)
	{
		int j = 0;
		for (j = 0; j < strLength; j++)
		{
			if (str[i] == DELIM[j])
			{
				isDelim = true;
				break;
			}

			else if (str[i] == specDELIM[j])
			{
				isSpecDelim = true;
				break;
			}
			
			else
			{
				tokenLength += 1;
				offset += 1;
			}
		}

		if (isSpecDelim)
		{
			if (str[i] == "#")
			{
				j = i;
				while (str[j] != ">")
				{
					tokenLength += 1;
					offset += 1;
					j++;
				}
			}

			else if (str[i] == "/" && str[i + 1] == "/")
			{
				while (str[j] != "\n")
				{
					tokenLength += 1;
					offset += 1;
					j++;
				}
			}

			else if (str[i] == "/" && str[i + 1] == "*")
			{
				while (str[j] != "*" && str[j + 1] != "/")
				{
					tokenLength += 1;
					offset += 1;
					j++;
				}
			}

			else
			{
				tokenLength += 1;
			}
		}
	}
}

//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *strIN) 
{
	bool processingInlineComment = false;
	bool processingBlockComment = false;
	bool processingIncludeStatement = false;
	bool complete = false;

	str = strIN;
	prepareNextToken();
	return;
}

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken()
{
	string tempString;
	if (str[offset] == "/")
	{
		processingInlineComment = true;
	}

	else if (str[offset] == "/" && str[offset + 1] == "*")
	{
		processingBlockComment = true;
	}

	else if (str[offset] == "<")
	{
		processingIncludeStatement = true;
	}

	else
	{
		tempString = str->substr(offset, tokenLength);
	}

	offset = offset + tokenLength;
	tokenLength = 0;

	prepareNextToken();
	return tempString;
}