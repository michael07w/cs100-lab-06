#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
	// must return false if column doesn't exist, otherwise we would be trying to make a query on a nonexistent column
	if (column == -1)
	    return false;

        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains: public Select_Column {
    private:
	std::string value_to_find;
    public:
	// Constructor stores column index and calls parent class constructor
	Select_Contains(Spreadsheet* sheet, std::string col_name, std::string value)
	: Select_Column(sheet, col_name) {
	    value_to_find = value;
	}
	
	// check if row includes value
	virtual bool select(const std::string& s) const {
            // search for string in column
	    if (s.find(value_to_find) != std::string::npos)
		return true;
	    return false;
	}
};

class Select_Not: public Select {
    private:
	Select* local_select_obj;

    public:
	// Constructor stores reference to Select
	Select_Not(Select* select) {
	    local_select_obj = select;
	}

	// Destructor should delete internal Select object
	~Select_Not() {
	    delete local_select_obj;
	}

	// Implement virtual function from base class (Select)
	virtual bool select(const Spreadsheet* sheet, int row) const {
	    if (local_select_obj->select(sheet, row) == false)
		return true;
	    return false;
	}
};

#endif //__SELECT_HPP__
