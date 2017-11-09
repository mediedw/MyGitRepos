// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
// Math trick from the book "Here's Looking at Euclid".
// Choose any three digit number where the first and last digit differ by at least 2
// Reverse the number and subtract the smaller from the larger. 
// Take the result and reverse it, add it to the result and you will always get 1089.
// Ex: 753, 753 - 357 = 396; 396 + 693 = 1089.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int reverse_digits(int num);

int _tmain(int argc, _TCHAR* argv[])
{
	int abc = 0;	// Hold the users input.  Used abc to be able to later break the number to
					// its respective digits for reversal. 
	int cba = 0; 
	int def = 0;	// result of the difference between the input and reversed input.
	int fed = 0;	// reversal of the difference.

	int userChoice = 'n'; 
	char option;

	while(userChoice != 'y')
	{
		// Prompt for a 3 digit number
		cout << "Please enter a 3 digit number" << endl; 
		cin >> abc; 

		cout << "You entered " << abc << endl;
		cout << endl;
		cba = reverse_digits(abc); 

		// determine larger value
		if(abc > cba)
		{
			def = abc-cba;
			cout << abc << " - " << cba << " = " << abc-cba << endl; 
		}
		else
		{
			def = cba-abc;
			cout << cba << " - " << abc << " = " << cba-abc << endl; 
		}

		fed = reverse_digits(def); 
		cout << def << " + " << fed << " = " << def + fed << endl; 

		cout << endl; 
		cout << "Exit( y or n)? "; 
		cin >> option; 
		userChoice = tolower(option); 
	}


	return 0;
}

int reverse_digits(int num)
{
	int reversed_num = 0;
	int pos100 = 0; 
	int pos10 = 0; 
	int pos1 = 0; 
	int intermediateNum = 0; 

	pos100 = num/100; 
	intermediateNum = num - (pos100 * 100); 

	pos10 = intermediateNum/ 10; 
	
	pos1 = intermediateNum - (pos10 * 10); 

	reversed_num = (pos1 * 100) + (pos10 * 10) + (pos100 * 1); 

	return reversed_num;
}
