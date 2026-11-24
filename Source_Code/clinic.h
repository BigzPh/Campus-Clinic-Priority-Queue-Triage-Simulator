#ifndef CLINIC_H
#define CLINIC_H

/* Capacity and string limits */
#define MAX_PATIENTS 50
#define MAX_STATIONS 3
#define MAX_NAME_LEN 40
#define MAX_SYMPTOM_LEN 50

/* Simulation defaults */
#define BASE_TICKET_ID 101
#define CLINIC_OPEN_MINUTES 480 /* 08:00 AM */
#define INPUT_EOF -1

/* Priority levels (lower number = higher urgency) */
#define PRIORITY_EMERGENCY 1
#define PRIORITY_URGENT 2
#define PRIORITY_STANDARD 3

/* Ticket lifecycle statuses */
#define STATUS_WAITING 0
#define STATUS_IN_SERVICE 1
#define STATUS_DONE 2
#define STATUS_CANCELLED 3

/* --- input_utils.c --- */
int readIntInRange(const char prompt[], int minVal, int maxVal);
int readNonEmptyString(const char prompt[], char dest[], int maxLen);
void copyStringSafe(char dest[], const char src[], int maxLen);

/* --- triage.c --- */
void initSimulation(int stationTicketIdx[], int stationRemaining[],
                    int stationServed[], int waitTimes[], int statuses[]);
int countByStatus(const int statuses[], int patientCount, int targetStatus);
int updatePeakQueue(const int statuses[], int patientCount, int currentPeak);
int registerPatient(int ticketIds[], char patientNames[][MAX_NAME_LEN],
                    char symptoms[][MAX_SYMPTOM_LEN], int priorities[],
                    int arrivalTimes[], int serviceDurations[],
                    int waitTimes[], int statuses[], int patientCount,
                    int currentClock);
int loadDemoBatch(int ticketIds[], char patientNames[][MAX_NAME_LEN],
                  char symptoms[][MAX_SYMPTOM_LEN], int priorities[],
                  int arrivalTimes[], int serviceDurations[],
                  int waitTimes[], int statuses[], int patientCount,
                  int currentClock);
int findNextWaitingIndex(const int priorities[], const int arrivalTimes[],
                         const int ticketIds[], const int statuses[],
                         int patientCount);
int dispatchWaitingPatients(const int ticketIds[],
                            const char patientNames[][MAX_NAME_LEN],
                            const int priorities[], const int arrivalTimes[],
                            const int serviceDurations[], int waitTimes[],
                            int statuses[], int stationTicketIdx[],
                            int stationRemaining[], int patientCount,
                            int currentClock);
int advanceClock(const int ticketIds[], const char patientNames[][MAX_NAME_LEN],
                 const int priorities[], const int arrivalTimes[],
                 const int serviceDurations[], int waitTimes[], int statuses[],
                 int stationTicketIdx[], int stationRemaining[],
                 int stationServed[], int patientCount, int currentClock,
                 int minutes);
int manageWaitingTicket(const int ticketIds[],
                        const char patientNames[][MAX_NAME_LEN],
                        int priorities[], const int arrivalTimes[],
                        int statuses[], int patientCount);

/* --- display.c --- */
void formatClockTime(int elapsedMinutes, char out[], int outSize);
void getPriorityLabel(int priority, char out[], int outSize);
void getStatusLabel(int status, char out[], int outSize);
void displayBanner(void);
void displayMenu(int currentClock, int waitingCount);
void displayDashboard(const int ticketIds[],
                      const char patientNames[][MAX_NAME_LEN],
                      const char symptoms[][MAX_SYMPTOM_LEN],
                      const int priorities[], const int arrivalTimes[],
                      const int serviceDurations[], const int statuses[],
                      const int stationTicketIdx[],
                      const int stationRemaining[], int patientCount,
                      int currentClock);
void displayAllTickets(const int ticketIds[],
                       const char patientNames[][MAX_NAME_LEN],
                       const char symptoms[][MAX_SYMPTOM_LEN],
                       const int priorities[], const int arrivalTimes[],
                       const int serviceDurations[], const int waitTimes[],
                       const int statuses[], int patientCount);
void displayAnalytics(const int priorities[], const int arrivalTimes[],
                      const int waitTimes[], const int statuses[],
                      const int stationTicketIdx[], const int stationServed[],
                      int patientCount, int currentClock, int peakQueue);

#endif /* CLINIC_H */
