# My Lab Report: Question 5 Solution

In my computer science class, I studied what happens when a system crashes while editing a file in vi
. Because crashes can happen unexpectedly, I evaluated the different recovery tools that vi offers to see which one is the most reliable for getting our unsaved work back

Here is my evaluation of the five recovery mechanisms, followed by my proposed strategy and my personal justification.

---

## Part 1: My Evaluation of the Recovery Mechanisms
### 1. Swap Files (.swp)
**How it works:** When I open a file to edit it, vi automatically creates a hidden swap file with a .swp extension in the same folder
. This file acts like a live temporary backup of my active memory buffer
. If the system crashes, I can open my terminal and type the command vi -r filename (or use the swap file directly like vim -r .filename.swp) to restore my unsaved edits

**My Student Assessment:** This is incredibly useful because vi manages it completely in the background without me having to remember to do anything

### 2. Undo History
**How it works:** vi tracks my editing steps, allowing me to press u to undo a mistake or Ctrl + r to redo it
. In the improved version (vim), I can even undo multiple steps, whereas old-school vi only lets me undo the very last change

**My Student Assessment:** While undo history is amazing for fixing typos while I am actively working, it is kept in the system's volatile RAM
. If the entire computer crashes and loses power, the in-memory undo chain is completely wiped out

### 3. Registers
**How it works:** When I delete or cut text, vi doesn't just throw it away; it stores the text inside numbered registers (from 0 to 9) and named registers (from a to z)
. I can retrieve things from these buffers using paste commands like "1p or "0P

**My Student Assessment:** Registers are a great safety net if I accidentally delete a paragraph
. However, just like undo history, these registers are mostly held in transient memory buffers and are usually lost in a sudden power crash unless they were saved to a special file

### 4. Backup Files
**How it works:** I can tell vi to make a backup copy of my file by adding set backup to my configuration file (~/.vimrc)
. By default, it makes a copy of my file with a ~ symbol at the end (or a .bak extension if I configure it that way)

**My Student Assessment:** This is awesome for keeping older versions of my work, but it has one massive flaw: vi only creates or updates the backup file when I actually save my file using the :w command
. Because my system crashed before I could save, the backup file will only have the old version and won't contain any of the new edits I made right before the crash

### 5. Auto-Recovery (viminfo)
**How it works:** I can configure vi to save my state and recovery history by adding the line set viminfo='1000,<s1000 to my ~/.vimrc file
. When I reboot my computer and reopen the crashed file, vi reads this file and prompts me with an option to recover my unsaved changes

**My Student Assessment:** This is a nice automatic prompt system, but it relies on the configuration being set up perfectly beforehand, and it still uses the swap file behind the scenes to do the actual heavy lifting

---

### Part 2: My Proposed Recovery Strategy and Justification

My Proposed Strategy
If a software developer's system crashes while editing a critical configuration file, the most reliable recovery strategy is to use the Swap File Recovery mechanism

**When the computer turns back on, the developer should run this command:**

**vi -r critical_config.conf**

(Note: I would replace critical_config.conf with the actual name of the configuration file
).
Once the unsaved edits are successfully recovered and displayed on the screen, the developer must save the recovered text immediately by typing :w
. Finally, because vi does not delete swap files automatically after a recovery, the developer should exit vi and manually delete the hidden swap file using rm .critical_config.conf.swp so that the system doesn't keep showing annoying "swap file exists" warnings every time they open the file

**My Justification**
I believe swap files are the absolute best recovery method for these reasons:
**It is fully automatic:** Unlike backup files, which require me to actively save the file using :w, the swap file is constantly and silently updated by vi in the background while I type
. This means it actively captures my newest changes even if I haven't saved in hours

**It survives physical power loss:** Unlike undo history and registers, which exist only in volatile RAM and vanish the second the power cuts out, the swap file is written directly to the hard drive

**It is extremely easy to use:** The developer doesn't need to write any complex scripts. They just need to add the -r flag when opening the file to let vi automatically reconstruct the unsaved file buffer
.