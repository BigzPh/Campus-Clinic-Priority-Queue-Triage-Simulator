# Presentation Outline — Campus Clinic Priority Queue & Triage Simulator
**Total Duration:** 10 Minutes Maximum  
**Group:** `[PLACEHOLDER: Group XX]`  
**Final Deliverable Target:** Export slides to `Presentation/Presentation.pptx` (or record `Presentation/Presentation.mp4`)

---

## Segment Breakdown & Speaker Plan (10 Minutes Total)

### 1. Introduction (2 Minutes) — `[PLACEHOLDER: Member 1]`
* **Slide 1 — Title & Team:** Project title, group number, and member roles.
* **Slide 2 — Problem & Purpose:** Why campus clinics cannot rely on simple first-come, first-served queues—an asthma flare-up or severe injury must be treated before routine clearance forms, while keeping fairness within each urgency tier.
* **Slide 3 — Objectives & Scope:** Simulates 3 treatment stations, up to 50 shift tickets (`08:00` open), urgency triage (`1=Emergency`, `2=Urgent`, `3=Standard`), and wait-time analytics using core CS101 C fundamentals.

### 2. Live Demonstration (3 Minutes) — `[PLACEHOLDER: Member 2]`
Follow this fast, high-impact walkthrough (`./clinic_sim`):
1. **Input Validation (20s):** Choose Option `1`, briefly enter an invalid menu choice (e.g., `abc`) to show safe recovery, then register 1 live patient (`Urgent`, `8 min`).
2. **Sample Batch + Triage Queue (30s):** Choose Option `8` to load 5 sample cases, then Option `4` to show how the waiting queue automatically displays `Emergency` ahead of `Urgent` and `Standard`.
3. **Escalate & Cancel (35s):** Choose Option `5`: escalate `#101` from `Urgent` to `Emergency`, then cancel `#106`.
4. **Dispatch & Minute-by-Minute Simulation (55s):** Choose Option `2` to fill all 3 treatment stations, then Option `3` to advance `+10` minutes—point out `#101` finishing at `08:08` and `#105` immediately auto-dispatching with an exact `8 min` wait.
5. **Ledger & Analytics (40s):** Show Option `6` (All Tickets Ledger) and Option `7` (Shift Analytics: per-tier average wait times, peak queue size, station utilization).

### 3. Technical Explanation (3 Minutes) — `[PLACEHOLDER: Member 3 & Member 4]`
* **Slide 4 — Modular Architecture (`Source_Code/`):** Show how responsibilities split across `clinic.h`, `main.c`, `input_utils.c`, `triage.c`, and `display.c`.
* **Slide 5 — Parallel & 2D Arrays:** Explain how synchronized arrays (`ticketIds[]`, `patientNames[][]`, `priorities[]`, `arrivalTimes[]`, `statuses[]`, `stationTicketIdx[]`) model records cleanly without `struct`s or pointers, and how `STATUS_CANCELLED` keeps `ticketId - 101` indexing intact.
* **Slide 6 — Key Algorithms & Mandatory Concepts:**
  * `findNextWaitingIndex()` — compound relational/logical priority + FIFO tie-breaker.
  * `displayDashboard()` — non-mutating selection sort on a local `order[]` index array.
  * `advanceClock()` — minute-by-minute loop preventing wait-time distortion.
  * `readIntInRange()` — `fgets()` line-buffered validation replacing unsafe `scanf()`.

### 4. Conclusion (2 Minutes) — `[PLACEHOLDER: Member 4 / All]`
* **Slide 7 — Challenges & Lessons Learned:** Eliminating `scanf` newline/text bugs, handling exact timestamps across multi-minute jumps, and separating UI/validation/engine cleanly.
* **Slide 8 — Results, Limitations & Future Improvements:** Zero-warning C11 build, in-memory shift scope (`50` max, `3` stations), future upgrades (`FILE *` CSV export, `struct Patient`), and Q&A readiness.
