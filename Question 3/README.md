# My Lab Report: Question 3 Solution

For my class assignment, I designed the logic for a secure employee file-processing utility. To make it highly secure and efficient, I chose to use low-level Linux system calls directly instead of standard C library functions (like fopen or fprintf)
. By doing this, I interact directly with the Linux kernel, which gives me precise control over file access and avoids the extra overhead and user-space buffering that standard libraries use

To keep my database efficient, I will use fixed-length records (represented as a C structure)

```c
struct Employee {
    int id;
    char name[50];
    float salary;
};
```
Because every employee record will be exactly the same size in bytes, I can easily calculate the exact location of any record on the disk without reading the file from the beginning

---
## Part 1: My Program Logic Design

Here is how I designed my program's logic to handle the four required tasks:

**Creating a File:** I will use the open() system call with the flags O_RDWR | O_CREAT | O_TRUNC
. The O_CREAT flag tells the kernel to create the file if it doesn't exist yet, and O_TRUNC empties any old data so I can start fresh
. To make it secure, I will set the file permissions to S_IRUSR | S_IWUSR (octal 0600), which means only the owner of the file can read or write to it

**Writing Employee Records:** I will define an employee record structure (for example, storing an ID, a name, and a salary)
. To add new records, I will place my structure data into a memory buffer and use the write() system call to write the bytes directly to the file descriptor

**Retrieving Records Efficiently from Any Location:** Since I am using fixed-length records, I can jump directly to any record index using random access
. If I want to read the record at index i, I can calculate its byte offset using: 

**Offset=i×sizeof(struct Employee)**

 I will use the lseek() system call with SEEK_SET to move my file's internal pointer directly to that calculated offset
. Once the pointer is there, I will call read() to load only that specific record into my memory buffer

**Updating Specific Records Without Rewriting the Whole File:** Instead of loading the entire file into memory, changing a record, and saving everything back, I can update a record in-place. 
. First, I will calculate the record's byte offset and use lseek() to jump directly to it
. Then, I will call write() with the updated employee structure
. The kernel will overwrite only the bytes at my current file pointer position, leaving all other records completely untouched

---
## Part 2: How the System Calls Work Together

These five system calls are the core building blocks of the UNIX I/O model
. Here is how each of them contributes to making my program work:

**open():** This is my entry point
. It opens my file and returns a file descriptor (a small, positive integer)
. This file descriptor represents the open file to my process, and I must use it in all subsequent system calls so the kernel knows which file I am talking about

**read():** This call retrieves data from my open file descriptor and places it into a memory buffer that I provide
. It only reads the exact number of bytes I ask for
. In my program, it allows me to load specific employee structures into memory for displaying or processing

**write():** This call takes data from my memory buffer and writes it directly to the file
. It is crucial for both appending new employee records and overwriting existing ones during in-place updates

**lseek():** This system call is the key to my utility's efficiency
. It does not write or read anything physically; it simply adjusts the kernel's internal record of the file offset (the read-write pointer)
. By using lseek(), I can skip sequential reading entirely and jump my pointer to any byte position in a split second

**close():** When I am finished with my file-processing work, I must call close() to release the file descriptor and free up the system's kernel resources
. If I do not close my file descriptors, my program will suffer from resource leaks and could eventually crash if it runs out of descriptors
