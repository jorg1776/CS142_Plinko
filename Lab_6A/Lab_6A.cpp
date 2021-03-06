/*
Jordan Gruenewald, Test-Out, jorgruenewald@gmail.com

Test 1: Using seed value 42, testing single drop path.
		Should get same output every time this test is run.
Input: 1, 5, 1, 4, 1, 3
Expected output:
	Path: [5.0, 4.5, 5.0, 4.5, 5.0, 4.5, 4.0, 4.5, 5.0, 4.5, 4.0, 3.5, 4.0] Winnings: $10000.00
	Path: [4.0, 3.5, 3.0, 3.5, 3.0, 3.5, 4.0, 3.5, 4.0, 3.5, 4.0, 4.5, 4.0] Winnings: $10000.00
	Path: [3.0, 3.5, 4.0, 4.5, 4.0, 3.5, 3.0, 3.5, 3.0, 3.5, 3.0, 3.5, 3.0] Winnings: $0.00

Test 2: Testing error handling
Input: 1, 9, "x", "nine", -3, 4,
		2, -7, "twenty", 10, "two", 2
Expected output:
Single drop:
	For sllot inputs 9, "x", "nine", -3: "Invalid slot"
	For drop in slot 4: Output "Path" and "Winnings"
Multiple drop:
	For chip count inputs -7 and "twenty": "Invalid number of chips"
	For chip count 10: Prompt for slot number
	For slot input "two": "Invalid slot"
	For slot input 2: Output "Total Winnings on 10 chips" and "Average Winnings per chip

Test 3: Using seed value 42, testing sequential drop
		Should get same output every time this test is run.
Input: 3, 50
Expected output:
Total winnings on slot 0 chips: 28100.00
Average winnings per chip: 562.00

Total winnings on slot 1 chips: 46700.00
Average winnings per chip: 934.00

Total winnings on slot 2 chips: 31000.00
Average winnings per chip: 620.00

Total winnings on slot 3 chips: 79800.00
Average winnings per chip: 1596.00

Total winnings on slot 4 chips: 105100.00
Average winnings per chip: 2102.00

Total winnings on slot 5 chips: 87600.00
Average winnings per chip: 1752.00

Total winnings on slot 6 chips: 111400.00
Average winnings per chip: 2228.00

Total winnings on slot 7 chips: 32900.00
Average winnings per chip: 658.00

Total winnings on slot 8 chips: 26900.00
Average winnings per chip: 538.00
*/

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

const int SEED_VALUE = 42;
const int NUM_SLOTS = 9;
const int NUM_ROWS = 12;

const int SINGLE_OPTION = 1;
const int MULTIPLE_OPTION = 2;
const int SEQUENTIAL_OPTION = 3;
const int MENU_OPTION = 4;
const int EXIT_OPTION = 5;

float ComputeWinnings(int slotNumber)
{
	float slotValues[NUM_SLOTS] = { 100, 500, 1000, 0, 10000, 0, 1000, 500, 100 };
	return slotValues[slotNumber];
}

float CalculateAvgEarnings(float numChips, float totalEarnings)
{
	return totalEarnings / numChips;
}

void DisplayWinnings(int numChips, float winnings)
{
	cout << setprecision(2) << fixed;
	if (numChips == 1)
	{
		cout << "Winnings: $" << winnings << endl;
		cout << endl;
	}
	else if (numChips > 1)
	{
		cout << "Total winnings on " << numChips << " chips: $" << winnings << endl;
		float averageEarnings = CalculateAvgEarnings(numChips, winnings);
		cout << "Average winnings per chip: $" << averageEarnings << endl; //"setprecision(2) << fixed" is for rounding cents
		cout << endl;
	}
}

enum DropType
{
	Single,
	Multiple,
	Sequential
};

enum InputType
{
	OptionNumber,
	NumberOfChips,
	SlotSelection
};

bool ValidInput(int input, InputType type)
{
	switch (type)
	{
		case OptionNumber:
		{
			if (input < SINGLE_OPTION || input > EXIT_OPTION)
			{
				cout << "Invalid selection. Enter 4 to see options." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		}
		case NumberOfChips:
		{
			if (input <= 0)
			{
				cout << "Invalid number of chips." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		}
		case SlotSelection:
			if (input < 0 || input > (NUM_SLOTS - 1))
			{
				cout << "Invalid slot." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		default:
			return false;
	}
}

int GetUserInput()
{
	int input;
	cin >> input;

	if (cin.fail()) //if input is not an int
	{
		cin.clear();
		cin.ignore(256, '\n');
		return -1; //returns -1 so ValidateInput will show error message
	}
	else
	{
		return input;
	}
}

int GetSlotNumber()
{
	int slotNumber;
	bool valid = false;

	do {
		cout << "Which slot do you want to drop the chip in (0-" << NUM_SLOTS - 1 << ")? ";
		slotNumber = GetUserInput();

		cout << endl;

		if (!ValidInput(slotNumber, SlotSelection))
		{
			valid = false;
		}
		else
			valid = true;

	} while (valid == false);

	return slotNumber;
}

int GetNumChips()
{
	int numChips;

	do
	{
		cout << "How many chips do you want to drop (>0)? ";
		numChips = GetUserInput();
		cout << endl;

	} while (ValidInput(numChips, NumberOfChips) == false);

	return numChips;
}

int TrackChip(float chipLocation, DropType dropType)
{
	//logic for when ball hits peg
	for (int j = 0; j < NUM_ROWS; j++)
	{
		if (dropType == Single && j == 0) //for outputting first part of path on single drop
		{
			cout << setprecision(1) << fixed;
			cout << "Path: [" << chipLocation << ", ";
		}

		if (chipLocation > 0 && chipLocation < (NUM_SLOTS - 1))
		{
			int direction = rand() % 2;

			switch (direction)
			{
				case 0:
					chipLocation -= 0.5;
					break;
				case 1:
					chipLocation += 0.5;
					break;
			}
		}
		else if (chipLocation == 0)
		{
			chipLocation += 0.5;
		}
		else if (chipLocation == (NUM_SLOTS - 1))
		{
			chipLocation -= 0.5;
		}

		if (dropType == Single) //for outputting rest of path on single drop
		{
			if (j == NUM_ROWS - 1)
				cout << chipLocation << "]" << endl;
			else
				cout << chipLocation << ", ";
		}
	}

	return chipLocation;
}

float DropChips(int numChips, DropType dropType)
{
	float winnings = 0;

	switch (dropType)
	{
		case Single:
		{
			int slotNumber = GetSlotNumber();
			cout << "*** Dropping chip into slot " << slotNumber << " ***" << endl;
			winnings = ComputeWinnings(TrackChip(slotNumber, Single));
			break;
		}
		case Multiple:
		{
			int slotNumber = GetSlotNumber();
			for (int i = 0; i < numChips; i++)
			{
				winnings += ComputeWinnings(TrackChip(slotNumber, Multiple));
			}

			break;
		}
		case Sequential:
		{
			float slotWinnings[NUM_SLOTS] = {};
			
			for (int i = 0; i < NUM_SLOTS; i++)
			{
				for (int j = 0; j < numChips; j++)
				{
					slotWinnings[i] += ComputeWinnings(TrackChip(i, Sequential));
				}

				cout << setprecision(2) << fixed;
				cout << "Total winnings on slot " << i << " chips: " << slotWinnings[i] << endl;
				float averageEarnings = CalculateAvgEarnings(numChips, slotWinnings[i]);
				cout << "Average winnings per chip: " << averageEarnings << endl; //"setprecision(2) << fixed" is for rounding cents
				cout << endl;
			}

			break;
		}
	}
	return winnings;
}

void PlayGame(DropType dropType)
{
	int slotNumber;
	int numChips;
	float winnings = 0;

	switch (dropType)
	{
		case Single:
		{
			numChips = 1;
			winnings = DropChips(numChips, dropType);
			DisplayWinnings(numChips, winnings);
			break;
		}
		case Multiple:
		{
			numChips = GetNumChips();

			if (numChips < 0)
				break;

			winnings = DropChips(numChips, dropType);
			DisplayWinnings(numChips, winnings);
			break;
		}
		case Sequential:
		{
			numChips = GetNumChips();

			if (numChips < 0)
				break;

			DropChips(numChips, dropType);
			break;
		}
	}
}

void DisplayMenu()
{
	cout << "Menu: Please select one of the following options :" << endl;
	cout << "\t1 - Drop a single chip into one slot" << endl;
	cout << "\t2 - Drop multiple chips into one slot" << endl;
	cout << "\t3 - Show the options menu" << endl;
	cout << "\t4 - Quit the program" << endl;
	cout << endl;
}

int main()
{
	srand(SEED_VALUE);
	
	cout << "Welcome to the Plinko simulator! Enter 4 to see options." << endl;
	cout << endl;

	bool isValid = false;
	bool isPlaying = true;

	do
	{
		int choice;

		//prompts until input is valid or after displaying options
		do {
			cout << "Enter your selection now : ";
			choice = GetUserInput();
			cout << endl;

			if (choice == MENU_OPTION)
			{
				DisplayMenu();
				choice = -1; //causes selection prompt to appear again
			}

			isValid = ValidInput(choice, OptionNumber);

		} while (isValid == false);

		//action begins based on user selection
		switch (choice)
		{
			case SINGLE_OPTION:
			{
				cout << "*** Drop a single chip ***" << endl;
				cout << endl;
				PlayGame(Single);
				break;
			}
			case MULTIPLE_OPTION:
			{
				cout << "*** Drop multiple chips ***";
				cout << endl;
				PlayGame(Multiple);
				break;
			}
			case SEQUENTIAL_OPTION:
			{
				cout << "*** Sequentially drop multiple chips ***" << endl;
				cout << endl;
				PlayGame(Sequential);
				break;
			}
			case EXIT_OPTION:
			{
				cout << "Goodbye!";
				isPlaying = false;
				return 0;
			}
		}

	} while (isPlaying);

	return 0;
}