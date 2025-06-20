#include <stdio.h>

typedef enum Status {"Done"}Status;

/*
    프로세스가 자겨야 할 정보
    1. MAX
    2. Allocated
    3. Need
*/
typedef struct Process {
    int* max;
    int* allocated;
    int* need;
    int ready_time;

} Process;

typedef struct System {
    int* available;
    int isSafe;
} System;

int main() {

    FILE* inp = fopen("1.inp", "r");
    FILE* out = fopen("1.out", "w");
    if (!inp || !out) {
        printf("File Open Failed");
        return 1;
    }
    
    // 프로세스 수
    int N;
    fscanf(inp, "%d", &N);

    // 자원 수
    int M;
    fscanf(inp, "%d", &M);

    // 시스템 초기화
    System s;
    s.available = (int)malloc(sizeof(int) * M);
    for (int i = 0; i < M; i++) {
        fscanf(inp, "%d", &s.available[i]);
    }

    // 프로세스 초기화
    Process p[N];
    // 각 프로세스의 MAX 입력
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(inp, "%d", &p[i].max[j]);
        }
    }

    // 각 프로세스의 Allocation 입력
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(inp, "%d", &p[i].allocated[j]);
            int need_sources = p[i].max[j] - p[i].allocated[j];
            p[i].need[j] = need_sources;
        }
    }

    while(1) {
        int which_process = -1;
        int* request_sources = NULL;
        int* release_sources = NULL;
        // 문자가 request인지, quit인지 판단
        char* check_string = NULL;
        fscanf(inp, "%s", &check_string);
        // quit면 break
        if (check_string == 'quit') {
            break;
        }
        // request이면 진행
        else if (check_string == 'request') {

            // 어떤 프로세스인지 판단
            fscanf(inp, "%d", &which_process);
            if (which_process < 0) break;

            // 요구하는 자원들의 수
            for (int i = 0; i < M; i++) {
                fscanf(inp, "%d", &request_sources[i]);
            }

            // 프로세스가 요구하는 자원의 수와 현재 배분 가능한 자원의 수를 비교 => 배분해도 safe하다면 배분
            for (int i = 0; i < M; i++) {
                
            }

        }
        else if (check_string == 'release') {

            // 어떤 프로세스인지 판단
            fscanf(inp, "%d", &which_process);
            if (which_process < 0) break;

            // 반환하는 자원들의 수
            for(int i = 0; i < M; i++) {
                fscanf(inp, "%d", &release_sources[i]);
            }

            // 
            for (int i = 0; i < M; i++) {

            }

        }
    }

    fclose(inp);
    fclose(out);

    return 0;
}