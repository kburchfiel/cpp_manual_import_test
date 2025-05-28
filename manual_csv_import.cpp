// 'Manual' C++-based TSV Import Example
// By Kenneth Burchfiel
// Released under the MIT License

// This code demonstrates how to read a tab-separated file into memory.
// It is NOT meant for production-level code and will likely fail (or,
// worse yet, return incorrect output) if the source file has missing 
// data or varying column lengths.)


// This code was based on 
// https://en.cppreference.com/w/cpp/string/basic_string/getline;
// Kerrek SB's response at
// https://stackoverflow.com/a/12133398/13097194 ;
// Programming: Principles and Practice (3rd Edition)
// by Bjarne Stroustrup (especially p. 354); and 
// A Tour of C++ (2nd Edition) by Bjarne Stroustrup.


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

//#include <error>

int main(){

std::cout << "Starting program.\n";

std::string filename = "../T_T100_SEGMENT_ALL_CARRIER_2024_top_\
10k_rows_by_pax.tsv"; 
//I think this works because the compiled file is located in a
// directory below the output. Using an absolute path also works.

/* I made the file tab-separated so that it would be easier to 
differentiate between
different components. Comma-separated files do appear to be more of a 
challenge (see https://stackoverflow.com/questions/1120140/
how-can-i-read-and-parse-csv-files-in-c),
but you could probably just use an existing 3rd-party library to handle 
them. Examples would be Hossein Moein's C++ DataFrame library 
(https://github.com/hosseinmoein/DataFrame) or Vincent La's CSV parser
(https://github.com/vincentlaucsb/csv-parser). */


std::ifstream stfs {filename};
//if(!stfs) throw("Failed to open input file.");
std::cout << "\nReading in data from tsv test file:\n";

int row_index = 0;

std::vector<std::map<std::string,std::string>> mapped_rowvec;
std::vector<std::string> col_names; // This vector will store 
        // column names so that we can then add those names to maps
        // that will get created later on.

for (std::string row; getline(stfs, row);) {
    if (row_index == 0) // We'll want to treat the first row differently,
    // as we're assuming that it includes column names rather than 
    // values.
    {
        std::istringstream first_row;
        first_row.str(row); // Based on:
    // https://en.cppreference.com/w/cpp/string/basic_string/getline
        
    // Using getline() with a tab delimiter argument in order 
    // to go through each line within our istringstream (i.e.
    // the first row) and then extract all field names from it:
    // (See https://en.cppreference.com/w/cpp/string/basic_string/getline
    // for more information.)
    for (std::string column; getline(first_row, column, '\t');){
        //Adding this column to our std::vector of columns:
        col_names.push_back(column);}
    }

    else { // In this case, the current row will be considered to
    // contain values rather than headers.

        
    // This section of the code will store all rows as :vectors of maps.
    // (This will allow for columns to be labeled, and also doesn't require 
    // that we know the number of columns, or their type, ahead of time.
    // All map values will be initialized as strings, so in order to
    // work with numbers or date/time values, those values' corresponding
    // columns will need to get converted to the proper type.

    // Initializing the current row as a new istringstream:
    std::istringstream current_row;
    current_row.str(row);

    // Creating a field_index integer that will help us keep track
    // of which column we're accessing--*and* will allow us to link
    // each column's value with its corresponding field name:
    int field_index = 0;

    // Creating a map of std::strings that can store labeled column values:
    std::map<std::string,std::string> mapped_row;
    
    // Adding each column to our map: 
    // The key of this map entry will be the corresponding
    // field name (which can be retrieved from col_names using
    // the current field_index position); its value will be 
    // the value of the string located at the current field_index
    // position.

    // Note that, in this case, getline() is being used to split
    // data within a single line (e.g. the current row) into
    // a series of tab-separated values. This saves us the trouble
    // of manually figuring out where one field ends and another begins
    // (as fields may contain whitespace-separated words).
    // This approach is based on the example code found at 
    // https://en.cppreference.com/w/cpp/string/basic_string/getline.

    for (std::string column; getline(current_row, column, '\t');)
    {
        mapped_row[col_names[field_index]] = column;
        field_index++; // Incrementing the index so that we can then
        // move on to the next column
    }
    // Adding this map to our vector of maps:
    mapped_rowvec.push_back(mapped_row);
    
    }
    row_index++;
}


std::cout << "Size of mapped_rowvec: " << mapped_rowvec.size() << "\n";
// Testing out this code:

// Retrieving all values for a given column:



std::cout << "UNIQUE_CARRIER_NAME column values:";

for (int i = 0; i < mapped_rowvec.size(); i++) {
    std::cout << mapped_rowvec[i]["UNIQUE_CARRIER_NAME"] << "\n";
} 

std::cout << "\nFinished running script.";

}