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

TEST(SelectContains, SheetDoesNotContainValueInColumn) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "mathematics"});

    Select* select = new Select_Contains(&sheet, "Last", "Johnson");
    EXPECT_FALSE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectContains, SheetContainsExactValue) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "mathematics"});

    Select* select = new Select_Contains(&sheet, "Major", "mathematics");
    EXPECT_TRUE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectContains, SearchForSubstring) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "computer science"});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    Select* select = new Select_Contains(&sheet, "Major", "math");
    EXPECT_TRUE(select->select(&sheet, 1));
    delete select;
}

TEST(SelectContains, CaseMismatch) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "computer science"});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    Select* select = new Select_Contains(&sheet, "Major", "Math");
    EXPECT_FALSE(select->select(&sheet, 1));
    delete select;
}

TEST(SelectContains, SearchForEmptyString) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "computer science"});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    // empty string should always return true
    // empty string is a substring of all strings
    Select* select = new Select_Contains(&sheet, "Major", "");
    EXPECT_TRUE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectContains, EmptyStringMatchWithEmptyString) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", ""});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    // empty string should match with empty string
    Select* select = new Select_Contains(&sheet, "Major", "");
    EXPECT_TRUE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectContains, QueryNonExistentColumn) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    Select* select = new Select_Contains(&sheet, "Middle", "Steve");
    EXPECT_FALSE(select->select(&sheet, 0));
    delete select;
}

// Need to implement test for more than one column with same name

TEST(SelectNot, SelectedItemIsNotPresent) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Dwight", "Schrute", "30", "mathematics"});

    Select* select = new Select_Not(new Select_Contains(&sheet, "Major", "english"));
    // sheet does not contain english in Major column on row 0, so we should return true
    EXPECT_TRUE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectNot, SelectedItemIsPresent) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "english"});

    Select* select = new Select_Not(new Select_Contains(&sheet, "Major", "english"));
    // sheet does contains "english" in Major column on row 0, so we should return false
    EXPECT_FALSE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectAnd, MajorandLastNameAreNotPresent) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "english"});
    Select* select = new Select_And(new Select_Contains(&sheet, "Last", "Schmo"), new Select_Contains(&sheet, "Major", "22"));
    EXPECT_FALSE(select->select(&sheet, 0));
    delete select;
}

TEST(SelectAnd, AgeandLastNameArePresent) {
    Spreadsheet sheet;
    sheet.set_column_names({"First", "Last", "Age", "Major"});
    sheet.add_row({"Joe", "Schmo", "22", "english"});
    Select* select = new Select_And(new Select_Contains(&sheet, "Last", "Schmo"), new Select_Contains(&sheet, "Age", "22"));
    EXPECT_TRUE(select->select(&sheet, 0));
    delete select;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
