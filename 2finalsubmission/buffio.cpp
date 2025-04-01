#include "buffio.h"
#include "error.h"
#include <fstream>
#include <iostream>

int src_line_no = 0;           // define the two global line counter variables
int src_col_no = -1;           // start at -1 for now because lex calls get_next_char_first. Should fix logic later to make better
int buffer_size = 0;           // STARTS AT 1, UNLIKE BUFFER WHICH STARTS AT 0
char *global_buffer = nullptr; // initiialize the global buffer
int buffer_index = -1;

int get_file_size(const char *f)
{
    // std::cout << "file name is: " << f << std::endl;

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
    // std::cout << f << " is " << char_counter << std::endl;
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

int count_new_lines()
{
    int counter = 0;
    for (int i = 0; i < buffer_size; i++)
    {
        if (global_buffer[i] == '\n')
        {
            counter++;
        }
    }
    return counter;
}

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

    global_buffer = new char[size + 1];
    populate_buffer_with_file(filename, size);
    global_buffer[size] = ' ';      // set a space. should change later because it is not good practive but i am running out of time.
    global_buffer[size + 1] = '\0'; // set null terminator
    // std::cout << "Buffer contents:\n";
    // for (int i = 0; i < size; i++)
    //{
    //     std::cout << global_buffer[i];
    // }

    // int k = count_new_lines();
    std::cout << std::endl;
    //  delete[] global_buffer;
    return NCC_OK;
    // return buffer_cleanup();
}

int buffer_get_cur_char(char &c)
{
    // std::cout << "at position: " << buffer_index << " Character is: " << global_buffer[buffer_index] << std::endl;

    if (global_buffer[buffer_index] == '\0')
    {
        // std::cout << "end of file was reached" << std::endl;
        return NCC_EOF;
    }
    c = global_buffer[buffer_index];
    return NCC_OK;
}

int buffer_next_char(void)
{
    src_col_no++;
    buffer_index++;
    if (global_buffer[buffer_index] == '\n')
    {
        // buffer_index++;
        src_col_no = 0;
        src_line_no++;
    }

    return NCC_OK;
}

int buffer_get_next_char(char &c)
{
    src_col_no++;
    buffer_index++;
    if (global_buffer[buffer_index] == '\n')
    {
        // buffer_index++;
        // std::cout << "a new line was detected, setting the col to 0 now" << std::endl;
        src_col_no = 0;
        src_line_no++;
    }

    return buffer_get_cur_char(c);
}

bool buffer_eof(void)
{
    return buffer_index >= buffer_size;
}

int buffer_back_char(void)
{
    // std::cout << "Go back was called" << std::endl;
    if (global_buffer[buffer_index - 1] != '\n')
    { // if the column number is 1 or above, simply decrement the column number and buffer index
        buffer_index--;
        src_col_no--;
        return NCC_OK;
    }

    if (buffer_index < 0) // if the buffer index is less than 0, we cant go back so return out of bounds
    {
        return NCC_OUTOFBOUNDS;
    }

    // if the charcter back was a new line, we need to decrement the buffer_index by one,  as well as set the line number to -- and the col to 0
    buffer_index = buffer_index - 1;
    src_col_no = 0;
    src_line_no--;
    return NCC_OK;
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
