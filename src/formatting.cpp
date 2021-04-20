#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

struct City {
	std::string name;
	long population;
	double cost;
};

struct Country {
	std::string name;
	std::vector<City> cities;
};

struct Tours {
	std::string title;
	std::vector<Country> countries;
};

void ruler() {
	std::cout << "\n1234567890123456789012345678901234567890123456789012345678901234567890\n" << std::endl;
}

int main() {
	Tours tours
	{
		"South America tour",
		{
			// Country
			{
				"Colombia",
				{
				// City
				{"Bogota", 8778000, 400.98},
				{"Cali", 2401000, 424.12},
				{"Medellin", 2464000, 350.98},
				{"Cartagena", 972000, 345.34}
				}
			},
			{
				"Brazil",
				{
				{"Rio De Janeiro", 13500000, 567.45},
				{"Sao Paulo", 11310000, 975.45},
				{"Salvador", 18234000, 855.99}
				}
			},
			{
				"Chile",
				{
				{"Valdivia", 260000, 569.12 },
				{"Santiago", 7040000, 520.00 }
				},
			},
				{ "Argentina", 
				{
				{ "Buenos Aires", 3010000, 723.77 }
				}
			},
		}
	};

	const int table_width{70};
	const int column1_width{20};
	const int column2_width{20};
	const int column3_width{15};
	const int column4_width{15};

	ruler();

	size_t title_length = tours.title.length();

	std::cout << std::setw((table_width - title_length)/2) << "" << tours.title << std::endl;
	std::cout << std::endl;
	std::cout << std::setw(column1_width) << std::left << "Country"
		<< std::setw(column2_width) << std::left << "City"
		<< std::setw(column3_width) << std::right << "Population"
		<< std::setw(column4_width) << std::right << "Price"
		<< std::endl;

	std::cout << std::setw(table_width) << std::setfill('-') << "" << std::endl;

	std::cout << std::setfill(' '); // reset fill character
	std::cout << std::setprecision(2) << std::fixed; // for displaying numbers

	for (auto country : tours.countries)
	{
		bool is_first = true;
		for (auto city : country.cities)
		{
			std::cout << std::setw(column1_width) << std::left << ((is_first)?country.name:"")
			    << std::setw(column2_width) << std::left << city.name 
				<< std::setw(column3_width) << std::right << city.population 
				<< std::setw(column4_width) << std::right << city.cost << std::endl;
			is_first = false;
		}
	}

	std::cout << std::endl << std::endl;

	return 0;

}