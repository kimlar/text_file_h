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
// NOTE: If you are using text_file_get_length(..) to get the size to allocate memory,
//       then you don't need to add +1 in the size when allocating memory,
//       otherwise remember to add +1 like this: text = malloc(size + 1);
//       when dealing with strings (null terminated).
//
// TODO: Make this also to work with C++.
// 
// Example of using text_file.h
//
//		#include "text_file.h"
//		...
//		text_file file = text_file_openfor_read("bigtextfile.txt");
//		if (file == NULL)
//		{
//			printf("Error: Failed to open reading file\n");
//			exit(1); // Exit to OS
//		}
//		i64 length = text_file_get_length("bigtextfile.txt");
//		str text = malloc(length);
//		text_file_read_str(text, length, file);
//		//printf("%s\n", text);
//		text_file file2 = text_file_openfor_write_new("bigtext2.txt");
//		text_file_write_str(text, file2);
//		text_file_close(file2);
//		free(text);
//		text_file_close(file);
//
//
// Example of writing numbers to a text file
// 
//		str filename = "hello.txt";
//		text_file file = text_file_openfor_write_new(filename);
//		if (file == NULL)
//		{
//			printf("Error: Failed to open for writing file\n");
//			exit(1); // Exit to OS
//		}
//		text_file_write_str("Hello, World!", file);
//		text_file_write_i8(-128, file);
//		text_file_write_i16(-32768, file);
//		text_file_write_i32(-2147483647, file);
//		text_file_write_i64(-9223372036854775807, file);
//		text_file_write_u8(255, file);
//		text_file_write_u16(65535, file);
//		text_file_write_u32(4294967295, file);
//		text_file_write_u64(18446744073709551615, file);
//		text_file_close(file);
//
// 
// Example of reading numbers from a text file
// 
//		str filename = "hello.txt";
//		text_file file = text_file_openfor_read(filename);
//		if (file == NULL)
//		{
//			printf("Error: Failed to open for reading file\n");
//			exit(1); // Exit to OS
//		}
//		
//		i8 value1 = 0;
//		if (text_file_read_i8(&value1, 4, file))
//			printf("%hhi\n", value1);
//		
//		i16 value2 = 0;
//		if (text_file_read_i16(&value2, 6, file))
//			printf("%hi\n", value2);
//		
//		i32 value3 = 0;
//		if (text_file_read_i32(&value3, 11, file))
//			printf("%i\n", value3);
//		
//		i64 value4 = 0;
//		if (text_file_read_i64(&value4, 20, file))
//			printf("%lli\n", value4);
//		
//		u8 value5 = 0;
//		if (text_file_read_u8(&value5, 3, file))
//			printf("%hhu\n", value5);
//		
//		u16 value6 = 0;
//		if (text_file_read_u16(&value6, 5, file))
//			printf("%hu\n", value6);
//		
//		u32 value7 = 0;
//		if (text_file_read_u32(&value7, 10, file))
//			printf("%u\n", value7);
//		
//		u64 value8 = 0;
//		if (text_file_read_u64(&value8, 20, file))
//			printf("%llu\n", value8);
//		
//		text_file_close(file);
//
//
// Example of reading bool from a text file
//
//		str filename = "hello.txt";
//		text_file file = text_file_openfor_read(filename);
//		if (file == NULL)
//		{
//			printf("Error: Failed to open for reading file\n");
//			exit(1); // Exit to OS
//		}
//		
//		bool value1;
//		if (text_file_read_bool(&value1, file))
//			printf("%d = %s\n", value1, value1 ? "true" : "false");
//		
//		text_file_close(file);
//
// 
// Example of writing bytes from a text file
// 
//		str filename = "hello.txt";
//		text_file file = text_file_openfor_write_new(filename);
//		if (file == NULL)
//		{
//			printf("Error: Failed to open for writing file\n");
//			exit(1); // Exit to OS
//		}
//		i64 length = 12;
//		byte* data = (byte*)malloc(length);
//		if (data == NULL)
//		{
//			printf("Error: Failed to allocate memory\n");
//			exit(1); // Exit to OS
//		}
//		memcpy(data, "Hello World!", length);
//		text_file_write_byte(data, length, file);
//		free(data);
//		text_file_close(file);
// 
// 
// Example of reading bytes from a text file
//
//		str filename = "hello.txt";
//		text_file file = text_file_openfor_read(filename);
//		if (file == NULL)
//		{
//			printf("Error: Failed to open for reading file\n");
//			exit(1); // Exit to OS
//		}
//		
//		i64 length = 12;
//		byte* data = malloc(length);
//		if (data == NULL)
//		{
//			printf("Error: Failed to allocate memory\n");
//			exit(1); // Exit to OS
//		}
//		if (text_file_read_byte(data, length, file))
//		{
//			for (i64 i = 0; i < length; i++)
//				printf("%c", data[i]);
//			printf("\n");
//		}
//		free(data);
//		
//		text_file_close(file);
// 
//

#pragma once

// Disable warnings
#define _CRT_SECURE_NO_WARNINGS

//
// C includes
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//
// Data types
//
typedef char				 i8;
typedef short				i16;
typedef int					i32;
typedef long long			i64;
typedef unsigned char		 u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef float				f32;
typedef double				f64;
typedef unsigned char	   byte;
typedef unsigned char*		str;
typedef unsigned char		 c8;

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
file_size text_file_get_length(str filename);
bool text_file_write_i8(i8 data, text_file file);
bool text_file_write_i16(i16 data, text_file file);
bool text_file_write_i32(i32 data, text_file file);
bool text_file_write_i64(i64 data, text_file file);
bool text_file_write_u8(u8 data, text_file file);
bool text_file_write_u16(u16 data, text_file file);
bool text_file_write_u32(u32 data, text_file file);
bool text_file_write_u64(u64 data, text_file file);
bool text_file_write_f32(f32 data, text_file file);
bool text_file_write_f64(f64 data, text_file file);
bool text_file_write_bool(bool data, text_file file);
bool text_file_write_byte(byte* data, i64 length, text_file file);
bool text_file_write_str(str text, text_file file);
bool text_file_read_i8(i8* data, u8 length, text_file file);
bool text_file_read_i16(i16* data, u8 length, text_file file);
bool text_file_read_i32(i32* data, u8 length, text_file file);
bool text_file_read_i64(i64* data, u8 length, text_file file);
bool text_file_read_u8(u8* data, u8 length, text_file file);
bool text_file_read_u16(u16* data, u8 length, text_file file);
bool text_file_read_u32(u32* data, u8 length, text_file file);
bool text_file_read_u64(u64* data, u8 length, text_file file);
bool text_file_read_bool(bool* data, text_file file);
bool text_file_read_byte(byte* data, i64 length, text_file file);
bool text_file_read_str(str text, i64 length, text_file file);
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
	text_file file = fopen((const char *)filename, "w");

	return file;
}
// Open a text file for writing in append mode.
text_file text_file_openfor_write_append(str filename)
{
	// Open a text file in append mode. Create if not exists.
	text_file file = fopen((const char*)filename, "a");

	return file;
}
// Open a text file for reading in text mode.
text_file text_file_openfor_read(str filename)
{
	// Read a text file in text mode. Returns NULL if the file does not exist.
	text_file file = fopen((const char*)filename, "r");

	return file;
}
// Get the length of a text file. Note: This function returns -1 if the text file is not found
file_size text_file_get_length(str filename)
{
	text_file file = text_file_openfor_read(filename);
	if (file == NULL)
		return -1; // Return -1 if the text file is not found

	if (_fseeki64(file, 0, SEEK_END) != 0)
		return -1; // Failure

	file_size length = _ftelli64(file);

	text_file_close(file);

	return length; // Return the length of the text file
}
// Write 'i8' data to a text file as text
bool text_file_write_i8(i8 data, text_file file)
{
	char buffer[5] = { 0 };
	sprintf(buffer, "%hhi", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'i16' data to a text file as text
bool text_file_write_i16(i16 data, text_file file)
{
	char buffer[7] = { 0 };
	sprintf(buffer, "%hi", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'i32' data to a text file as text
bool text_file_write_i32(i32 data, text_file file)
{
	char buffer[12] = { 0 };
	sprintf(buffer, "%i", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'i64' data to a text file as text
bool text_file_write_i64(i64 data, text_file file)
{
	char buffer[21] = { 0 };
	sprintf(buffer, "%lli", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'u8' data to a text file as text
bool text_file_write_u8(u8 data, text_file file)
{
	char buffer[4] = { 0 };
	sprintf(buffer, "%hhu", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'u16' data to a text file as text
bool text_file_write_u16(u16 data, text_file file)
{
	char buffer[6] = { 0 };
	sprintf(buffer, "%hu", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'u32' data to a text file as text
bool text_file_write_u32(u32 data, text_file file)
{
	char buffer[11] = { 0 };
	sprintf(buffer, "%u", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'u64' data to a text file as text
bool text_file_write_u64(u64 data, text_file file)
{
	char buffer[21] = { 0 };
	sprintf(buffer, "%llu", data);
	if (fwrite(buffer, sizeof(char), strlen(buffer), file) != strlen(buffer))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'f32' data to a text file as text
bool text_file_write_f32(f32 data, text_file file)
{
	// We are using snprintf instead of sprintf because sprintf does not handle float's well as it would need to handle a large number of digits.
	int len = snprintf(NULL, 0, "%f", data);
	char* str_data = malloc(len + 1);
	if(str_data == NULL)
		return false; // Something went wrong while trying to write the data
	snprintf(str_data, len + 1, "%f", data);

	if (fwrite(str_data, sizeof(char), strlen(str_data), file) != strlen(str_data))
		return false; // Something went wrong while trying to write the data

	free(str_data);

	return true; // Success
}
// Write 'f64' data to a text file as text
bool text_file_write_f64(f64 data, text_file file)
{
	// We are using snprintf instead of sprintf because sprintf does not handle double's well as it would need to handle a large number of digits.
	int len = snprintf(NULL, 0, "%lf", data);
	char* str_data = malloc(len + 1);
	if (str_data == NULL)
		return false; // Something went wrong while trying to write the data
	snprintf(str_data, len + 1, "%lf", data);

	if (fwrite(str_data, sizeof(char), strlen(str_data), file) != strlen(str_data))
		return false; // Something went wrong while trying to write the data

	free(str_data);

	return true; // Success
}
// Write 'bool' data to a text file as text. Writing 'true' as '1' and 'false' as '0'.
bool text_file_write_bool(bool data, text_file file)
{
	char buffer = data ? '1' : '0';

	if (fwrite(&buffer, sizeof(char), 1, file) != 1)
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'byte' data to a text file.
bool text_file_write_byte(byte* data, i64 length, text_file file)
{
	if (fwrite(data, sizeof(byte), length, file) != length)
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Write 'str' data to a text file
bool text_file_write_str(str text, text_file file)
{
	if (fwrite(text, sizeof(char), strlen(text), file) != strlen(text))
		return false; // Something went wrong while trying to write the data

	return true; // Success
}
// Read 'i8' data from a text file from part of string
bool text_file_read_i8(i8* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 4;
	if(length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if(fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%hhi", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'i16' data from a text file from part of string
bool text_file_read_i16(i16* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 6;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%hi", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'i32' data from a text file from part of string
bool text_file_read_i32(i32* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 11;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%i", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'i64' data from a text file from part of string
bool text_file_read_i64(i64* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 20;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%lli", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'u8' data from a text file from part of string
bool text_file_read_u8(u8* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 3;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%hhu", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'u16' data from a text file from part of string
bool text_file_read_u16(u16* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 5;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%hu", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'u32' data from a text file from part of string
bool text_file_read_u32(u32* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 10;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%u", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'u64' data from a text file from part of string
bool text_file_read_u64(u64* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 20;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%llu", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'f32' data from a text file from part of string. The 'length' is the number of chars to read.
bool text_file_read_f32(f32* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 20;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%f", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'f64' data from a text file from part of string. The 'length' is the number of chars to read.
bool text_file_read_f64(f64* data, u8 length, text_file file)
{
	const u8 MAX_LENGTH = 20;
	if (length > MAX_LENGTH) // Clamp it to 'length' so that it doesn't go out of bounds
		length = MAX_LENGTH;

	str value = calloc(MAX_LENGTH + 1, sizeof(char));
	if (value == NULL)
		return false; // Something went wrong while trying to read the data

	if (fread(value, sizeof(char), length, file) != length)
		return false; // Something went wrong while trying to read the data

	if (sscanf(value, "%lf", data) != 1)
		return false; // Something went wrong while trying to read the data

	free(value);

	return true; // Success
}
// Read 'bool' data from a text file. Read character '1' for true and '0' for false.
bool text_file_read_bool(bool* data, text_file file)
{
	c8 value = '\0';
	if (fread(&value, sizeof(char), 1, file) != 1)
		return false; // Something went wrong while trying to read the data

	if (value == '0')
	{
		*data = false;
		return true; // Success
	}
	else if (value == '1')
	{
		*data = true;
		return true; // Success
	}

	return false; // Something went wrong while trying to read the data
}
// Read 'byte' data from a text file
bool text_file_read_byte(byte* data, i64 length, text_file file)
{
	if (fread(data, sizeof(byte), length, file) != length)
		return false; // Something went wrong while trying to read the data

	return true; // Success
}
// Read 'str' data from a text file
bool text_file_read_str(str text, i64 length, text_file file)
{
	// Read specified portion of the text file into memory. Typically all of the file.
	i64 bytes = fread(text, sizeof(char), length, file);
	if (ferror(file) != 0)
		return false; // Error reading file

	text[bytes] = '\0';
	
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
