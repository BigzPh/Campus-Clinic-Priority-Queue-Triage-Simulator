# CS101 Final Project Documentation
**Project Title:** Campus Clinic Priority Queue & Triage Simulator  
**Group:** `[PLACEHOLDER: Group XX]`  
**Members:** `[PLACEHOLDER: Member 1, Member 2, Member 3, Member 4]`  

---

## 1. Project Overview

### Title & Description
The **Campus Clinic Priority Queue & Triage Simulator** is a console-based C application that models patient intake, medical urgency triage, treatment station assignment, and wait-time analytics for a campus health clinic.

Unlike a basic first-come, first-served queue, clinic receptionists must prioritize urgent and emergency cases while still treating patients fairly within the same urgency level. This program simulates that triage workflow minute by minute across **3 treatment stations** (`MAX_STATIONS = 3`) and up to **50 patient tickets per shift** (`MAX_PATIENTS = 50`), starting at `08:00` (`CLINIC_OPEN_MINUTES = 480`).

### Intended Users
- **Campus clinic staff / triage coordinators** testing queue policies and station throughput.
- **CS101 students and instructors** studying how priority queues, multi-station scheduling, and state-driven simulations can be built cleanly using fundamental C programming constructs.

### Purpose
To provide an accurate, interactive management simulator that demonstrates how urgency-based triage reduces emergency wait times compared to plain arrival order, while tracking measurable performance metrics (average wait time per priority tier, peak queue size, and station utilization).

### Key Features
1. **Patient Registration with Strict Validation:** Captures patient name, chief complaint/symptom, urgency tier (`1 = Emergency`, `2 = Urgent`, `3 = Standard`), and estimated treatment duration (`1–120` min), assigning sequential ticket numbers starting at `#101`.
2. **Priority + FIFO Dispatch Engine:** Assigns waiting patients to available treatment stations by choosing the lowest priority number (`1` before `2` before `3`), breaking ties by earliest arrival time and ticket ID.
3. **Minute-by-Minute Clock Simulation:** Advancing the clock by `N` minutes (`1–120`) steps forward one minute at a time so stations finish treatments at exact timestamps and immediately auto-dispatch the next highest-priority waiting patient.
4. **Live Clinic Dashboard:** Displays the current clinic time (`HH:MM`), active station status, and the waiting queue sorted in dispatch order without altering the underlying chronological arrays.
5. **Priority Escalation & Ticket Cancellation:** Allows updating a waiting patient's priority if symptoms worsen or cancelling a ticket if a patient leaves before being seen.
6. **All-Tickets Ledger & Shift Analytics:** Reports status breakdowns, peak waiting queue length, overall and per-tier average wait times, longest current wait, and patients completed per station.
7. **Built-in Demo Batch Loader:** Option 8 inserts 5 realistic campus clinic cases at the current clock time for rapid testing and live demonstration.

---

## 2. Objectives

### General Objective
Design and implement a reliable, modular, console-based priority triage simulator in C that meaningfully applies all seven mandatory CS101 programming concepts while producing accurate, verifiable simulation metrics.

### Specific Goals
1. **Accurate Triage Scheduling:** Guarantee that Emergency (`1`), Urgent (`2`), and Standard (`3`) patients are dispatched in strict priority order with FIFO tie-breaking.
2. **Exact Minute-Level Time Tracking:** Calculate wait times (`dispatchClock - arrivalClock`) and treatment completion times accurately even when advancing the simulation clock by multi-minute intervals.
3. **Resilient Input Validation:** Prevent crashes, buffer overflows, array-bounds violations, and infinite input loops on invalid text, out-of-range numbers, blank lines, or non-numeric strings.
4. **Clear Modular Architecture:** Separate responsibilities cleanly across `clinic.h`, `main.c`, `input_utils.c`, `triage.c`, and `display.c` for readability and team collaboration.
5. **Actionable Shift Reporting:** Compute per-priority wait averages and station utilization with zero-division safeguards when no patients have been dispatched yet.

---

## 3. User Guide

### Compilation Instructions
Ensure a C11-compatible compiler (`gcc` or `clang`) is installed.

1. Open a terminal in the project root and move to `Source_Code/`:
   ```bash
   cd Source_Code
   ```
2. Compile all source files:
   ```bash
   gcc -Wall -Wextra -std=c11 main.c input_utils.c triage.c display.c -o clinic_sim
   ```
   *(On Windows MinGW, replace `-o clinic_sim` with `-o clinic_sim.exe`.)*

### Launching & Operating the Program
Run the compiled binary:
```bash
./clinic_sim
```
*(On Windows: `clinic_sim.exe`)*

From the main menu (`0–8`):
- **1 — Register New Patient:** Enter patient name (max 39 chars), symptom (max 49 chars), priority (`1–3`), and estimated duration (`1–120` min).
- **2 — Dispatch Waiting Patients:** Assigns waiting patients in triage order to any free stations at the current clock time.
- **3 — Advance Simulation Clock:** Enter `1–120` minutes to step treatments forward, mark completed tickets, and auto-dispatch waiting patients as stations become free.
- **4 — View Live Clinic Dashboard:** Displays the 3 stations and the waiting queue ordered by triage priority.
- **5 — Escalate Priority or Cancel Ticket:** Select any `WAITING` ticket ID to change its priority or cancel it.
- **6 — View All Tickets Ledger:** Displays every ticket from `#101` onward with arrival, wait time, and status.
- **7 — View Shift Analytics:** Shows status totals, peak queue count, overall/per-tier wait averages, and station counts.
- **8 — Load Sample Campus Clinic Batch:** Adds 5 preset patients for quick testing or demos.
- **0 — Exit Simulator:** Exits cleanly and prints the final shift analytics summary if patients were registered.

### Sample Program Output
Below is an actual captured session demonstrating input validation, loading the sample batch, escalating `#101` to Emergency, cancelling `#106`, dispatching stations, advancing `+10` minutes, and viewing the Shift Analytics report:

```text
===================================================================
          CAMPUS CLINIC PRIORITY QUEUE & TRIAGE SIMULATOR
     Stations: 3 | Capacity: 50 Patients | Shift Opens: 08:00
===================================================================

-------------------------------------------------------------------
 Clinic Clock: 08:00 (+0 min)   |   Patients Waiting: 0
-------------------------------------------------------------------
  1. Register New Patient
  2. Dispatch Waiting Patients to Free Stations
  3. Advance Simulation Clock (+N Minutes)
  4. View Live Clinic Dashboard (Stations & Triage Queue)
  5. Escalate Priority or Cancel Waiting Ticket
  6. View All Tickets Ledger
  7. View Shift Analytics & Wait-Time Report
  8. Load Sample Campus Clinic Batch (5 Demo Patients)
  0. Exit Simulator
-------------------------------------------------------------------
Select an option (0-8): abc
  [!] Invalid input. Digits only, between 0 and 8.
Select an option (0-8): 1

--- Register New Patient ---
Patient full name:   Elena Cruz  
Chief complaint / symptom:   Mild dizziness  
Priority Levels: 1 = Emergency | 2 = Urgent | 3 = Standard
Select priority (1-3): 2
Estimated service duration (1-120 min): 8

[+] Registered Ticket #101 for Elena Cruz (Urgent) at 08:00 | Est: 8 min.

... (after Option 8 loads #102-#106, Option 5 escalates #101 -> Emergency and cancels #106) ...

--- Dispatching Waiting Patients ---
  [08:00] Dispatched #101 (Elena Cruz | Emergency) -> Station 1 (Waited: 0 min, Est: 8 min)
  [08:00] Dispatched #103 (Maya Lin | Emergency) -> Station 2 (Waited: 0 min, Est: 15 min)
  [08:00] Dispatched #102 (Alex Rivera | Urgent) -> Station 3 (Waited: 0 min, Est: 12 min)

Enter minutes to advance clock (1-120): 10

--- Advancing Clock by 10 Minute(s) ---
  [08:08] Station 1 finished Ticket #101 (Elena Cruz).
  [08:08] Dispatched #105 (Samira Gomez | Urgent) -> Station 1 (Waited: 8 min, Est: 10 min)

==================== SHIFT ANALYTICS REPORT =======================
 Current Clinic Time  : 08:10 (10 elapsed minutes)
 Total Registered     : 6 / 50 capacity
 Status Breakdown     : 1 Completed | 3 In Service | 1 Waiting | 1 Cancelled
 Peak Waiting Queue   : 6 patient(s)
 Avg Wait (Dispatched): 2.00 min across 4 patient(s)
 Longest Current Wait : 10 min (among 1 waiting)

[ Wait Time Breakdown by Priority Tier ]
Priority     Registered   Dispatched   Avg Wait Time   
-------------------------------------------------------------------
Emergency    2            2            0.00 min
Urgent       2            2            4.00 min
Standard     2            0            N/A

[ Treatment Station Utilization ]
Station      Patients Completed Current State 
-------------------------------------------------------------------
Station 1    1                  Busy          
Station 2    0                  Busy          
Station 3    0                  Busy          
===================================================================
```

---

## 4. Decisions Made

### Design Decisions & Algorithms
1. **Parallel 1D & 2D Arrays Instead of `struct`s:** Patient records are stored across synchronized parallel arrays (`ticketIds[]`, `patientNames[][]`, `symptoms[][]`, `priorities[]`, `arrivalTimes[]`, `serviceDurations[]`, `waitTimes[]`, `statuses[]`) declared in `main.c` and passed into functions. Keeping ticket records parallel allows sequential O(1) Ticket ID lookup (`idx = ticketId - BASE_TICKET_ID`) because cancelled tickets are marked `STATUS_CANCELLED` instead of being deleted by shifting arrays.
2. **Non-Mutating Dashboard Sort (Index Array):** Rather than sorting the master parallel arrays—which would break `ticketId - BASE_TICKET_ID` indexing—`displayDashboard()` fills a temporary `int order[MAX_PATIENTS]` array with indices of `STATUS_WAITING` patients and sorts only `order[]` using selection sort by `(priority, arrivalTime, ticketId)`.
3. **Minute-by-Minute Clock Stepping:** When advancing the clock by `minutes`, `advanceClock()` loops `1..minutes` one minute at a time rather than subtracting `minutes` in one jump. This ensures that if Station 1 finishes at minute `8` of a `10`-minute jump, the next patient starts immediately at minute `8` with exact wait-time accounting.
4. **Line-Buffered Input Validation (`fgets` + Manual Digit Parser):** Instead of `scanf("%d")`, `readIntInRange()` reads a full line with `fgets()`, trims whitespace, verifies every character is a digit (handling optional `+`/`-` and overflow guards), and enforces `[minVal, maxVal]`. This completely eliminates leftover newline bugs and infinite loops on text input like `"abc"`.

### Application of Mandatory C Concepts
- **Data types & variables:** `int` for IDs, priorities, statuses, and clock minutes; `double` for average wait-time calculations; `char` arrays for names, symptoms, and formatted `HH:MM` clock strings.
- **Operators & expressions:** Arithmetic (`totalMinutes / 60`, `totalMinutes % 60`, `currentClock - arrivalTimes[i]`, `(double)totalWaitSum / totalDispatched`), relational (`<`, `==`, `>=`), and compound logical expressions (`&&`, `||`) in priority tie-breaking and validation.
- **Input & output:** Interactive menu prompts via `printf`, line-buffered input via `fgets`, and column-aligned console reports (`%-15.15s`, `%02d:%02d`, `%.2f`).
- **Conditional statements:** `switch` statements in `main.c` (menu dispatch) and `display.c` (`getPriorityLabel`, `getStatusLabel`); `if/else` chains for priority selection, station state checks, and zero-division guards in `displayAnalytics()`.
- **Looping structures:** `do-while` main program loop in `main()`, `while` loops for input re-prompting and string trimming, and `for` loops for station scans, selection sort, minute-by-minute simulation ticks, and analytics aggregation.
- **Arrays:** Parallel 1D arrays (`MAX_PATIENTS`), 2D `char` arrays (`patientNames[MAX_PATIENTS][MAX_NAME_LEN]`, `symptoms[MAX_PATIENTS][MAX_SYMPTOM_LEN]`), station arrays (`MAX_STATIONS`), local sorting index array (`order[]`), and 3-element tier summary arrays (`tierRegistered[3]`, `tierDispatched[3]`, `tierWaitSum[3]`).
- **User-defined functions:** Modularized across `input_utils.c`, `triage.c`, and `display.c` with shared declarations in `clinic.h`.

### Optional Advanced Concepts
No optional advanced concepts (`struct`, pointer parameters, dynamic memory allocation, file handling, or graphics libraries) were used. The group intentionally stayed within the seven mandatory C fundamentals so the entire codebase remains concise, accessible to every member, and aligned with CS101 core learning outcomes.

### Challenges & Solutions
- **Challenge 1 — `scanf` infinite loop on non-numeric input:** When testing text input on numeric menus, `scanf("%d")` leaves invalid characters in `stdin`. **Solution:** Implemented `readIntInRange()` using `fgets()` plus a manual character-by-character parser and `flushInputLine()` for oversized lines.
- **Challenge 2 — Wait-time distortion on large clock jumps:** Jumping `+10` minutes in one step would overcount wait time for patients who should enter a newly freed station mid-interval. **Solution:** Stepped `advanceClock()` 1 minute per iteration and invoked `dispatchWaitingPatients()` at each tick.
- **Challenge 3 — Preserving cancelled history without shifting arrays:** Deleting cancelled tickets would shift array indices and invalidate ticket IDs. **Solution:** Introduced `STATUS_CANCELLED` so cancelled entries stay auditable in the ledger and analytics without displacing existing indices.

### AI Assistance Disclosure
AI assistance (Google Antigravity / Gemini) was used to help brainstorm project concepts aligned with the CS101 rubric, structure the modular C architecture, draft initial source code and documentation templates, and design validation test cases. All code, compilation outputs, and calculations were reviewed, compiled, and verified by the group (`[PLACEHOLDER: verify final review by members]`).

---

## 5. Conclusion

### Results
The **Campus Clinic Priority Queue & Triage Simulator** compiles cleanly with zero warnings under strict C11 flags (`gcc -Wall -Wextra -Werror -pedantic -std=c11`) and passes functional tests covering priority ordering, FIFO tie-breaking, escalation/cancellation, exact minute-by-minute dispatch math, capacity limits (`50`), and invalid-input recovery.

### Limitations
1. **In-Memory Shift State:** Data resets when the program exits since persistent file storage (`FILE *`) was intentionally omitted to stay strictly within the seven mandatory concepts.
2. **Fixed Compile-Time Capacity & Station Count:** `MAX_PATIENTS` (`50`) and `MAX_STATIONS` (`3`) are compile-time constants defined in `clinic.h`.
3. **Single-Shift Clock:** Simulation assumes a single shift starting at `08:00` without multi-day calendar dates.

### Lessons Learned
- Designing clean parallel array contracts and status state machines (`WAITING -> IN SERVICE -> COMPLETED` or `CANCELLED`) makes complex scheduling logic achievable without pointers or structs.
- Separating input validation, simulation rules, and console rendering across separate `.c` files prevents spaghetti code and simplifies testing and presentation division.

### Possible Future Improvements
- Export end-of-shift ledgers and analytics to CSV/TXT files using `FILE *`.
- Refactor parallel patient arrays into a `struct Patient` array for grouped record handling.
- Add configurable station counts at startup and automated Poisson/random arrival simulation modes.
