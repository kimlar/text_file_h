//
// Text file operations: Read, write, get file size, set position
//
// Author:	Kim Einar Larsen
// Email:	kim.developer@gmail.com
// Github:	https://github.com/kimlar
// Copyright (c) 2024 Kim Einar Larsen. All rights reserved.
//
// 
// NOTE: Since this text file utility is made for Windows,
//       the newline character is \r\n instead of \n.
// NOTE: Memory allocation is done on the user side. Not inside here.
//
// TODO: Make this also to work with C++.
// 
// 
// Example of using text_file.h
//
//		#include "text_file.h"
//		...
//		text_file file = text_file_openfor_read("bigtextfile.txt");
//		if (file == NULL)
//		{
//			printf("Error: Failed to open item file\n");
//			exit(1); // Exit to OS
//		}
//		i64 size = text_file_get_size("bigtextfile.txt");
//		str text = malloc(size);
//		text_file_read_str(text, size, file);
//		//printf("%s\n", text);
//		text_file file2 = text_file_openfor_write_new("bigtext2.txt");
//		text_file_write_str(text, file2);
//		text_file_close(file2);
//		free(text);
//		text_file_close(file);
//
//
//

#pragma once

//
// C includes
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//
// Data types
//
typedef long long			i64;
typedef unsigned char*		str;

//
// File types
//
typedef FILE*		text_file;
typedef i64			file_size;

//
// Prototypes: Text file
//
text_file text_file_openfor_write_new(str filename);
text_file text_file_openfor_write_append(str filename);
text_file text_file_openfor_read(str filename);
file_size text_file_get_size(str filename);
bool text_file_read_str(str text, i64 size, text_file file);
bool text_file_write_str(str text, text_file file);
bool text_file_set_position_begin(text_file file);
bool text_file_set_position(file_size position, text_file file);
bool text_file_set_position_relative(file_size position, text_file file);
bool text_file_set_position_end(text_file file);
file_size text_file_get_position(text_file file);
void text_file_close(text_file file);

//
// Implementations: Text file
//

// Open a text file for writing in truncate mode.
text_file text_file_openfor_write_new(str filename)
{
	// Create a text file. Truncate the file if it already exists.
	text_file file = fopen(filename, "w");

	return file;
}
// Open a text file for writing in append mode.
text_file text_file_openfor_write_append(str filename)
{
	// Open a text file in append mode. Create if not exists.
	text_file file = fopen(filename, "a");

	return file;
}
// Open a text file for reading in text mode.
text_file text_file_openfor_read(str filename)
{
	// Read a text file in text mode. Returns NULL if the file does not exist.
	text_file file = fopen(filename, "r");

	return file;
}
// Get the size of a text file. Note: This function returns -1 if the text file is not found
file_size text_file_get_size(str filename)
{
	text_file file = text_file_openfor_read(filename);
	if (file == NULL)
		return -1; // Return -1 if the text file is not found

	if (_fseeki64(file, 0, SEEK_END) != 0)
		return -1; // Failure

	file_size data_size = _ftelli64(file);

	text_file_close(file);

	return data_size; // Return the data size of the text file
}
// Read 'str' data from a text file
bool text_file_read_str(str text, i64 size, text_file file)
{
	// Read specified portion of the text file into memory. Typically all of the file.
	size_t bytes = fread(text, sizeof(char), size, file);
	if (ferror(file) != 0)
		return false; // Error reading file

	text[bytes] = '\0';
	
	return true; // Success
}
// Write 'str' data to a text file
bool text_file_write_str(str text, text_file file)
{
	if (fwrite(text, sizeof(char), strlen(text), file) != strlen(text))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Seek to the beginning of a text file
bool text_file_set_position_begin(text_file file)
{
	return (_fseeki64(file, 0, SEEK_SET) == 0); // Returns true if the seek returns 0 meaning that the seek was successful
}
// Seek to an absolute position in a text file
bool text_file_set_position(file_size position, text_file file)
{
	return (_fseeki64(file, position, SEEK_SET) == 0); // Returns true if the seek returns 0 meaning that the seek was successful
}
// Seek to a relative position in a text file
bool text_file_set_position_relative(file_size position, text_file file)
{
	return (_fseeki64(file, position, SEEK_CUR) == 0); // Returns true if the seek returns 0 meaning that the seek was successful
}
// Seek to the end of a text file
bool text_file_set_position_end(text_file file)
{
	return (_fseeki64(file, 0, SEEK_END) == 0); // Note: the regular fseek() supports only file sizes up to 2GB. This _fseeki64() supports up to 8 Exabytes.
}
// Get the current position in a text file
file_size text_file_get_position(text_file file)
{
	return _ftelli64(file);
}
// Close a text file
void text_file_close(text_file file)
{
	fclose(file);
}
