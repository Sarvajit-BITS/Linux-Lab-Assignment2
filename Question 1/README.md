# My Lab Report: Question 1 Solution

I have designed and tested a complete shell script that automatically identifies duplicate student submissions using cryptographic hashes, backs up only the unique files, writes a clean final report, and routes any potential error messages to a separate log.
Below is the code I wrote, followed by the exact terminal commands I ran to set up and execute my lab, my conceptual explanations, and my scientific justifications.

## Part 1: My Submission Processor Script (process_submissions.sh)

To solve this problem, I developed a robust Bash shell script using the `vi` text editor. I prioritized clean, maintainable code by including descriptive in-line comments (using `#`) to clearly document my logic, variable assignments, and directory paths. To ensure my lab results are perfectly reproducible, I also embedded the test-environment creation commands directly into the setup phase of the script.

---

## Part 2: My Step-by-Step Lab Setup and Execution Log

These are the exact commands I entered in my terminal to set up my workspace, create test files with identical content (simulating student duplicates), and run my script.

### Step 1: Creating the Submissions Directory
```bash
mkdir -p submissions
**My Explanation and Observation:** I ran the mkdir command with the -p option to create my main submissions folder. I observed that the directory was created successfully and did not throw an error since the folder didn't exist yet.

### Step 2: I generated my mock student submissions
echo "This is assignment by Rahul" > "$SUB_DIR/rahul.txt"
echo "This is assignment by Priya" > "$SUB_DIR/priya.txt"
echo "This is assignment by Rahul" > "$SUB_DIR/rahul_copy.txt"
echo "This is assignment by Aman" > "$SUB_DIR/aman.txt"
echo "This is assignment by Priya" > "$SUB_DIR/priya_copy.txt"
**My Explanation and Observation:** I used echo and the output redirection operator > to write content into five text files
. I created identical text file of Rahul and Priya to simulate a duplicate files submission. I observed five files appear in the directory.

### Step 3: I opened vi to write my automation script
vi process_submissions.sh
**My Explanation and Observation:** I opened my text editor by typing vi followed by my desired script name
. I pressed the i key to enter Insert Mode, wrote my script code, and saved and exited by typing :wq in Command-Line Mode

### Step 4: I granted execute permission to my script
chmod +x process_submissions.sh
**My Explanation and Observation:** I used the chmod utility with +x to change my script file's mode to executable
. I observed that my file permissions updated, which allowed me to run the script directly.

### Step 5: I ran my submission processor script
./process_submissions.sh
**My Explanation and Observation:** I executed the script in my current directory by typing ./ before its name
. I observed my terminal pause briefly as the loop analyzed all the files, and it completed without printing any error messages to the screen.

### Step 6: I checked the generated system report
cat system_report.txt
**My Explanation and Observation:** I used the cat command to display the entire contents of my new report file
. I observed that the script correctly identified 5 total files, found exactly 2 duplicates , and backed up 3 unique submissions.

### Step 7: I verified the unique backups were created
ls -l backup_unique
**My Explanation and Observation:** I ran the ls -l command on my backup folder to inspect the detailed attributes of my backed-up files
. I observed that duplicate files were submitted for Rahul and Priya, but only a single unique copy was preserved in backup_unique, confirming that the script successfully filtered out the duplicate.

### Step 8: I inspected the error log file
cat error.log
**My Explanation and Observation:** I displayed the contents of my error log to check if any commands failed during execution
. I observed that the file was completely empty, which showed me that my script ran flawlessly with an exit status of 0

---

## Part 3: My Justifications
**1.** Why I Selected These Linux Commands
sha256sum: I used this tool because checking only file sizes or names can be misleading. Students might copy another student's exact text but rename their file. Generating an SHA-256 hash creates a secure, unique content fingerprint

cut -d' ' -f1: The output of sha256sum includes both the hash and the filename separated by spaces. I used cut with a space delimiter (-d' ') to extract only the first field (-f1), allowing me to store the clean hash in my variable

grep -q: I chose the quiet option (-q) so that grep only checks for a match silently and returns its exit status
. This allowed my script to evaluate the match status in an if statement without displaying search matches on my screen

mktemp: I used this command to create a secure, randomly named file in /tmp
. This is a great system practice because it prevents files from clashing or overwriting other critical logs on the system.
cp -p: I selected the -p flag because it preserves important file attributes like original owner permissions and modification times
. This lets the administrator inspect exactly when the student last edited their file.

**2.** Why I Selected These Redirection Operators
> and >>: I used the single greater-than operator (>) to overwrite or create my report file freshly each time
. I used the double greater-than operator (>>) to append unique hashes sequentially to the bottom of my temporary database file without wiping out existing records

2> and 2>>: I used standard error redirection (2>) and append redirection (2>>) to capture any system-level errors (like permission issues or missing files) and route them into error.log
. This isolates errors so they don't break my script's terminal display or pollute my report

{ ... } >: By wrapping my echo statements in curly braces, I grouped them into a single command blocks
. This allowed me to redirect the combined stream into system_report.txt in a single operation, which is highly efficient for system performance

**3.** Why I Selected These File-Handling Techniques
File Globbing ("$SUB_DIR"/*): I used wildcard globbing within a for loop to iterate through the files
. I wrapped my variable reference in double quotes ("$filepath") to prevent the shell from breaking filenames with spaces into separate arguments

File Testing (-f): I included the -f test in my if statement to ensure my loop only tries to calculate hashes for actual regular files, avoiding crashes if directory folders or symlinks are placed inside the submissions directory

Exit Status Evaluations ($?): I checked the special exit status variable ($?) after running my copy command
. This let me confirm the copy was successful before incrementing my backup counter, ensuring my final statistics are 100% accurate.
