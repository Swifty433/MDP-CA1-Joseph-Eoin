//Eoin Hamill D00258444
//Joseph Bryne D00255161

#include "application.hpp"
#include <iostream>

int main()
{
	try
	{
		Application app;
		app.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	
}