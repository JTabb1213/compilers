#include <iostream>
#include <fstream>
#include "buffio.h"
#include "error.h"

int main()
{
    const char *filename = "source1.cpp"; // Use a test file for the buffer initialization.

    // Step 1: Initialize the buffer
    std::cout << "Initializing buffer with file: " << filename << std::endl;
    int init_status = buffer_init(filename);
    if (init_status != NCC_OK)
    {
        std::cerr << "Buffer initialization failed." << std::endl;
        return -1;
    }

    // Step 2: Test buffer_get_cur_char
    char current_char;
    std::cout << "Testing buffer_get_cur_char..." << std::endl;
    while (buffer_get_cur_char(current_char) == NCC_OK)
    {
        std::cout << current_char; // Print each character.
        buffer_next_char();        // Move to the next character
    }

    // Step 3: Test get_src_line function
    int line_no = 2; // Example: Get the 3rd line.
    std::cout << "\n\nTesting get_src_line for line number (index at 0) " << line_no << "..." << std::endl;
    std::string line;
    int line_status = get_src_line(line_no, line);
    if (line_status == NCC_OK)
    {
        std::cout << "Line " << line_no << ": " << line << std::endl;
    }
    else
    {
        std::cerr << "Failed to get the line." << std::endl;
    }

    // Step 4: Test buffer_back_char function
    std::cout << "\nTesting buffer_back_char..." << std::endl;
    src_line_no = 1; // define the two global line counter variables
    src_col_no = 2;
    buffer_back_char(); // Go back one character.
    buffer_get_cur_char(current_char);
    std::cout << "Current character after moving back: " << current_char << std::endl;

    // Step 5: Test buffer_eof function
    std::cout << "\nTesting buffer_eof..." << std::endl;
    if (buffer_eof())
    {
        std::cout << "End of file detected." << std::endl;
    }
    else
    {
        std::cout << "More data to read." << std::endl;
    }

    // Step 6: Cleanup
    std::cout << "\nCleaning up..." << std::endl;
    buffer_cleanup();

    return 0;
}
