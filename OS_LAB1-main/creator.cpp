#include <iostream>
#include <fstream>
#include "employee.h"

int main(int argc, char* argv[])
{
	std::ofstream fout(argv[1], std::ios::binary);

    int number_of_records = atoi(argv[2]);
	employee* records = new employee[number_of_records];
    

	std::cout << "\nType information about " << number_of_records << " employees:\n" << std::endl;
	for (int i = 0; i < number_of_records; i++)
	{
		std::cout << "Employee Id " << (i + 1) << ": ";
		std::cin >> records[i].num;
		std::cout << std::endl;
        std::cout << "Employee name ";
        std::cin >> records[i].name;
        std::cout << std::endl;
        std::cout << "Employee hours ";
        std::cin >> records[i].hours;
        std::cout << std::endl;
	}

	fout.write((char*)&number_of_records, sizeof(int));
	for (int i = 0; i < number_of_records; i++) {
		fout.write((char*)&records[i], sizeof(employee));
	}

	fout.close();

	delete[] records;

	return 0;
}