# 🏗️ Reconstruction Simulation – SPL Assignment 1

This project is a C++ simulation of post-war reconstruction planning in a fictional land called SPLand.  
It was developed as part of the Systems Programming Language (SPL) course at Ben-Gurion University.

## 🎯 Goals
- Apply object-oriented programming in C++
- Practice memory-safe design using the **Rule of Five**
- Simulate reconstruction plans with dynamic strategies
- Work with configuration parsing, command-line interaction, and custom actions

## 🧠 Key Components
- **Simulation** – Manages settlements, plans, facilities, and user actions
- **Settlement** – Village / City / Metropolis with different construction limits
- **Plan** – Represents reconstruction logic using a defined **SelectionPolicy**
- **Selection Policies**:
  - Naive
  - Balanced
  - Economy-focused
  - Sustainability-focused
- **Actions** – Text-based commands like `step`, `plan`, `facility`, `changePolicy`, `log`, `close`

## ⚙️ How to Run

1. Compile the project:
```bash
make
```

2. Run with configuration:
```bash
./bin/simulation example_config.txt
```

3. Example actions:
```bash
settlement Kfar_SPL 0
facility kindergarten 0 3 3 2 1
plan Kfar_SPL eco
step 3
log
close
```

## 💡 Notes
- No memory leaks (validated with `valgrind`)
- Designed to run and compile on CS Lab UNIX machines only
- Submission includes full makefile and folder structure
