# text_file_h
![alt text](https://github.com/kimlar/text_file_h/blob/main/text_file-code-example.png "Showing the code")

<br>

![alt text](https://github.com/kimlar/text_file_h/blob/main/text_file-running.png "Showing the code in action")

## Examples of using text_file.h

### How to read a text file:
```c
#include "text_file.h"
...
text_file file = text_file_openfor_read("bigtextfile.txt");
if (file == NULL)
{
	printf("Error: Failed to open item file\n");
	exit(1); // Exit to OS
}
i64 size = text_file_get_size("bigtextfile.txt");
str text = malloc(size);
text_file_read_str(text, size, file);
//printf("%s\n", text);
free(text);
text_file_close(file);
```

### How to write a text file in append mode:
```c
#include "text_file.h"
...
text_file file = text_file_openfor_write_append("application.log");
if (file == NULL)
{
	printf("Error: Failed to open log file\n");
	exit(1); // Exit to OS
}
str text = "LOG: Application has added an another entry to the log\n";
text_file_write_str(text, file);
text_file_close(file);
```

This is a drop-in file for any Windows C projects to quickly add a higher level text file operation for reading and writing.
