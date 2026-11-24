#include <stdio.h>
#include "clinic.h"

int main(void) {
    /* Parallel arrays holding patient ticket records */
    int ticketIds[MAX_PATIENTS];
    char patientNames[MAX_PATIENTS][MAX_NAME_LEN];
    char symptoms[MAX_PATIENTS][MAX_SYMPTOM_LEN];
    int priorities[MAX_PATIENTS];
    int arrivalTimes[MAX_PATIENTS];
    int serviceDurations[MAX_PATIENTS];
    int waitTimes[MAX_PATIENTS];
    int statuses[MAX_PATIENTS];

    /* Parallel arrays tracking clinic treatment stations */
    int stationTicketIdx[MAX_STATIONS];
    int stationRemaining[MAX_STATIONS];
    int stationServed[MAX_STATIONS];

    int patientCount = 0;
    int currentClock = 0;
    int peakQueue = 0;
    int choice = 0;

    initSimulation(stationTicketIdx, stationRemaining, stationServed, waitTimes,
                   statuses);
    displayBanner();

    do {
        int waitingCount =
            countByStatus(statuses, patientCount, STATUS_WAITING);
        displayMenu(currentClock, waitingCount);

        choice = readIntInRange("Select an option (0-8): ", 0, 8);
        if (choice == INPUT_EOF) {
            printf("\nInput stream ended. Closing simulator...\n");
            break;
        }

        switch (choice) {
            case 1:
                patientCount = registerPatient(
                    ticketIds, patientNames, symptoms, priorities, arrivalTimes,
                    serviceDurations, waitTimes, statuses, patientCount,
                    currentClock);
                peakQueue =
                    updatePeakQueue(statuses, patientCount, peakQueue);
                break;

            case 2: {
                int currentWaiting =
                    countByStatus(statuses, patientCount, STATUS_WAITING);
                if (currentWaiting == 0) {
                    printf("\n[i] No waiting patients to dispatch.\n");
                } else {
                    int dispatched = 0;
                    printf("\n--- Dispatching Waiting Patients ---\n");
                    dispatched = dispatchWaitingPatients(
                        ticketIds, patientNames, priorities, arrivalTimes,
                        serviceDurations, waitTimes, statuses, stationTicketIdx,
                        stationRemaining, patientCount, currentClock);
                    if (dispatched == 0) {
                        printf("  [i] All %d treatment stations are currently "
                               "busy.\n",
                               MAX_STATIONS);
                    }
                }
                break;
            }

            case 3: {
                int minutes = readIntInRange(
                    "\nEnter minutes to advance clock (1-120): ", 1, 120);
                if (minutes == INPUT_EOF) {
                    choice = 0;
                } else {
                    currentClock = advanceClock(
                        ticketIds, patientNames, priorities, arrivalTimes,
                        serviceDurations, waitTimes, statuses, stationTicketIdx,
                        stationRemaining, stationServed, patientCount,
                        currentClock, minutes);
                }
                break;
            }

            case 4:
                displayDashboard(ticketIds, patientNames, symptoms, priorities,
                                 arrivalTimes, serviceDurations, statuses,
                                 stationTicketIdx, stationRemaining,
                                 patientCount, currentClock);
                break;

            case 5:
                manageWaitingTicket(ticketIds, patientNames, priorities,
                                    arrivalTimes, statuses, patientCount);
                break;

            case 6:
                displayAllTickets(ticketIds, patientNames, symptoms, priorities,
                                  arrivalTimes, serviceDurations, waitTimes,
                                  statuses, patientCount);
                break;

            case 7:
                displayAnalytics(priorities, arrivalTimes, waitTimes, statuses,
                                 stationTicketIdx, stationServed, patientCount,
                                 currentClock, peakQueue);
                break;

            case 8:
                patientCount = loadDemoBatch(
                    ticketIds, patientNames, symptoms, priorities, arrivalTimes,
                    serviceDurations, waitTimes, statuses, patientCount,
                    currentClock);
                peakQueue =
                    updatePeakQueue(statuses, patientCount, peakQueue);
                break;

            case 0:
                printf("\nClosing Campus Clinic Triage Simulator.\n");
                break;

            default:
                printf("\n[!] Invalid option.\n");
                break;
        }
    } while (choice != 0);

    if (patientCount > 0) {
        displayAnalytics(priorities, arrivalTimes, waitTimes, statuses,
                         stationTicketIdx, stationServed, patientCount,
                         currentClock, peakQueue);
    }

    return 0;
}
