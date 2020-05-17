//Author:	John Smith
//Assignment 3

#ifndef SUB_H
#define SUB_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

static std::vector<int> stack;
static std::vector<int> display;
static std::vector<std::string> names;

void print(std::string = "");
void displayPrint(size_t);

class Sub
{
public:

	Sub(std::string name, Sub* parent, size_t argCount, size_t varCount);

	void call_();
	void return_();
	size_t getDepth();
	size_t getChildren();
	void addChild(Sub*);
	std::string getName();
	


private:

	std::string name; 
	Sub* parent; 
	std::vector<Sub*> children; 
	size_t argCount; 
	size_t varCount; 
	size_t depth; 

};

#endif // !SUB_H

//you can't get the variable and argument count from the stack