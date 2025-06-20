#include <stdio.h>

#define MAX_PROCESS 1000
#define MAX_WORKS 100

typedef enum {Ready, Running, IO, Done, Not_Created} Status;

typedef struct Process {
    int created_time;
    int p_id;
    int works[MAX_WORKS];
    int now;
    Status status;
    int end_time;
    int arrived_time;
} Process;

int remaining_cpu_time(Process* p) {
    if (p->now % 2 == 0) {
        return p->works[p->now];
    }
    return 0;
}

int all_processes_done(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        if (p[i].status != Done) return 0;
    }
    return 1;
}

int main() {
    FILE* inp = fopen("srtf.inp", "r");
    FILE* out = fopen("srtf.out", "w");
    if (!inp || !out) return 1;

    int N;
    fscanf(inp, "%d", &N);
    Process p[MAX_PROCESS];

    for (int i = 0; i < N; i++) {
        p[i].p_id = i;
        p[i].now = 0;
        p[i].status = Not_Created;
        p[i].end_time = 0;
        p[i].arrived_time = -1;
        fscanf(inp, "%d", &(p[i].created_time));
        int w, idx = 0;
        while (fscanf(inp, "%d", &w) && w != -1) {
            p[i].works[idx++] = w;
        }
        p[i].works[idx] = -1;
    }

    int T = 0, idle = 0;
    while (!all_processes_done(p, N)) {
        for (int i = 0; i < N; i++) {
            if (p[i].status == Not_Created && p[i].created_time == T) {
                p[i].status = Ready;
                p[i].arrived_time = T;
            }
        }

        int min_idx = -1, min_remain = 1e9;
        for (int i = 0; i < N; i++) {
            if ((p[i].status == Ready || p[i].status == Running) && p[i].now % 2 == 0) {
                int remain = remaining_cpu_time(&p[i]);
                if (min_idx == -1 || remain < min_remain ||
                    (remain == min_remain && p[i].arrived_time < p[min_idx].arrived_time) ||
                    (remain == min_remain && p[i].arrived_time == p[min_idx].arrived_time && p[i].p_id < p[min_idx].p_id)) {
                    min_idx = i;
                    min_remain = remain;
                }
            }
        }

        for (int i = 0; i < N; i++) {
            if (p[i].status == Running && i != min_idx) {
                p[i].status = Ready;
                p[i].arrived_time = T;
            }
        }

        if (min_idx != -1) {
            p[min_idx].status = Running;
        }

        for (int i = 0; i < N; i++) {
            if (p[i].status == Running || p[i].status == IO) {
                p[i].works[p[i].now]--;
            }
        }

        for (int i = 0; i < N; i++) {
            if ((p[i].status == Running || p[i].status == IO) && p[i].works[p[i].now] == 0) {
                p[i].now++;
                if (p[i].works[p[i].now] == -1) {
                    p[i].status = Done;
                    p[i].end_time = T + 1;
                } else if (p[i].now % 2 == 0) {
                    p[i].status = Ready;
                    p[i].arrived_time = T + 1;
                } else {
                    p[i].status = IO;
                }
            }
        }

        int cpu_idle = 1;
        for (int i = 0; i < N; i++) {
            if ((p[i].status == Ready && p[i].now % 2 == 0) || p[i].status == Running) {
                cpu_idle = 0;
                break;
            }
        }
        if (cpu_idle) idle++;

        T++;
    }

    fprintf(out, "%d\n", idle);
    for (int i = 0; i < N; i++) {
        fprintf(out, "%d\n", p[i].end_time);
    }

    fclose(inp);
    fclose(out);
    return 0;
}
