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
