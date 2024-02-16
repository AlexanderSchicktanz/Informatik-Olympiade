#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char hours;
    char minutes;
} time;

typedef struct {
    short length;
    time *times;
} timeArray;

typedef struct {
    time *times;
    short index;
} solution;

unsigned char countSetBits(unsigned int n) {
    unsigned char count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void numHoursOf(timeArray *array, char bits) {
    array->length = 0;
    for (char i = 0; i < 12; i++) {
        if (bits == countSetBits(i)) {
            array->times[array->length].hours = i;
            array->times[array->length].minutes = 0;
            array->length++;
        }
    }
}

void numMinutesOf(timeArray *array, char bits) {
    array->length = 0;
    for (short i = 0; i < 60; i++) {
        if (bits == countSetBits(i)) {
            array->times[array->length].hours = 0;
            array->times[array->length].minutes = i;
            array->length++;
        }
    }
}

void append(solution *s, timeArray hour, timeArray minute) {
    for (short i = 0; i < hour.length; i++) {
        for (short j = 0; j < minute.length; j++) {
            s->times[s->index].hours = hour.times[i].hours;
            s->times[s->index].minutes = minute.times[j].minutes;
            s->index++;
        }
    }
}

void output(solution s) {
    printf("[");
    for (short i = 0; i < s.index; i++) {
        printf("\"%d:%02d\"", s.times[i].hours, s.times[i].minutes);
        if (i < s.index - 1) {
            printf(",");
        }
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {
    char bitNum = 0;
    solution sol = {
        .index = 0,
        .times = (time *)malloc(sizeof(time) * 720)
    };
    if (argc > 1) {
        bitNum = argv[1][0] - '0'; //First byte - ascii value of 0 to get offset
        if(strlen(argv[1])>1){
            printf("[]");
            return 1;
        }
    } else {
        printf("Too few arguments provided!\nUse: clock <number of bits>\n");
        return 1;
    }
    timeArray hourVersions, minuteVersions;
    hourVersions.times = (time *)malloc(sizeof(time) * 12);
    minuteVersions.times = (time *)malloc(sizeof(time) * 60);
    
    for (char hourBits = 0; hourBits <= bitNum; hourBits++) {
        if (hourBits > 3 || bitNum - hourBits > 5) {
            continue;
        }
        numHoursOf(&hourVersions, hourBits);
        numMinutesOf(&minuteVersions, bitNum - hourBits);
        append(&sol, hourVersions, minuteVersions);
    }
    
    output(sol);
    
    free(hourVersions.times);
    free(minuteVersions.times);
    free(sol.times);
    return 0;
}
