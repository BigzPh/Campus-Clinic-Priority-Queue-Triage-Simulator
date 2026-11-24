#include <stdio.h>
#include "clinic.h"

/* Initializes all treatment stations and patient status/wait arrays. */
void initSimulation(int stationTicketIdx[], int stationRemaining[],
                    int stationServed[], int waitTimes[], int statuses[]) {
    int i = 0;
    for (i = 0; i < MAX_STATIONS; i++) {
        stationTicketIdx[i] = -1;
        stationRemaining[i] = 0;
        stationServed[i] = 0;
    }
    for (i = 0; i < MAX_PATIENTS; i++) {
        waitTimes[i] = 0;
        statuses[i] = STATUS_WAITING;
    }
}

/* Counts how many registered patients currently have targetStatus. */
int countByStatus(const int statuses[], int patientCount, int targetStatus) {
    int count = 0;
    int i = 0;
    for (i = 0; i < patientCount; i++) {
        if (statuses[i] == targetStatus) {
            count++;
        }
    }
    return count;
}

/* Returns the updated peak waiting-queue length. */
int updatePeakQueue(const int statuses[], int patientCount, int currentPeak) {
    int waitingNow = countByStatus(statuses, patientCount, STATUS_WAITING);
    if (waitingNow > currentPeak) {
        return waitingNow;
    }
    return currentPeak;
}

/* Registers a new patient after validating all inputs. Returns updated patientCount. */
int registerPatient(int ticketIds[], char patientNames[][MAX_NAME_LEN],
                    char symptoms[][MAX_SYMPTOM_LEN], int priorities[],
                    int arrivalTimes[], int serviceDurations[],
                    int waitTimes[], int statuses[], int patientCount,
                    int currentClock) {
    char tempName[MAX_NAME_LEN];
    char tempSymptom[MAX_SYMPTOM_LEN];
    char timeStr[16];
    char prioStr[16];
    int priority = 0;
    int duration = 0;

    if (patientCount >= MAX_PATIENTS) {
        printf("\n[!] Clinic queue capacity (%d patients) reached for this shift.\n",
               MAX_PATIENTS);
        return patientCount;
    }

    printf("\n--- Register New Patient ---\n");
    if (!readNonEmptyString("Patient full name: ", tempName, MAX_NAME_LEN)) {
        return patientCount;
    }
    if (!readNonEmptyString("Chief complaint / symptom: ", tempSymptom,
                            MAX_SYMPTOM_LEN)) {
        return patientCount;
    }

    printf("Priority Levels: 1 = Emergency | 2 = Urgent | 3 = Standard\n");
    priority = readIntInRange("Select priority (1-3): ", PRIORITY_EMERGENCY,
                              PRIORITY_STANDARD);
    if (priority == INPUT_EOF) {
        return patientCount;
    }

    duration = readIntInRange("Estimated service duration (1-120 min): ", 1, 120);
    if (duration == INPUT_EOF) {
        return patientCount;
    }

    ticketIds[patientCount] = BASE_TICKET_ID + patientCount;
    copyStringSafe(patientNames[patientCount], tempName, MAX_NAME_LEN);
    copyStringSafe(symptoms[patientCount], tempSymptom, MAX_SYMPTOM_LEN);
    priorities[patientCount] = priority;
    arrivalTimes[patientCount] = currentClock;
    serviceDurations[patientCount] = duration;
    waitTimes[patientCount] = 0;
    statuses[patientCount] = STATUS_WAITING;

    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));
    getPriorityLabel(priority, prioStr, (int)sizeof(prioStr));
    printf("\n[+] Registered Ticket #%d for %s (%s) at %s | Est: %d min.\n",
           ticketIds[patientCount], patientNames[patientCount], prioStr,
           timeStr, duration);

    return patientCount + 1;
}

/* Loads a 5-patient sample batch for quick testing and live demonstrations. */
int loadDemoBatch(int ticketIds[], char patientNames[][MAX_NAME_LEN],
                  char symptoms[][MAX_SYMPTOM_LEN], int priorities[],
                  int arrivalTimes[], int serviceDurations[],
                  int waitTimes[], int statuses[], int patientCount,
                  int currentClock) {
    const char sampleNames[5][MAX_NAME_LEN] = {
        "Alex Rivera",
        "Maya Lin",
        "Jordan Patel",
        "Samira Gomez",
        "Noah Kim"
    };
    const char sampleSymptoms[5][MAX_SYMPTOM_LEN] = {
        "Sprained ankle during PE",
        "Severe asthma flare-up",
        "Routine clearance form",
        "High fever and migraine",
        "Prescription refill check"
    };
    const int samplePriorities[5] = {
        PRIORITY_URGENT,
        PRIORITY_EMERGENCY,
        PRIORITY_STANDARD,
        PRIORITY_URGENT,
        PRIORITY_STANDARD
    };
    const int sampleDurations[5] = {12, 15, 6, 10, 5};

    char timeStr[16];
    char prioStr[16];
    int loaded = 0;
    int i = 0;

    if (patientCount >= MAX_PATIENTS) {
        printf("\n[!] Queue is already full (%d/%d).\n", patientCount,
               MAX_PATIENTS);
        return patientCount;
    }

    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));
    printf("\n--- Loading Sample Campus Clinic Batch at %s ---\n", timeStr);

    for (i = 0; i < 5 && patientCount < MAX_PATIENTS; i++) {
        ticketIds[patientCount] = BASE_TICKET_ID + patientCount;
        copyStringSafe(patientNames[patientCount], sampleNames[i],
                       MAX_NAME_LEN);
        copyStringSafe(symptoms[patientCount], sampleSymptoms[i],
                       MAX_SYMPTOM_LEN);
        priorities[patientCount] = samplePriorities[i];
        arrivalTimes[patientCount] = currentClock;
        serviceDurations[patientCount] = sampleDurations[i];
        waitTimes[patientCount] = 0;
        statuses[patientCount] = STATUS_WAITING;

        getPriorityLabel(priorities[patientCount], prioStr,
                         (int)sizeof(prioStr));
        printf("  [+] #%d | %-14s | %-9s | %2d min | %s\n",
               ticketIds[patientCount], patientNames[patientCount], prioStr,
               serviceDurations[patientCount], symptoms[patientCount]);

        patientCount++;
        loaded++;
    }

    printf("Loaded %d sample patient(s) into the waiting queue.\n", loaded);
    return patientCount;
}

/* Finds the index of the highest-priority waiting patient (ties break FIFO). */
int findNextWaitingIndex(const int priorities[], const int arrivalTimes[],
                         const int ticketIds[], const int statuses[],
                         int patientCount) {
    int bestIdx = -1;
    int i = 0;

    for (i = 0; i < patientCount; i++) {
        if (statuses[i] != STATUS_WAITING) {
            continue;
        }
        if (bestIdx == -1) {
            bestIdx = i;
        } else if (priorities[i] < priorities[bestIdx]) {
            bestIdx = i;
        } else if (priorities[i] == priorities[bestIdx]) {
            if (arrivalTimes[i] < arrivalTimes[bestIdx] ||
                (arrivalTimes[i] == arrivalTimes[bestIdx] &&
                 ticketIds[i] < ticketIds[bestIdx])) {
                bestIdx = i;
            }
        }
    }

    return bestIdx;
}

/* Assigns waiting patients to all currently free stations by triage priority. */
int dispatchWaitingPatients(const int ticketIds[],
                            const char patientNames[][MAX_NAME_LEN],
                            const int priorities[], const int arrivalTimes[],
                            const int serviceDurations[], int waitTimes[],
                            int statuses[], int stationTicketIdx[],
                            int stationRemaining[], int patientCount,
                            int currentClock) {
    int dispatched = 0;
    int s = 0;
    char timeStr[16];
    char prioStr[16];

    formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));

    for (s = 0; s < MAX_STATIONS; s++) {
        int nextIdx = -1;
        if (stationTicketIdx[s] != -1) {
            continue;
        }

        nextIdx = findNextWaitingIndex(priorities, arrivalTimes, ticketIds,
                                       statuses, patientCount);
        if (nextIdx == -1) {
            break;
        }

        stationTicketIdx[s] = nextIdx;
        stationRemaining[s] = serviceDurations[nextIdx];
        statuses[nextIdx] = STATUS_IN_SERVICE;
        waitTimes[nextIdx] = currentClock - arrivalTimes[nextIdx];
        dispatched++;

        getPriorityLabel(priorities[nextIdx], prioStr, (int)sizeof(prioStr));
        printf("  [%s] Dispatched #%d (%s | %s) -> Station %d "
               "(Waited: %d min, Est: %d min)\n",
               timeStr, ticketIds[nextIdx], patientNames[nextIdx], prioStr,
               s + 1, waitTimes[nextIdx], stationRemaining[s]);
    }

    return dispatched;
}

/* Advances the clinic clock minute-by-minute, completing & auto-dispatching tickets. */
int advanceClock(const int ticketIds[], const char patientNames[][MAX_NAME_LEN],
                 const int priorities[], const int arrivalTimes[],
                 const int serviceDurations[], int waitTimes[], int statuses[],
                 int stationTicketIdx[], int stationRemaining[],
                 int stationServed[], int patientCount, int currentClock,
                 int minutes) {
    int step = 0;
    int s = 0;
    int completedCount = 0;
    int autoDispatched = 0;
    char timeStr[16];

    printf("\n--- Advancing Clock by %d Minute(s) ---\n", minutes);

    for (step = 1; step <= minutes; step++) {
        currentClock++;
        formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));

        for (s = 0; s < MAX_STATIONS; s++) {
            int pIdx = stationTicketIdx[s];
            if (pIdx != -1) {
                stationRemaining[s]--;
                if (stationRemaining[s] <= 0) {
                    statuses[pIdx] = STATUS_DONE;
                    stationServed[s]++;
                    completedCount++;
                    printf("  [%s] Station %d finished Ticket #%d (%s).\n",
                           timeStr, s + 1, ticketIds[pIdx],
                           patientNames[pIdx]);
                    stationTicketIdx[s] = -1;
                    stationRemaining[s] = 0;
                }
            }
        }

        /* Auto-assign newly freed or idle stations if patients are waiting */
        autoDispatched += dispatchWaitingPatients(
            ticketIds, patientNames, priorities, arrivalTimes, serviceDurations,
            waitTimes, statuses, stationTicketIdx, stationRemaining,
            patientCount, currentClock);
    }

    if (completedCount == 0 && autoDispatched == 0) {
        formatClockTime(currentClock, timeStr, (int)sizeof(timeStr));
        printf("  [%s] No status transitions occurred during this interval.\n",
               timeStr);
    }

    return currentClock;
}

/* Allows escalating/changing priority or cancelling a waiting patient's ticket. */
int manageWaitingTicket(const int ticketIds[],
                        const char patientNames[][MAX_NAME_LEN],
                        int priorities[], const int arrivalTimes[],
                        int statuses[], int patientCount) {
    int waitingCount = countByStatus(statuses, patientCount, STATUS_WAITING);
    int i = 0;
    int chosenId = 0;
    int foundIdx = -1;
    int action = 0;

    if (waitingCount == 0) {
        printf("\n[!] There are currently no WAITING patients to modify.\n");
        return 0;
    }

    printf("\n--- Escalate Priority or Cancel Waiting Ticket ---\n");
    printf("%-8s %-20s %-11s %-8s\n", "Ticket", "Patient", "Priority",
           "Arrived");
    printf("---------------------------------------------------\n");
    for (i = 0; i < patientCount; i++) {
        if (statuses[i] == STATUS_WAITING) {
            char prioStr[16];
            char timeStr[16];
            getPriorityLabel(priorities[i], prioStr, (int)sizeof(prioStr));
            formatClockTime(arrivalTimes[i], timeStr, (int)sizeof(timeStr));
            printf("#%-7d %-20.20s %-11s %-8s\n", ticketIds[i],
                   patientNames[i], prioStr, timeStr);
        }
    }

    while (1) {
        int maxTicketId = BASE_TICKET_ID + patientCount - 1;
        chosenId = readIntInRange("\nEnter Ticket ID to manage (0 to cancel): ",
                                  0, maxTicketId);
        if (chosenId == INPUT_EOF || chosenId == 0) {
            return 0;
        }
        if (chosenId < BASE_TICKET_ID) {
            printf("  [!] Ticket #%d does not exist. IDs start at #%d.\n",
                   chosenId, BASE_TICKET_ID);
            continue;
        }

        foundIdx = chosenId - BASE_TICKET_ID;
        if (statuses[foundIdx] != STATUS_WAITING) {
            char statStr[16];
            getStatusLabel(statuses[foundIdx], statStr, (int)sizeof(statStr));
            printf("  [!] Ticket #%d (%s) is '%s' and cannot be modified.\n",
                   chosenId, patientNames[foundIdx], statStr);
            continue;
        }
        break;
    }

    printf("\nSelected Ticket #%d (%s)\n", ticketIds[foundIdx],
           patientNames[foundIdx]);
    printf("  1. Change Priority (Escalate / Downgrade)\n");
    printf("  2. Cancel Ticket (Patient Left)\n");
    printf("  0. Back\n");
    action = readIntInRange("Choose action (0-2): ", 0, 2);

    if (action == 1) {
        int newPrio = readIntInRange(
            "Enter new priority (1=Emergency, 2=Urgent, 3=Standard): ",
            PRIORITY_EMERGENCY, PRIORITY_STANDARD);
        if (newPrio == INPUT_EOF) {
            return 0;
        }
        if (newPrio == priorities[foundIdx]) {
            printf("  [i] Priority for Ticket #%d is unchanged.\n",
                   ticketIds[foundIdx]);
        } else {
            char oldStr[16];
            char newStr[16];
            getPriorityLabel(priorities[foundIdx], oldStr, (int)sizeof(oldStr));
            getPriorityLabel(newPrio, newStr, (int)sizeof(newStr));
            priorities[foundIdx] = newPrio;
            printf("  [+] Ticket #%d priority updated: %s -> %s.\n",
                   ticketIds[foundIdx], oldStr, newStr);
        }
        return 1;
    } else if (action == 2) {
        statuses[foundIdx] = STATUS_CANCELLED;
        printf("  [+] Ticket #%d (%s) has been cancelled.\n",
               ticketIds[foundIdx], patientNames[foundIdx]);
        return 1;
    }

    return 0;
}
