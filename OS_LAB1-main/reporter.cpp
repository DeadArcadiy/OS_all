#include <fstream>
#include <iostream>
#include "employee.h"

int main(int argc, char *argv[])
{
	std::ifstream fin(argv[1], std::ios::binary);
	std::ofstream fout(argv[2]);
	double salary_per_hour = atof(argv[3]);

	int number_of_records;
	fin.read((char *)&number_of_records, sizeof(int));
	employee *records = new employee[number_of_records];

	for (int i = 0; i < number_of_records; i++)
	{
		fin.read((char *)&records[i], sizeof(employee));
	}

	for (int i = 0; i < number_of_records; i++)
	{
		fout << "ID: " << records[i].num << std::endl;
        fout << "Name: " << records[i].name << std::endl;
        fout << "Hours: " << records[i].hours << std::endl;
        fout << "Total salary: " << records[i].hours * salary_per_hour << std::endl;
	}
	fin.close();
	fout.close();

	delete[] records;

	return 0;
}