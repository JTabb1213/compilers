#include "buffio.h"
#include "error.h"
#include <fstream>
#include <iostream>

int src_line_no = 0;           // define the two global line counter variables
int src_col_no = -1;           // start at -1 for now because lex calls get_next_char_first. Should fix logic later to make better
int buffer_size = 0;           // STARTS AT 1, UNLIKE BUFFER WHICH STARTS AT 0
char *global_buffer = nullptr; // initiialize the global buffer

int get_file_size(const char *f)
{
    std::cout << "file name is: " << f << std::endl;

    std::ifstream file(f, std::ios::in | std::ios::binary); // open file in binary mode, for reading only
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return NCC_FILE_NOT_FOUND;
    }

    char ch;
    int char_counter = 0;

    while (file.get(ch))
    {
        char_counter++;
    }
    std::cout << f << " is " << char_counter << std::endl;
    file.close();
    buffer_size = char_counter;
    return char_counter;
}

void populate_buffer_with_file(const char *name, int file_size)
{
    std::fstream file(name, std::ios::in | std::ios::binary); // open file in binary mode, for reading only
    if (!file.is_open())
    {
        std::cerr << "Error opening file in populate part!" << std::endl;
        exit(1);
    }

    char ch;
    int i = 0;

    while (i < file_size && file.get(ch))
    {
        global_buffer[i] = ch;
        i++;
    }
    // file.read(buffer, file_size);
    //  buffer[file_size] = '\0'; // Add null terminator

    file.close();
}
///////////Everything above was taken from assignment 0//////////
int buffer_init(const char *filename)
{
    int size = get_file_size(filename);
    // if (size == -1)
    if (size == NCC_FILE_NOT_FOUND)
    {
        std::cerr << "something went wrong with getting the file size, ending :(";
        return NCC_FILE_NOT_FOUND;
        // exit(EXIT_FAILURE);
    }

    global_buffer = new char[size];
    populate_buffer_with_file(filename, size);
    std::cout << "Buffer contents:\n";
    for (int i = 0; i < size; i++)
    {
        std::cout << global_buffer[i];
    }
    std::cout << std::endl;
    // delete[] global_buffer;
    return NCC_OK;
    // return buffer_cleanup();
}

int buffer_get_cur_char(char &c)
{
    int line = 0;
    int col = 0;
    int i = 0;
    while (i < buffer_size && line < src_line_no)
    { // to get to the right line.
        if (global_buffer[i] == '\n')
        {
            line++;
        }
        i++;
    }

    while (i < buffer_size)
    {
        if (col == src_col_no)
        {
            c = global_buffer[i];
            return NCC_OK;
        }
        col++;
        i++;
    }

    return NCC_EOF;
    // std::cout << "HERE" << std::endl;
    //  c = global_buffer[src_line_no * src_col_no]; // try to get it like this
    //  if the above method doesnt work, try indexing through
    // std::cout << "getting to line: " << src_line_no << " col: " << src_col_no << std::endl;
    /*
    int line = src_line_no;
    int col = 0;
    int i = 0;              // keep track of column
    while (i < buffer_size) // go through buffer
    {
        // std::cout << "ACCESING GLOBAL BUFFER: " << std::endl;
        // std::cout << "BUFFER IS: " << global_buffer[7] << std::endl;
        if (global_buffer[i] == '\n') // if the char encounter is a newline, decrement the line counter by one
        {
            // std::cout << "NEWline character was discovered at: " << std::endl;
            line--;
            std::cout << "Newline detected, we are on line: " << src_line_no << std::endl;
        }

        if (line == 0) // if/once we are on the right line, increment the col until we get it equal to the src_col
        {
            std::cout << "we are on the right line, now getting the column (which should be " << src_col_no << ") " << col << "character is: " << global_buffer[i] << std::endl;
            if (col == src_col_no)
            {
                c = global_buffer[i];
                // std::cout << "character read was: " << c << std::endl;
                return NCC_OK;
            }
            col++;
        }

        // if (line == 0 && global_buffer[i] == '\n')
        //{
        //     return NCC_EOF;
        // }
        // std::cout << "size of i is: " << i << std::endl;

        i++; // move to the next character in the buffer.
    }

    std::cout << "EOF reached at size: " << i << "/" << buffer_size << std::endl;
    return NCC_EOF;
    */
}

int buffer_next_char(void)
{
    src_col_no++;
    if (global_buffer[src_col_no] == '\n')
    {
        src_col_no = 0;
        src_line_no++;
    }

    return NCC_OK;
}

int buffer_get_next_char(char &c)
{
    src_col_no++;
    if (global_buffer[src_col_no] == '\n')
    {
        std::cout << "a new line was detected, setting the col to 0 now" << std::endl;
        src_col_no = 0;
        src_line_no++;
    }

    return buffer_get_cur_char(c);
}

bool buffer_eof(void)
{
    int counter = 0;
    int line = src_line_no;
    int col = 0; // initialize all to 0

    while (counter < buffer_size)
    { // while the counter is less the buffer size
        if (line == 0)
        { // if we are at the right line, increment col until we reach the src_col_no
            if (col == src_col_no && global_buffer[counter] != '\n')
            {
                return false; // not eof, line and col are valid
            }

            if (global_buffer[counter] == '\n')
            { // we are out of bounds on the line
                return true;
            }
            col++;
        }

        if (global_buffer[counter] == '\n')
        { // if new line is encountered, decrement line till we get to correct line
            line--;
        }

        counter++;
    }

    return true; // if counter is larger than buffer, return true we are at eof
}

int buffer_back_char(void)
{
    if (src_col_no > 0)
    { // if the column number is 1 or above, simply decrement the column number
        src_col_no--;
        return NCC_OK;
    }

    if (src_line_no == 0 && src_col_no == 0)
    { // if we are at the first character of the file, return eof since we cannot go out of bounds
        return NCC_EOF;
    }

    // if we are at the -1th character, but not at the top line, we need to count the characters of the line above it and
    // set the col_no equal to the last char
    int tracker = 0; // keep track of what line we are on
    int line_tracker = 0;
    src_line_no--; // subrtact one from the line number, since we are going one above it
    while (tracker < buffer_size && line_tracker < src_line_no)
    { // loop through the buffer until we get to the correct line. This is achieved by counting the number of \n characters
        if (global_buffer[tracker] == '\n')
        {
            line_tracker++;
        }
        tracker++;
    } // once loop completes, we will be at the start of the line.

    src_col_no = 0; // update the col number to the start of the column
    while (tracker <= buffer_size && global_buffer[tracker] != '\n')
    { // get the column to one characte rbehind the \n character
        src_col_no++;
        tracker++;
    }

    if (tracker == buffer_size)
    { // means we went out of bounds if they are equal
        return NCC_EOF;
    }

    return NCC_OK;
    /*
    int tracker = 0; // keep track of what line we are on
    int line_tracker = 0;
    src_col_no--; // go back one character
    if (src_col_no == -1)
    {                   // if the src_col_no was already zero, we need to set it to the line above it, as well as count how many characters are on the line above it to set the col to the last char
        src_col_no = 0; // set the column number to zero. Will be incremented later if needed.
        if (src_line_no == 0)
        { // if we are already on the top line, return eof since we cannot go any higher
            return NCC_EOF;
        }
        else
        {
            src_line_no--; // if not top line, go to the line above it
        }

        while (tracker != src_line_no)
        { // count \n chars until we reach our line
            if (global_buffer[tracker] == '\n')
            { // if we found a new line, increment the line counter
                line_tracker++;
            }
            tracker++;
        } // once we exit the loop, tracker is on the start of the line we are on.

        while (global_buffer[tracker] != '\n')
        {                 // index through the line until we encounter the \n character
            tracker++;    // increment the tracker
            src_col_no++; // increment the col number, which is going to start at zero
        }
    }
    return NCC_OK;
    */
}

int buffer_cleanup(void)
{
    if (global_buffer != nullptr)
    {
        delete[] global_buffer;
        global_buffer = nullptr;
    }

    return NCC_OK;
}

int get_src_line(int line_no, string &line)
{
    int buffer_tracker = 0;
    int line_tracker = 0;
    while (buffer_tracker <= buffer_size && line_no > line_tracker)
    { // while the buffer_tracker is in bounds and the line_no is greater than the line_tracker
        if (global_buffer[buffer_tracker] == '\n')
        { // increnet the line_tracker if a new line is found. Will keep doing this until the line_no is == to the line_tracker
            line_tracker++;
        }
        buffer_tracker++;
    }

    if (buffer_tracker == buffer_size)
    { // means we went out of bounds above
        return NCC_EOF;
    }

    line.clear(); // make sure line is clear

    while (buffer_tracker <= buffer_size && global_buffer[buffer_tracker] != '\n')
    { // index through the line, adding all the characters to the string, until a new line is reached
        line += global_buffer[buffer_tracker];
        buffer_tracker++;
    }

    if (buffer_tracker == buffer_size)
    { // means we went out of bounds above
        return NCC_EOF;
    }

    return NCC_OK;
}
