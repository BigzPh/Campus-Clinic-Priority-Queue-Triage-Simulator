========================================================================
CS101 | Fundamentals of Programming in C — Final Project
========================================================================

Project Title : Campus Clinic Priority Queue & Triage Simulator
Group Number  : [PLACEHOLDER: Group XX]
Course / Term : CS101 | [PLACEHOLDER: Section / Term / Academic Year]

------------------------------------------------------------------------
GROUP MEMBERS & INDIVIDUAL CONTRIBUTIONS
------------------------------------------------------------------------
1. [PLACEHOLDER: Member 1 Full Name] ([PLACEHOLDER: Student ID])
   - Contribution: [PLACEHOLDER: e.g., Main program flow (main.c), shared
     constants and interfaces (clinic.h), integration testing]

2. [PLACEHOLDER: Member 2 Full Name] ([PLACEHOLDER: Student ID])
   - Contribution: [PLACEHOLDER: e.g., Input validation module
     (input_utils.c), boundary and malformed-input test cases]

3. [PLACEHOLDER: Member 3 Full Name] ([PLACEHOLDER: Student ID])
   - Contribution: [PLACEHOLDER: e.g., Triage priority queue, dispatch
     algorithm, and minute-by-minute clock engine (triage.c)]

4. [PLACEHOLDER: Member 4 Full Name] ([PLACEHOLDER: Student ID])
   - Contribution: [PLACEHOLDER: e.g., Console dashboard, non-mutating
     priority sort, shift analytics report (display.c), documentation]

(Add or remove member entries as needed for your group size.)

------------------------------------------------------------------------
DIRECTORY STRUCTURE
------------------------------------------------------------------------
CS101_[PLACEHOLDER: GroupXX]_ClinicQueue/
  README.txt
  Documentation/
    Project_Documentation.pdf   (and Project_Documentation.md source)
  Presentation/
    Presentation_Outline.md     (use to build Presentation.pptx / .mp4)
  Source_Code/
    clinic.h
    main.c
    input_utils.c
    triage.c
    display.c

------------------------------------------------------------------------
HOW TO COMPILE AND RUN
------------------------------------------------------------------------
Prerequisites: A standard C11 compiler (gcc or clang).

1. Open a terminal and navigate into the Source_Code directory:
     cd Source_Code

2. Compile all source files:
   - macOS / Linux (gcc or clang):
       gcc -Wall -Wextra -std=c11 main.c input_utils.c triage.c display.c -o clinic_sim

   - Windows (MinGW / GCC):
       gcc -Wall -Wextra -std=c11 main.c input_utils.c triage.c display.c -o clinic_sim.exe

3. Run the application:
   - macOS / Linux:
       ./clinic_sim

   - Windows:
       clinic_sim.exe

------------------------------------------------------------------------
QUICK DEMO TIP
------------------------------------------------------------------------
Inside the main menu, select Option 8 ("Load Sample Campus Clinic Batch")
to populate 5 realistic triage cases immediately, then test:
  - Option 4: View sorted waiting queue and treatment stations
  - Option 5: Escalate a patient's priority or cancel a waiting ticket
  - Option 2: Dispatch highest-priority patients to the 3 stations
  - Option 3: Advance simulation clock (e.g., +10 minutes)
  - Option 7: View Shift Analytics & Wait-Time Report

------------------------------------------------------------------------
BEFORE CREATING FINAL SUBMISSION ZIP
------------------------------------------------------------------------
1. Replace all [PLACEHOLDER: ...] entries in this README.txt,
   Documentation/Project_Documentation.md (then regenerate PDF), and
   Presentation/.
2. Export your slides or video to Presentation/Presentation.pptx (or .mp4).
3. Remove any compiled executables (clinic_sim / clinic_sim.exe) from
   Source_Code/ if your instructor requires source-only submission.
4. Package into: CS101_<GroupNumber>_ClinicQueue.zip
========================================================================
