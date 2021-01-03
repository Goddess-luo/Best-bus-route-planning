/*
Procedure steps:
1. Prompt information, initial input data, n stations, m lines, each line has at most k stations, so the
   number of total stations n>=k.
   n stations use n data storage, and m*n arrays are used to store station names and timetables according
   to the input line.
2. Prompt the user to enter the departure point, arrival point and departure time, find the index of the
   corresponding total station according to the departure point and arrival point, then traverse all the
   routes, and check whether it is possible in the route to find a route that meets the conditions, and
   output it directly if there are any.
3. In the process of finding the transfer station, the worst case is to traverse all the stations of
   each line separately, and then compare the two lines to see which arrival point is earlier. At the end
   of the output, the worst case is that both lines are required. After the traversal, the time complexity
   is O(m * k). Similar to the first step, k <= n, so the time complexity can be expressed as O(m * n).

 Therefore, in the worst case, the program needs to traverse all stations and timetables, that is, traverse
 m*n arrays storing stations and times, that is, the time complexity is O(m * n).
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* calculate the schedule time */
int calScheduleTime(char *schedule) {
    int t = 0;
    int divisor[4] = {600, 60, 10, 1};
    t += (schedule[0] - '0') * divisor[0];
    t += (schedule[1] - '0') * divisor[1];
    t += (schedule[2] - '0') * divisor[2];
    t += (schedule[3] - '0') * divisor[3];
    return t;
}

/* print time string by time */
void showScheduleTime(int t) {
    char s[5];
    int divisor[4] = {600, 60, 10, 1};
    s[0] = t / divisor[0] + '0';
    t %= divisor[0];
    s[1] = t / divisor[1] + '0';
    t %= divisor[1];
    s[2] = t / divisor[2] + '0';
    t %= divisor[2];
    s[3] = t / divisor[3] + '0';
    s[4] = '\0';
    printf("%s ", s);
}

/* calculate the stop index by name */
int calStopIndex(char **stopNames, char *name, int number) {
    int i;
    for (i = 0; i < number; i++) {
        if (strcmp(stopNames[i], name) == 0) {
            return i + 1;
        }
    }
    return 0;
}

/* Found a route from the starting point to the ending point and printed it */
void showResult(char **stopNames, int **scheduleStopRecord, int **scheduleRecord, int index, int start, int end) {
    printf("\n");
    int i;
    int sche_t;
    int sche_n;
    for (i = start; i <= end; i++) {
        sche_t = scheduleRecord[index][i];
        showScheduleTime(sche_t);
        sche_n = scheduleStopRecord[index][i];
        printf("%s\n", stopNames[sche_n - 1]);
    }
}


/* main function */
int main() {

    int stopNumbers;
    int scheduleNumbers;
    int scheduleStopNumbers;
    char **stopNames;
    char *schedule;
    char *name;
    int **scheduleStopRecord;
    int **scheduleRecord;
    int i, j;


    schedule = (char *) malloc(sizeof(char) * 5);
    name = (char *) malloc(sizeof(char) * 32);


    /* Get the total number of sites and the name of each site */
    printf("Enter the total number of stops on the network: ");
    scanf("%d", &stopNumbers);
    stopNames = (char **) malloc(sizeof(char *) * stopNumbers);
    for (i = 0; i < stopNumbers; i++) {
        stopNames[i] = (char *) malloc(sizeof(char) * 32);
    }
    for (i = 0; i < stopNumbers; i++) {
        scanf("%s", stopNames[i]);
    }

/* One way of thinking is to use an array of pointers to store the site name and time of each line,
 * but there seems to be a problem */
    char *all_schedules[stopNumbers];
    char *all_schedule_times[stopNumbers];
    for (int p = 0; p < stopNumbers; p++) {  // Allocate storage space for each site
        all_schedules[p] = (char *) malloc(32 * sizeof(char));
        assert(all_schedules[p] != NULL);
        all_schedule_times[p] = (char *) malloc(5 * sizeof(char));
        assert(all_schedule_times[p] != NULL);
    }

    /* The second idea is to define a structure to store the site name and time of each line */
    typedef char *pt_of_stop[stopNumbers];
    typedef char *pt_of_time[stopNumbers];

/* An extension of the first idea, a two-dimensional pointer array, and allocate space for each bit at the same time */
    char *stop[stopNumbers][32];
    char *times[stopNumbers][5];

    for (int q = 0; q < stopNumbers; q++) {  /* Allocate storage space for each site */
        stop[q][0] = (char *) malloc(32 * sizeof(char *));
        assert(stop[q][0] != NULL);
        times[q][0] = (char *) malloc(5 * sizeof(char));
        assert(times[q][0] != NULL);
    }


    printf("Enter the number of schedules: ");
    scanf("%d", &scheduleNumbers);
    scheduleStopRecord = (int **) malloc(sizeof(int *) * scheduleNumbers);
    scheduleRecord = (int **) malloc(sizeof(int *) * scheduleNumbers);
    /* Used for the first idea, define a separate pointer, and then return to the pointer array of the first idea */
    char *pt_to_all_schedules;
    char *pt_to_all_schedule_times;
    for (int s = 0; s < scheduleNumbers; s++) {
        pt_to_all_schedules = (char *) malloc(scheduleNumbers * sizeof(char));
        assert(pt_to_all_schedules != NULL);
        pt_to_all_schedule_times = (char *) malloc(scheduleNumbers * sizeof(char));
        assert(pt_to_all_schedule_times != NULL);
    }
    for (int l = 0; l < scheduleNumbers; l++) {
        pt_to_all_schedule_times = all_schedule_times[l];
        pt_to_all_schedules = all_schedules[l];
    }

    for (i = 0; i < scheduleNumbers; i++) {
        scheduleStopRecord[i] = (int *) malloc(sizeof(int) * stopNumbers);
        scheduleRecord[i] = (int *) malloc(sizeof(int) * stopNumbers);
    }

/* The fourth idea, an extension of the third idea, is to define a two-dimensional pointer array,
 * and then call a custom function to get the site name and time */
    for (i = 0; i < scheduleNumbers; i++) {
        printf("Enter the number of stops: ");
        scanf("%d", &scheduleStopNumbers);
        for (j = 0; j < scheduleStopNumbers; j++) {
            scanf("%s", schedule);
            scheduleRecord[i][j] = calScheduleTime(schedule);
            scanf("%s", name);
            scheduleStopRecord[i][j] = calStopIndex(stopNames, name, stopNumbers);
        }
        if (scheduleStopNumbers < stopNumbers) {  /* Compare whether the departure time meets the requirements */
            int x = scheduleStopNumbers;
            while (x < stopNumbers) {
                scheduleStopRecord[i][x] = -1;
                scheduleRecord[i][x] = -1;
                x++;
            }
        }
    }

    /* Define two pointers to get the site and time of the query */
    char *st;
    st = (char *) malloc(sizeof(char) * 32);
    assert(st != NULL);
    char *time;
    time = (char *) malloc(sizeof(char) * 5);
    assert(time != NULL);
    int from;
    int to;
    int d_t;
    do {
        printf("\nFrom: ");
        scanf("%s", st);
        from = calStopIndex(stopNames, st, stopNumbers);/* Get the index of the starting point */
        if (from == 0) {
            printf("Thank you for using goNSW.\n");
            break;
        }
        printf("To: ");
        scanf("%s", st);
        to = calStopIndex(stopNames, st, stopNumbers);/* Get the index of the arrival point */
        printf("Depart at: ");
        scanf("%s", time);
        d_t = calScheduleTime(time);/* Get the time of departure */
        int start = -1, end = -1;/* Initialize the index value of the station name found for each line */
        int start_i = -1, end_i = -1;/* Initialize the index value of the found line */
        int end_time;
        int flag = 0;
        for (i = 0; i < scheduleNumbers; i++) {
            for (j = 0; j < stopNumbers; j++) {
                if (start < 0) {
                    if (scheduleStopRecord[i][j] == from) {/* The departure point was found in the main station */
                        if (scheduleRecord[i][j] >= d_t) {/* and the departure time was later than depart */
                            start = j;
                            start_i = i;
                            end_time = scheduleRecord[start_i][start];
                        }
                    }
                }
                if (scheduleStopRecord[i][j] == to) {
                    end_i = i;
                    end = j;/* The arrival point was found in the main station */
                    if (scheduleRecord[end_i][end] < end_time){
                        end_time = scheduleRecord[end_i][end];
                    }
                }
            }
        }

        if (start_i >= 0 && end_i >= 0) {
            if (end_i == start_i) {/* Departure point and arrival point are on the same line, output directly */
                for (i = 0; i < scheduleNumbers; i++) {
                    /* Both the departure point and arrival point are found,
                    * and the index value of the arrival point is greater than the departure point */
                    if (start >= 0 && end > start) {
                        /* Just print this paragraph directly */
                        showResult(stopNames, scheduleStopRecord, scheduleRecord, start_i, start, end);
                        flag += 1;
                        break;
                    }
                }
            } else {/* Departure point and arrival point are not on the same route */
                int break_point = 0;
                /* The following four statements check for transfer points */
                for (i = 0; i < stopNumbers; i++) {
                    for (j = 0; j < stopNumbers; j++) {
                        if (scheduleStopRecord[start_i][i] == scheduleStopRecord[end_i][j]) {
                            if (scheduleRecord[start_i][i] <= scheduleRecord[end_i][j]) {
                                /* Output to different lines according to the transfer point */
                                if (start >= 0 && end >= 0) {
                                    showResult(stopNames, scheduleStopRecord, scheduleRecord, start_i, start, i);
                                    int sch_name = scheduleStopRecord[start_i][i];
                                    printf("Change at %s", stopNames[sch_name - 1]);
                                    showResult(stopNames, scheduleStopRecord, scheduleRecord, end_i, j, end);
                                    flag += 1;
                                    break_point += 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (break_point != 0){
                        break;
                    }
                }
            }
        }
        if (flag == 0) {
            printf("\nNo connection found.\n");
        }
    } while (10);

    for (i = 0;
         i < stopNumbers; i++) {/* Release the space for recording the total site and the call of the following ideas */
        free(stopNames[i]);
        free(all_schedules[i]);
        free(all_schedule_times[i]);
    }
    for (i = 0; i < scheduleNumbers; i++) {/* Free up the space called when recording line information */
        free(scheduleStopRecord[i]);
        free(scheduleRecord[i]);
    }
    free(scheduleStopRecord);/* Release all single pointers that have allocated space */
    free(scheduleRecord);
    free(st);
    free(time);
    free(stopNames);
    free(schedule);
    free(name);
    return 0;
}