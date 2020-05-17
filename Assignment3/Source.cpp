//Author:	John Smith
//Assignment 3

#include <iostream>
#include "Sub.h"

void main()
{
	

	Sub main_("main", NULL, 0, 0);
	Sub sub1("sub1", &main_, 3, 1);
	Sub sub2("sub2", &main_, 1, 2);
	Sub sub3("sub3", &main_, 0, 1);
	Sub sub4("sub4", &sub2, 1, 0);
	Sub sub5("sub5", &sub2, 1, 1);
	Sub sub6("sub6", &sub5, 2, 2);
	

//can add to stack
#pragma region Test1
	std::cout << "Test 1" << std::endl;
	
	main_.call_();
	sub1.call_();
	print("Main and Sub1 added to stack");

	std::cout << std::endl;
#pragma endregion

//can remove from stack
#pragma region Test2
	std::cout << "Test 2" << std::endl;

	sub1.return_();
	main_.return_();
	print("Stacks after removal of main and sub1");

	std::cout << std::endl;
#pragma endregion

//can store and restore
#pragma region Test3
	std::cout << "Test 3" << std::endl;

	main_.call_();
	sub1.call_();
	sub2.call_();

	print("Sub2 should store sub1's pointer");

	sub2.return_();

	print("Sub1 sould be back on the display stack");

	sub1.return_();
	main_.return_();

	std::cout << std::endl;
#pragma endregion

//test to see if we can add anything before main
#pragma region Test4

	std::cout << "Test 4" << std::endl;
	try
	{
		sub1.call_();
	}
	catch (std::exception exception)
	{
		std::cout << "Exception:" << std::endl;
		std::cout << '\t' << exception.what() << std::endl << std::endl;
	}
#pragma endregion

//test 5 can process that stack after exceptions
#pragma region Test5
	std::cout << "Test 5" << std::endl;

	main_.call_();
	sub2.call_();
	sub5.call_();
	sub6.call_();

	try
	{
		sub5.return_();
	}
	catch (std::exception exception)
	{
		std::cout << "Exception:" << std::endl;
		std::cout << '\t' << exception.what() << std::endl;
	}

	sub6.return_();
	sub5.return_();
	sub2.return_();
	main_.return_();

	std::cout << std::endl;
#pragma endregion

//try adding something not visible from the top of the runtime stack
#pragma region Test6
	std::cout << "Test 6" << std::endl;
	
	main_.call_();
	sub1.call_();
	sub2.call_();
	sub3.call_();

	try
	{
		sub4.call_();
	}
	catch(std::exception exception)
	{
		std::cout << "Exception:" << std::endl;
		std::cout << '\t' << exception.what() << std::endl << std::endl;
	}

	sub3.return_();
	sub2.return_();
	sub1.return_();
	main_.return_();

#pragma endregion

//try removing something not at the top of the stack
#pragma region Test7
	std::cout << "Test 7" << std::endl;

	main_.call_();
	sub1.call_();
	sub2.call_();

	try
	{
		sub1.return_();
	}
	catch (std::exception exception)
	{
		std::cout << "Exception:" << std::endl;
		std::cout << '\t' << exception.what() << std::endl << std::endl;
	}

	sub2.return_();
	sub1.return_();
	main_.return_();

#pragma endregion

//Can handle complex changes to the stacks
#pragma region Test8
	std::cout << "Test 8" << std::endl;

	//Set verbose to true to print after every step
	bool verbose = true;

	if (verbose)
	{
		main_.call_();
		print("Added main_");
		sub1.call_();
		print("Added Sub1");
		sub2.call_();			
		print("Added Sub2");
		sub5.call_();	
		print("Added Sub5");
		sub6.call_();
		print("Added Sub6");
		sub4.call_();
		print("Added Sub4");
	}
	else
	{
		main_.call_();
		sub1.call_();
		sub2.call_();
		sub5.call_();
		sub6.call_();
		sub4.call_();		
		print("stack after all subs added");
	}

	if (verbose)
	{
		sub4.return_();
		print("Removed Sub4");
		sub6.return_();
		print("Removed Sub6");
		sub5.return_();
		print("Removed Sub5");
		sub2.return_();
		print("Removed Sub2");
		sub1.return_();
	}
	else
	{
		sub4.return_();
		sub6.return_();
		sub5.return_();
		sub2.return_();
		sub1.return_();
	}

	print("Stack after removal of all but main");
	main_.return_();
#pragma endregion 

//Can call the same sub multiple times
#pragma region Test9
	std::cout << "Test 9" << std::endl;

	main_.call_();
	sub1.call_();
	sub2.call_();
	sub4.call_();
	sub4.call_();
	sub4.call_();
	sub4.call_();

	print("Showing nested call of Sub4");

	sub4.return_();
	sub4.return_();
	sub4.return_();
	sub4.return_();
	sub2.return_();
	sub1.return_();
	main_.return_();

#pragma endregion

}
