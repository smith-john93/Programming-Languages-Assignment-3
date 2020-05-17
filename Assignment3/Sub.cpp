//Author:	John Smith
//Assignment 3

#include "Sub.h"

Sub::Sub(std::string name, Sub* parent, size_t argCount, size_t varCount)
{	

	this->name = name;
	this->parent = parent;
	this->argCount = argCount;
	this->varCount = varCount;

	//Calculate the depth of the current Sub
	parent != NULL ? depth = parent->getDepth() +1 : 0;
	
	if (parent != NULL)
	{
		parent->addChild(this);
	}

	names.push_back(name);
}

void Sub::addChild(Sub* newChild)
{
	children.push_back(newChild);
}

size_t Sub::getDepth()
{
	return depth;
}

void Sub::call_()
{
	int dynamicLink = stack.size() - 1;

	int encodedName = 0;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == name)
		{
			encodedName = i;
		}
	}

	int savedLink = -1;
	int parentName = 0;
	
	//get the parent name
	if (parent != NULL)
	{
		bool match = false;
		for (size_t i = 0; i < names.size(); i++)
		{
			if (names[i] == parent->name)
			{
				parentName = i;
				match = true;
				break;
			}
		}

		if (!match)
		{
			std::string exception = "The parent for sub " + name +" could not be located on the display stack";
			throw std::exception(exception.c_str());
		}
	}

	//get the size of the stack
	size_t stackPointer = stack.size();

	//if the display is empty, populate it
	if (display.size() == 0)
	{
		if (this->name != "main")
		{
			
			throw std::exception("Main must be added before adding subs to the runtime stack");
		}

		display.push_back(stackPointer);
		
		//push the encoded name
		stack.push_back(encodedName);

		//push the dynamic link
		stack.push_back(-1);

		//push the saved link
		stack.push_back(savedLink);

		//push back args
		for (size_t j = 0; j < argCount; j++)
		{
			int arg = j * 10;
			stack.push_back(arg + 1);
		}

		//push back local vars
		for (size_t k = 0; k < varCount; k++)
		{
			int var = k *  10;
			stack.push_back(var);
		}
		return;
	}
	
	//iterate over the display
	for (size_t i = 0; i < display.size(); i++)
	{
		//if the parent is on the display, action
		if (parentName == stack[display[i]])
		{
			//found the parent			
			//if we are at the top, we can start adding new
			if (display.size() == depth)
			{
				display.push_back(stackPointer);
			}
			else
			{
				//we are not at the top, lets save the stack location we are replacing for now
				savedLink = display[depth];
				display[depth] = stackPointer;
			}

			//store the encoded name on the stack
			stack.push_back(encodedName);

			//write the saved link to the stack
			//if there isn't one, this stores -1
			stack.push_back(savedLink);

			//Puch the Dynamic Link onto the stack
			stack.push_back(dynamicLink);

			//push back args
			for (size_t j = 0; j < argCount; j++)
			{
				int arg = j * 10;
				stack.push_back(arg + 1);
			}

			//push back local vars
			for (size_t k = 0; k < varCount; k++)
			{
				int var = k * 10;
				stack.push_back(var);
			}
			return;
		}
	}

	std::string excep = "No parent found for sub " + name + " in the display";
	throw std::exception(excep.c_str());
}

void Sub::return_()
{
	//get the size of this sub on the stack
	size_t subTotal = 3 + argCount + varCount;

	//get the top of the stack
	size_t stackTop = stack.size();

	//calculate the frame
	size_t frameStart = stackTop - subTotal;

	//find the ordinal of the name in the names vecotr
	//the ordinal is the encoded name
	size_t nameLoc = 0;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == name)
		{
			nameLoc = i;
		}
	}

	//if the value in the stack at frame start isn't the encoded name
	//		then this sub isn't at the top of the stack
	if (stack[frameStart] != nameLoc)
	{
		std::string err = "Cannot return " + name + ". Not at the top of the stack.";
		throw std::exception(err.c_str());
	}

	//remove the Local Variables from the stack
	for (size_t i = 0; i < varCount; i++)
	{
		stack.pop_back();
	}

	//remove the arguments from the stack
	for (size_t j = 0; j < argCount; j++)
	{
		stack.pop_back();
	}

	//remove the dynamicLink
	stack.pop_back();

	//remove the saved link
	int savedLink = stack.back();
	stack.pop_back();

	//remove the encoded name of the sub
	stack.pop_back();

	//if saved link is -1, we didn't replace anything on the stack
	//if it has a value, we need to restore it
	if (savedLink != -1)
	{
		for (size_t j = 0; j < display.size(); j++)
		{
			//find where on the display stack we are located
			if (display[j] == frameStart)
			{
				//replace the value at that location
				display[j] = savedLink;
			}
		}
	}
	else
	{
		display.pop_back();
	}
}

size_t Sub::getChildren()
{
	return children.size();
}

std::string Sub::getName()
{
	return name;
}

//this prints the runtime and display stack side by side
void print(std::string message)
{

	std::cout << "=========================================================" << std::endl;
	if (!message.empty())
	{
		std::cout << message << std::endl << std::endl;
	}

	//make sure there is a stack to look at
	if (stack.size() == 0)
	{
		std::cout << "Runtime stack is empty" << std::endl << std::endl;
		std::cout << "=========================================================" << std::endl;
		return;
	}

	//set the global position
	//this is used to calculate when we will start printing the display start
	int globalPOS = -1;
	bool firstLoop = true;
	//we only perform this loop twice
	for (int p = 0; p < 2; p++)
	{
		//make a copy of the display stack
		std::vector<int> displayCopy = display;

		//we need to know what the top of thr relevant section of the stack if
		int previousTop = stack.size() - 1;
	
		//print the header if this is our second time through
		if (!firstLoop)
		{
			std::cout << "-----Runtime Stack-----" << std::endl;
		}

		//this will be set to true when we find a display link of -1
		bool AtBottom = false;
		while (!AtBottom)
		{

			int topFrame = -1;
			int topFrameLoc = -1;

			//a simple max claculation
			for (size_t i = 0; i < displayCopy.size(); i++)
			{
				if (displayCopy[i] > topFrame)
				{
					topFrame = displayCopy[i];
					topFrameLoc = i;
				}
			}
			//now we have the bottom of the top activation record
		
			//Store the display link and the savedLink
			int savedLink = stack[topFrame + 1];
			int dynamicLink = stack[topFrame + 2];
	
			if (firstLoop) { ++globalPOS; }
			else
			{
				std::cout << "-----------------------";
				displayPrint(globalPOS);
				globalPOS--;
			}

			for (int i = previousTop; i > topFrame+2; i--)
			{		
				//incrimebt global position if this is the first loop
				if (firstLoop) { ++globalPOS; }
				else
				{
					//not the first loop, we can print the argument or variable line
					std::cout << std::setw(4);
					std::cout << std::left << (i);
					std::cout << std::setw(15);
			
					//if the number is even, it is a variable
					//	all variables are encoded as 2n
					//  all arguments are encoded as 2n+1
					if (stack[i] % 2 == 0)
					{
						std::cout << "Local Var";
					}
					else
					{
						std::cout << "Argument";
					}
					std::cout << std::setw(4);
			
					
					if (stack[i] % 2 == 0)
					{
						//if this is a variable, divide the number on the stack by 10 and print
						int value = stack[i];				
						std::cout << std::right << (value / 10);
					}
					else
					{
						//this is an argument
						//subtract 1 and then divide by 10 before printing
						int value = stack[i];
						value -= 1;
						std::cout << std::right << (value / 10);
					}
			
					displayPrint(globalPOS);
					globalPOS--;
				}
			}

			//if first loop, calculate globalPOS
			if (firstLoop) { ++globalPOS; }
			else
			{
				//print the display link
				std::cout << std::setw(4); std::cout << std::left << (topFrame +2);
				std::cout << std::setw(15);	std::cout << "Dynamic Link";
				std::cout << std::setw(4); std::cout << std::right << stack[topFrame + 2];

				displayPrint(globalPOS);
				globalPOS--;
			}

			//if first loop, calculate globalPOS
			if (firstLoop) { ++globalPOS; }
			else
			{
				//print the saved Link
				std::cout << std::setw(4); std::cout << std::left << (topFrame + 1);
				std::cout << std::setw(15);	std::cout << "Saved Link";
				std::cout << std::setw(4); std::cout << std::right << stack[topFrame + 1];
		
				displayPrint(globalPOS);
				globalPOS--;
			}

			//if first loop, calculate globalPOS
			if (firstLoop) { ++globalPOS; }
			else
			{
				//print the Sub
				std::cout << std::setw(4); std::cout << std::left << (topFrame);
				std::cout << std::setw(15);	std::cout << "Sub";
				std::cout << std::setw(4); std::cout << std::right << names[stack[topFrame]];

				displayPrint(globalPOS);
				globalPOS--;
			}

			//if dynamicLink link is -1, we are at the bottom of the runtime stack
			if (dynamicLink == -1)
			{
				AtBottom = true;
			}
			else
			{
				//set the previous top the the old runtime size
				previousTop = dynamicLink;
			}

			//if saved link is -1
			//we can reduce the size of the dislpay stack
			if (savedLink == -1)
			{
				displayCopy.pop_back();
			}
			else
			{
				//we need to restore the pointer we replaced for this activation record
				displayCopy[topFrameLoc] = savedLink;
			}

		}
		//we are no longer in the first loop
		firstLoop = false;
	}
	
	std::cout << "=========================================================" << std::endl;
	//new line to make the output easy to read
	std::cout << std::endl;
}

//this helps in the calculation of when to start writing the display stack
void displayPrint(size_t locaiton)
{
	//if display is greater than or equal to the position passed
	// we can start displaying the stack
	if (display.size() >= locaiton)
	{

		std::cout << std::setw(10);		
		std::cout << ' ';
		
		//display the header the first time we are in here
		if (display.size() == locaiton)
		{			
			std::cout << "-----Display Stack-----" << std::endl;
		}
		else
		{
			//display the frame pointer
			std::cout << std::setw(3);
			std::cout << std::left << locaiton;

			std::cout << std::setw(15);
			std::cout << std::left << "Frame Pointer";
			std::cout << std::setw(5);
			std::cout << std::right << display[locaiton] << std::endl;;
		}
	}
	else
	{
		//location is the greater value, draw a newline and exit
		std::cout << std::endl;
	}
}