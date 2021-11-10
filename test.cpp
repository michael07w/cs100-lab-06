#include "spreadsheet.cpp"

#include "gtest/gtest.h"

#include <sstream>

TEST(PrintSelectionTest, NonEmptySheetWithNullPointer) {
    std::ostringstream sstream;
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "mathematics"});
    sheet.add_row({"Michael", "Wessels", "23", "csba"});

    std::string expected_val = "Joe Schmo 22 mathematics \nMichael Wessels 23 csba \n";
    sheet.print_selection(sstream);
    EXPECT_TRUE(sstream.str() == expected_val);
}

TEST(PrintSelectionTest, EmptySheetWithNullPointer) {
    std::ostringstream sstream;
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});

    std::string expected_val = "";
    sheet.print_selection(sstream);
    EXPECT_TRUE(sstream.str() == expected_val);
}

// Will add further tests for print_selection() once SELECT_* classes are implemented

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
