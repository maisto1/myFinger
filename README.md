# Finger-Like User Info Tool

## Overview

This is a lightweight C program that mimics the basic behavior of the Unix `finger` command. It displays information about local system users, such as login status, last login time, and optional `.plan`, `.project`, or `.forward` files if present.

**Note:** This tool only supports **local user information** — no remote user lookups.

---

## Features

- Lists currently logged-in users  
- Displays info about a specific user  
- Reads optional files in the user's home directory (`.plan`, `.project`, `.forward`)  
- Simple, portable C code with no external dependencies  

---

## Installation

Clone the repository and compile the source using `gcc`:

```bash
git clone https://github.com/your-username/finger-clone.git
cd finger-clone
gcc -o finger main.c

---

## About the Original `finger` Command

The original `finger` command displays information about system users. It supports several options to control the output format:

- `-s`  
  Shows a short summary: login name, real name, terminal, write status (`*` if write is off), idle time, login time, office location, and phone number. If the login time is over six months old, it shows the year instead of the time.

- `-l`  
  Produces a detailed, multi-line format including all `-s` fields plus the user's home directory, shell, mail status, and the contents of `.plan`, `.project`, `.pgpkey`, and `.forward`. Also shows formatted phone numbers and message status.

- `-p`  
  Prevents `finger` from displaying the contents of `.plan`, `.project`, and `.pgpkey` files.

- `-m`  
  Disables matching of real names — only matches login names (case-insensitive).

If no options are provided:
- If usernames are given → defaults to `-l` output.
- If no usernames are given → defaults to `-s` and shows all users currently logged in.

`finger` can also query remote users via `user@host` or just `@host`.

Files used by `finger`:
- `~/.plan`, `~/.project`, `~/.pgpkey`: Shown in long-format output.
- `~/.nofinger`: Hides the user from remote `finger` queries for privacy.

More info: [finger(1) - Linux Man Page](https://linux.die.net/man/1/finger)
