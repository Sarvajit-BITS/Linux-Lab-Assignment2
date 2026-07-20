#!/bin/bash
# =====================================================================
# Script Name: process_submissions.sh
# Author: Sarvajit Parsaniya
# Purpose: Detect duplicate files, backup unique ones, and log errors.
# =====================================================================

# I am defining my directory paths and files as variables
SUB_DIR="./submissions"
BACKUP_DIR="./backup_unique"
REPORT_FILE="./system_report.txt"
ERROR_LOG="./error.log"

# =====================================================================
# SETUP: Creating test files for demonstration purposes
# =====================================================================
mkdir -p "$SUB_DIR"

echo "This is assignment by Rahul" > "$SUB_DIR/rahul.txt"
echo "This is assignment by Priya" > "$SUB_DIR/priya.txt"
echo "This is assignment by Rahul" > "$SUB_DIR/rahul_copy.txt"
echo "This is assignment by Aman" > "$SUB_DIR/aman.txt"
echo "This is assignment by Priya" > "$SUB_DIR/priya_copy.txt"
# =====================================================================

# I am initializing my counter variables to zero
processed_count=0
duplicate_count=0
backup_count=0

# I am creating the backup directory if it does not exist
# I redirect standard error to my log using the 2> operator to keep things clean
mkdir -p "$BACKUP_DIR" 2> "$ERROR_LOG"

# I use mktemp to create a unique temporary file in /tmp to store the hashes
# I append any stderr messages to my error log using 2>>
HASH_DB=$(mktemp /tmp/hashes.XXXXXX) 2>> "$ERROR_LOG"

# I loop through all objects inside my submissions directory using a wildcard
# I wrap variables in double quotes to handle spaces in filenames safely
for filepath in "$SUB_DIR"/*; do
    
    # I check if the item is a regular file using the -f comparison
    if [ ! -f "$filepath" ]; then
        continue
    fi

    # I increment my count of processed files using integer arithmetic
    processed_count=$((processed_count + 1))

    # I calculate the SHA-256 hash of the file to uniquely identify its content
    # I use cut to extract just the first field (the hash string)
    file_hash=$(sha256sum "$filepath" 2>> "$ERROR_LOG" | cut -d' ' -f1)

    # I use grep to search if this hash already exists in my temporary database
    # The -q option keeps grep quiet so it does not clutter my terminal
    if grep -q "^$file_hash$" "$HASH_DB" 2>> "$ERROR_LOG"; then
        # If grep returns exit status 0 (success), I know it is a duplicate
        duplicate_count=$((duplicate_count + 1))
    else
        # If the hash is new, I append it to my temporary database
        echo "$file_hash" >> "$HASH_DB" 2>> "$ERROR_LOG"
        
        # I copy the unique file to my backup directory
        # I use the -p option with cp to preserve original timestamps and permissions
        cp -p "$filepath" "$BACKUP_DIR/" 2>> "$ERROR_LOG"
        
        # I check the exit status of the copy command using the special variable $?
        if [ $? -eq 0 ]; then
            backup_count=$((backup_count + 1))
        fi
    fi
done

# I generate my formatted report using standard output redirection
{
    echo "=========================================="
    echo "       SUBMISSION PROCESSING REPORT       "
    echo "       Date: $(date +'%Y-%m-%d %H:%M:%S')"
    echo "=========================================="
    echo "Total Files Processed   : $processed_count"
    echo "Duplicate Files Detected: $duplicate_count"
    echo "Unique Files Backed Up  : $backup_count"
    echo "=========================================="
} > "$REPORT_FILE" 2>> "$ERROR_LOG"

# I cleanly remove my temporary database file at the end
rm -f "$HASH_DB" 2>> "$ERROR_LOG"


