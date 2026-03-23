# 🧠 Linux System Call Tracer

A lightweight and extensible Linux system call tracer built using the `ptrace` API to monitor, filter, and analyze process-level system calls in real time. This project demonstrates low-level system programming concepts and provides insight into how user-space programs interact with the Linux kernel.

---

## 🚀 Features

- 🔍 Real-time system call tracing  
- 🧾 Displays syscall names and return values  
- ⚙️ Basic argument decoding (`read`, `write`, `openat`)  
- 🎯 Syscall filtering using `--filter` option  
- 🧩 Modular and extensible design  
- ⚡ Lightweight with minimal overhead  

---

## 🛠️ Tech Stack

- **Language:** C  
- **Platform:** Linux (x86_64)  
- **Core API:** `ptrace`  
- **Concepts:** System Calls, Process Control, Register Inspection  

---

## ▶️ Getting Started

### 📋 Prerequisites

- Linux-based OS (Ubuntu/Kali/Arch, etc.)
- GCC Compiler installed

---

### 🔧 Build

```bash
make

