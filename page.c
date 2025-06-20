#include <stdio.h>
#include <stdlib.h>

#define MAX_REFERENCES 10000
#define MAX_FRAMES 100

int references[MAX_REFERENCES];
int ref_count = 0;

// 입력 파일 읽기
void read_input(const char *filename, int *frame_size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패");
        exit(1);
    }

    fscanf(fp, "%d", frame_size);

    while (fscanf(fp, "%d", &references[ref_count]) == 1) {
        if (references[ref_count] == -1)
            break;
        ref_count++;
    }

    fclose(fp);
}

int is_in_memory(int *memory, int size, int page) {
    for (int i = 0; i < size; i++) {
        if (memory[i] == page)
            return 1;
    }
    return 0;
}

int fifo(int frame_size) {
    int memory[MAX_FRAMES];
    int next_replace = 0;
    int page_faults = 0;

    for (int i = 0; i < frame_size; i++) memory[i] = -1;

    for (int i = 0; i < ref_count; i++) {
        int page = references[i];
        int found = 0;

        for (int j = 0; j < frame_size; j++) {
            if (memory[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            memory[next_replace] = page;
            next_replace = (next_replace + 1) % frame_size;
            page_faults++;
        }
    }

    return page_faults;
}

int lru(int frame_size) {
    int memory[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int page_faults = 0;
    int time = 0;

    for (int i = 0; i < frame_size; i++) {
        memory[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < ref_count; i++, time++) {
        int page = references[i];
        int found = 0;

        // 페이지가 이미 메모리에 있는지 확인
        for (int j = 0; j < frame_size; j++) {
            if (memory[j] == page) {
                last_used[j] = time;  // 최근 사용 시간 갱신
                found = 1;
                break;
            }
        }

        if (!found) {
            // 빈 프레임이 있는 경우 먼저 채움
            int replaced_index = -1;
            for (int j = 0; j < frame_size; j++) {
                if (memory[j] == -1) {
                    replaced_index = j;
                    break;
                }
            }

            // 빈 프레임이 없으면 가장 오래 전에 사용된 것 교체
            if (replaced_index == -1) {
                int lru_time = last_used[0];
                replaced_index = 0;
                for (int j = 1; j < frame_size; j++) {
                    if (last_used[j] < lru_time) {
                        lru_time = last_used[j];
                        replaced_index = j;
                    }
                }
            }

            memory[replaced_index] = page;
            last_used[replaced_index] = time;
            page_faults++;
        }
    }

    return page_faults;
}


int opt(int frame_size) {
    int memory[MAX_FRAMES];
    int page_faults = 0;

    for (int i = 0; i < frame_size; i++) memory[i] = -1;

    for (int i = 0; i < ref_count; i++) {
        int page = references[i];
        int found = 0;

        for (int j = 0; j < frame_size; j++) {
            if (memory[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            int replace_index = -1;
            int farthest = -1;

            for (int j = 0; j < frame_size; j++) {
                if (memory[j] == -1) {
                    replace_index = j;
                    break;
                }

                int k;
                for (k = i + 1; k < ref_count; k++) {
                    if (references[k] == memory[j]) break;
                }

                if (k == ref_count) { // never used again
                    replace_index = j;
                    break;
                }

                if (k > farthest) {
                    farthest = k;
                    replace_index = j;
                }
            }

            memory[replace_index] = page;
            page_faults++;
        }
    }

    return page_faults;
}


int main() {
    int frame_size;
    read_input("page.inp", &frame_size);

    int fifo_result = fifo(frame_size);
    int lru_result = lru(frame_size);
    int opt_result = opt(frame_size);

    FILE *fp = fopen("page.out", "w");
    if (!fp) {
        perror("출력 파일 열기 실패");
        return 1;
    }

    fprintf(fp, "FIFO: %d\n", fifo_result);
    fprintf(fp, "LRU: %d\n", lru_result);
    fprintf(fp, "OPT: %d\n", opt_result);

    fclose(fp);

    return 0;
}
