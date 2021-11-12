#include "spreadsheet.hpp"
#include "select.hpp"

#include <algorithm>
#include <iostream>
#include <map>

struct DupColumns : public std::exception {
    const char* what() const throw() {
	return "Cannot have duplicate column names!";
    }
};

Spreadsheet::~Spreadsheet()
{
    delete select;
}

void Spreadsheet::set_selection(Select* new_select)
{
    delete select;
    select = new_select;
}

void Spreadsheet::clear()
{
    column_names.clear();
    data.clear();
    delete select;
    select = nullptr;
}

void Spreadsheet::set_column_names(const std::vector<std::string>& names)
{
    // prevent duplicate column name creation
    std::map<std::string, int> mymap;
    for (int i = 0; i < names.size(); i++) {
	if (mymap.find(names[i]) == mymap.end())
	    mymap.insert(std::pair<std::string, int>(names[i], 0));
	else
	    throw DupColumns();
    }

    // no duplicates, set column names
    column_names=names;
}

void Spreadsheet::add_row(const std::vector<std::string>& row_data)
{
    data.push_back(row_data);
}

int Spreadsheet::get_column_by_name(const std::string& name) const
{
    for(int i=0; i<column_names.size(); i++)
        if(column_names.at(i) == name)
            return i;
    return -1;
}

void Spreadsheet::print_selection(std::ostream& out) const {
    // iterate through each row in spreadsheet
    for (int i = 0; i < data.size(); i++) {
	// if select returns true or select is null pointer, print each column of each row
        if (select == nullptr || select->select(this, i) == true) {
	    for (int j = 0; j < data[i].size(); j++) {
	        out << data[i][j] << " ";
	    }
	    // print newline after each row
	    out << std::endl;
	}
    }
}
