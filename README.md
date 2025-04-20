# Finger Command Implementation

A custom implementation of the Unix `finger` command.

## Overview

This project implements a program that behaves like the Unix `finger` command, which displays information about system users. This implementation focuses on local user lookups only and does not use the `exec*` family of functions.

## Features

- Display information about system users in both short and long formats
- Support for querying specific users or all currently logged-in users
- Options to control the display format and content
- Processing of user configuration files like `.plan`, `.project`, and `.pgpkey`
- Respect for user privacy settings

## Command Syntax

```
finger [-lmsp] [user ...] 
```

## Options

- `-s`: Short format display (default when no users are specified)
  - Shows login name, real name, terminal name, write status, idle time, login time, office location, and office phone number
  
- `-l`: Long format display (default when users are specified)
  - Includes all information from short format
  - Adds home directory, home phone number, login shell, mail status
  - Displays contents of `.plan`, `.project`, `.pgpkey`, and `.forward` files
  
- `-p`: Prevents displaying the contents of `.plan`, `.project`, and `.pgpkey` files in long format

- `-m`: Prevents matching on users' real names (matches only on login names)

## Implementation Details

The code is organized into:
- Header file(s) containing declarations of variables, constants, and function prototypes
- C source file(s) containing the implementation of all functions

### Key Components

1. **User Information Retrieval**:
   - Gathering login data from system files
   - Reading user-specific configuration files
   - Determining login status and idle times

2. **Display Formatting**:
   - Phone number formatting based on digit count
   - Time display logic (showing year for old logins)
   - Mail status determination and display

3. **Privacy Management**:
   - Checking for `.nofinger` files
   - Handling write permission status

## Output Format Examples

### Short Format (`-s`)

```
Login     Name            Tty    Idle  Login Time   Office     Phone
jsmith    John Smith      tty1     2d  Apr 15 14:30 Room 123   +1-555-123-4567
```

### Long Format (`-l`)

```
Login: jsmith                           Name: John Smith
Directory: /home/jsmith                 Shell: /bin/bash
Office: Room 123, 555-123-4567          Home Phone: 555-765-4321
Last login Fri Apr 15 14:30 on tty1
Mail last read Wed Apr 13 09:15 2022
No Plan.
```

## Special Cases

- Nonexistent idle and login times are displayed as `*`
- Write permission denial shown as `*` after terminal name in short format
- Write permission denial shown as `(messages off)` in long format
- Phone numbers formatted according to digit count:
  - 11 digits: `+N-NNN-NNN-NNNN`
  - 10 digits: `NNN-NNN-NNNN`
  - 7 digits: `NNN-NNNN`
  - 5 digits: `xN-NNNN`
  - 4 digits: `xNNNN`

## Files Used

- `~/.nofinger`: Indicates user privacy preference
- `~/.plan`: Free-form text displayed in long format
- `~/.project`: Single line of text displayed in long format
- `~/.pgpkey`: PGP key information displayed in long format
- `~/.forward`: Mail forwarding information displayed in long format

## Limitations

- This implementation does not support remote user lookup (`user@host` syntax)
- The `exec*` family of functions is not used as per requirements

## Building and Running

Compile with:

```
gcc -o finger finger.c -Wall
```

Run with:

```
./finger [-lmsp] [user ...]
```

## Examples

1. Show information for all logged-in users in short format:
   ```
   ./finger
   ```

2. Show detailed information about a specific user:
   ```
   ./finger jsmith
   ```

3. Show information about multiple users without their plan/project files:
   ```
   ./finger -p jsmith janedoe
   ```

4. Show short format information for a specific user:
   ```
   ./finger -s jsmith
   ```
