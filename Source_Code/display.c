#include <stdio.h>
#include "clinic.h"

/* Converts elapsed simulation minutes since opening (08:00) into HH:MM format. */
void formatClockTime(int elapsedMinutes, char out[], int outSize) {
    int totalMinutes = CLINIC_OPEN_MINUTES + elapsedMinutes;
    int hours = (totalMinutes / 60) % 24;
    int mins = totalMinutes % 60;
    snprintf(out, (size_t)outSize, "%02d:%02d", hours, mins);
}

/* Maps numeric priority code to a human-readable label. */
void getPriorityLabel(int priority, char out[], int outSize) {
    switch (priority) {
        case PRIORITY_EMERGENCY:
            copyStringSafe(out, "Emergency", outSize);
            break;
        case PRIORITY_URGENT:
            copyStringSafe(out, "Urgent", outSize);
            break;
        case PRIORITY_STANDARD:
            copyStringSafe(out, "Standard", outSize);
            break;
        default:
            copyStringSafe(out, "Unknown", outSize);
            break;
    }
}

/* Maps numeric ticket status code to a human-readable label. */
void getStatusLabel(int status, char out[], int outSize) {
    switch (status) {
        case STATUS_WAITING:
            copyStringSafe(out, "WAITING", outSize);
            break;
        case STATUS_IN_SERVICE:
            copyStringSafe(out, "IN SERVICE", outSize);
            break;
        case STATUS_DONE:
            copyStringSafe(out, "COMPLETED", outSize);
            break;
        case STATUS_CANCELLED:
            copyStringSafe(out, "CANCELLED", outSize);
            break;
        default:
            copyStringSafe(out, "UNKNOWN", outSize);
            break;
    }
}

/* Prints application header banner. */
void displayBanner(void) {
    printf("===================================================================\n");
    printf("          CAMPUS CLINIC PRIORITY QUEUE & TRIAGE SIMULATOR\n");
    printf("     Stations: %d | Capacity: %d Patients | Shift Opens: 08:00\n",
           MAX_STATIONS, MAX_PATIENTS);
    printf("===================================================================\n");
}

/* Prints main interactive menu with current simulation clock and queue count. */
void displayMenu(int currentClock, int waitingCount) {
    char timeStr[16];
    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));

    printf("\n-------------------------------------------------------------------\n");
    printf(" Clinic Clock: %s (+%d min)   |   Patients Waiting: %d\n",
           timeStr, currentClock, waitingCount);
    printf("-------------------------------------------------------------------\n");
    printf("  1. Register New Patient\n");
    printf("  2. Dispatch Waiting Patients to Free Stations\n");
    printf("  3. Advance Simulation Clock (+N Minutes)\n");
    printf("  4. View Live Clinic Dashboard (Stations & Triage Queue)\n");
    printf("  5. Escalate Priority or Cancel Waiting Ticket\n");
    printf("  6. View All Tickets Ledger\n");
    printf("  7. View Shift Analytics & Wait-Time Report\n");
    printf("  8. Load Sample Campus Clinic Batch (5 Demo Patients)\n");
    printf("  0. Exit Simulator\n");
    printf("-------------------------------------------------------------------\n");
}

/* Shows treatment station states and the waiting queue sorted by triage priority. */
void displayDashboard(const int ticketIds[],
                      const char patientNames[][MAX_NAME_LEN],
                      const char symptoms[][MAX_SYMPTOM_LEN],
                      const int priorities[], const int arrivalTimes[],
                      const int serviceDurations[], const int statuses[],
                      const int stationTicketIdx[],
                      const int stationRemaining[], int patientCount,
                      int currentClock) {
    char timeStr[16];
    int order[MAX_PATIENTS];
    int wCount = 0;
    int s = 0;
    int i = 0;
    int j = 0;

    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));
    printf("\n==================== LIVE CLINIC DASHBOARD (%s) ====================\n",
           timeStr);

    printf("\n[ Treatment Stations ]\n");
    printf("%-10s %-12s %-18s %-10s %-12s\n", "Station", "Ticket", "Patient",
           "Priority", "Remaining");
    printf("-------------------------------------------------------------------\n");
    for (s = 0; s < MAX_STATIONS; s++) {
        int idx = stationTicketIdx[s];
        if (idx == -1) {
            printf("Station %-2d %-12s %-18s %-10s %-12s\n", s + 1, "--",
                   "[FREE]", "--", "--");
        } else {
            char prioStr[16];
            getPriorityLabel(priorities[idx], prioStr, (int)sizeof(prioStr));
            printf("Station %-2d #%-11d %-18.18s %-10s %2d min\n", s + 1,
                   ticketIds[idx], patientNames[idx], prioStr,
                   stationRemaining[s]);
        }
    }

    /* Collect waiting indices and sort in dispatch order without mutating master arrays */
    for (i = 0; i < patientCount; i++) {
        if (statuses[i] == STATUS_WAITING) {
            order[wCount] = i;
            wCount++;
        }
    }

    for (i = 0; i < wCount - 1; i++) {
        int minPos = i;
        for (j = i + 1; j < wCount; j++) {
            int a = order[j];
            int b = order[minPos];
            if (priorities[a] < priorities[b]) {
                minPos = j;
            } else if (priorities[a] == priorities[b]) {
                if (arrivalTimes[a] < arrivalTimes[b] ||
                    (arrivalTimes[a] == arrivalTimes[b] &&
                     ticketIds[a] < ticketIds[b])) {
                    minPos = j;
                }
            }
        }
        if (minPos != i) {
            int temp = order[i];
            order[i] = order[minPos];
            order[minPos] = temp;
        }
    }

    printf("\n[ Waiting Queue - Next-to-Be-Seen Order (%d Waiting) ]\n", wCount);
    if (wCount == 0) {
        printf("  (No patients currently waiting.)\n");
    } else {
        printf("%-5s %-7s %-16s %-10s %-7s %-6s %-18s\n", "Pos", "Ticket",
               "Patient", "Priority", "Wait", "Est.", "Symptom");
        printf("-------------------------------------------------------------------\n");
        for (i = 0; i < wCount; i++) {
            int idx = order[i];
            int waitedSoFar = currentClock - arrivalTimes[idx];
            char prioStr[16];
            getPriorityLabel(priorities[idx], prioStr, (int)sizeof(prioStr));
            printf("%-5d #%-6d %-16.16s %-10s %3dm    %3dm   %-18.18s\n",
                   i + 1, ticketIds[idx], patientNames[idx], prioStr,
                   waitedSoFar, serviceDurations[idx], symptoms[idx]);
        }
    }
    printf("===================================================================\n");
}

/* Prints full chronological record of all tickets created during the shift. */
void displayAllTickets(const int ticketIds[],
                       const char patientNames[][MAX_NAME_LEN],
                       const char symptoms[][MAX_SYMPTOM_LEN],
                       const int priorities[], const int arrivalTimes[],
                       const int serviceDurations[], const int waitTimes[],
                       const int statuses[], int patientCount) {
    int i = 0;

    printf("\n======================= ALL TICKETS LEDGER ========================\n");
    if (patientCount == 0) {
        printf("  No patients have registered yet.\n");
        printf("===================================================================\n");
        return;
    }

    printf("%-7s %-15s %-9s %-7s %-5s %-6s %-11s %-14s\n", "Ticket", "Patient",
           "Priority", "Arrived", "Est.", "Wait", "Status", "Symptom");
    printf("-------------------------------------------------------------------\n");

    for (i = 0; i < patientCount; i++) {
        char prioStr[16];
        char statStr[16];
        char arrStr[16];
        char waitStr[12];

        getPriorityLabel(priorities[i], prioStr, (int)sizeof(prioStr));
        getStatusLabel(statuses[i], statStr, (int)sizeof(statStr));
        formatClockTime(arrivalTimes[i], arrStr, (int)sizeof(arrStr));

        if (statuses[i] == STATUS_IN_SERVICE || statuses[i] == STATUS_DONE) {
            snprintf(waitStr, sizeof(waitStr), "%3dm", waitTimes[i]);
        } else {
            copyStringSafe(waitStr, " -- ", (int)sizeof(waitStr));
        }

        printf("#%-6d %-15.15s %-9s %-7s %3dm  %-6s %-11s %-14.14s\n",
               ticketIds[i], patientNames[i], prioStr, arrStr,
               serviceDurations[i], waitStr, statStr, symptoms[i]);
    }
    printf("===================================================================\n");
}

/* Prints shift-level metrics, per-tier wait averages, and station utilization. */
void displayAnalytics(const int priorities[], const int arrivalTimes[],
                      const int waitTimes[], const int statuses[],
                      const int stationTicketIdx[], const int stationServed[],
                      int patientCount, int currentClock, int peakQueue) {
    int waitingCount = 0;
    int inServiceCount = 0;
    int doneCount = 0;
    int cancelledCount = 0;
    int totalDispatched = 0;
    int totalWaitSum = 0;
    int longestWaiting = 0;

    int tierRegistered[3] = {0, 0, 0};
    int tierDispatched[3] = {0, 0, 0};
    int tierWaitSum[3] = {0, 0, 0};
    const char tierNames[3][12] = {"Emergency", "Urgent", "Standard"};

    char timeStr[16];
    int i = 0;
    int t = 0;
    int s = 0;

    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));

    for (i = 0; i < patientCount; i++) {
        int tierIdx = priorities[i] - 1;
        if (tierIdx >= 0 && tierIdx < 3) {
            tierRegistered[tierIdx]++;
        }

        if (statuses[i] == STATUS_WAITING) {
            int currentWait = currentClock - arrivalTimes[i];
            waitingCount++;
            if (currentWait > longestWaiting) {
                longestWaiting = currentWait;
            }
        } else if (statuses[i] == STATUS_IN_SERVICE) {
            inServiceCount++;
            totalDispatched++;
            totalWaitSum += waitTimes[i];
            if (tierIdx >= 0 && tierIdx < 3) {
                tierDispatched[tierIdx]++;
                tierWaitSum[tierIdx] += waitTimes[i];
            }
        } else if (statuses[i] == STATUS_DONE) {
            doneCount++;
            totalDispatched++;
            totalWaitSum += waitTimes[i];
            if (tierIdx >= 0 && tierIdx < 3) {
                tierDispatched[tierIdx]++;
                tierWaitSum[tierIdx] += waitTimes[i];
            }
        } else if (statuses[i] == STATUS_CANCELLED) {
            cancelledCount++;
        }
    }

    printf("\n==================== SHIFT ANALYTICS REPORT =======================\n");
    printf(" Current Clinic Time  : %s (%d elapsed minutes)\n", timeStr,
           currentClock);
    printf(" Total Registered     : %d / %d capacity\n", patientCount,
           MAX_PATIENTS);
    printf(" Status Breakdown     : %d Completed | %d In Service | %d Waiting | %d Cancelled\n",
           doneCount, inServiceCount, waitingCount, cancelledCount);
    printf(" Peak Waiting Queue   : %d patient(s)\n", peakQueue);

    if (totalDispatched > 0) {
        double avgWait = (double)totalWaitSum / (double)totalDispatched;
        printf(" Avg Wait (Dispatched): %.2f min across %d patient(s)\n",
               avgWait, totalDispatched);
    } else {
        printf(" Avg Wait (Dispatched): N/A (no patients dispatched yet)\n");
    }

    if (waitingCount > 0) {
        printf(" Longest Current Wait : %d min (among %d waiting)\n",
               longestWaiting, waitingCount);
    }

    printf("\n[ Wait Time Breakdown by Priority Tier ]\n");
    printf("%-12s %-12s %-12s %-16s\n", "Priority", "Registered", "Dispatched",
           "Avg Wait Time");
    printf("-------------------------------------------------------------------\n");
    for (t = 0; t < 3; t++) {
        if (tierDispatched[t] > 0) {
            double tierAvg = (double)tierWaitSum[t] / (double)tierDispatched[t];
            printf("%-12s %-12d %-12d %.2f min\n", tierNames[t],
                   tierRegistered[t], tierDispatched[t], tierAvg);
        } else {
            printf("%-12s %-12d %-12d N/A\n", tierNames[t], tierRegistered[t],
                   tierDispatched[t]);
        }
    }

    printf("\n[ Treatment Station Utilization ]\n");
    printf("%-12s %-18s %-14s\n", "Station", "Patients Completed",
           "Current State");
    printf("-------------------------------------------------------------------\n");
    for (s = 0; s < MAX_STATIONS; s++) {
        printf("Station %-4d %-18d %-14s\n", s + 1, stationServed[s],
               (stationTicketIdx[s] != -1) ? "Busy" : "Free");
    }
    printf("===================================================================\n");
}
