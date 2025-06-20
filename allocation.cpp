#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#define MEM_SIZE 1000

using namespace std;

struct Process {
    int pid;
    int arrival;
    int duration;
    int size;
    int start_time = -1;
    int addr = -1;

    Process(int _pid, int _arrival, int _duration, int _size)
        : pid(_pid), arrival(_arrival), duration(_duration), size(_size) {}
};

struct Block {
    int addr;
    int pid; // -1이면 hole
    int size;

    bool isHole() const {
        return pid == -1;
    }
};

enum FitType { FIRST_FIT, BEST_FIT, WORST_FIT };

void simulate(vector<Process>& processes, FitType type) {
    vector<Block> memory = { {0, -1, MEM_SIZE} };
    queue<Process> waitQ;
    vector<Process> procQueue = processes;
    int time = 0, loaded = 0, n = processes.size();

    while (loaded < n) {
        // 1. 종료 프로세스 메모리 반환
        for (auto& blk : memory) {
            if (blk.pid != -1) {
                Process& p = processes[blk.pid];
                if (p.start_time != -1 && p.start_time + p.duration == time) {
                    blk.pid = -1;
                }
            }
        }

        // 2. hole 병합
        for (int i = 0; i + 1 < memory.size();) {
            if (memory[i].isHole() && memory[i+1].isHole()) {
                memory[i].size += memory[i+1].size;
                memory.erase(memory.begin() + i + 1);
            } else {
                i++;
            }
        }

        // 3. 현재 시간에 도착한 프로세스 대기큐로
        while (!procQueue.empty() && procQueue.front().arrival == time) {
            waitQ.push(procQueue.front());
            procQueue.erase(procQueue.begin());
        }

        // 4. 대기 큐에서 적재 시도
        int waitSize = waitQ.size();
        for (int i = 0; i < waitSize; ++i) {
            Process p = waitQ.front(); waitQ.pop();

            int bestIdx = -1;
            int bestMetric = (type == BEST_FIT ? MEM_SIZE + 1 : -1);

            for (int j = 0; j < memory.size(); ++j) {
                Block& blk = memory[j];
                if (blk.isHole() && blk.size >= p.size) {
                    if (type == FIRST_FIT) {
                        bestIdx = j;
                        break;
                    } else if (type == BEST_FIT && blk.size < bestMetric) {
                        bestIdx = j;
                        bestMetric = blk.size;
                    } else if (type == WORST_FIT && blk.size > bestMetric) {
                        bestIdx = j;
                        bestMetric = blk.size;
                    }
                }
            }

            if (bestIdx != -1) {
                // 적재
                Process& ref = processes[p.pid];
                ref.start_time = time;
                ref.addr = memory[bestIdx].addr;
                loaded++;

                Block newProc = { memory[bestIdx].addr, p.pid, p.size };
                if (memory[bestIdx].size > p.size) {
                    Block newHole = { memory[bestIdx].addr + p.size, -1, memory[bestIdx].size - p.size };
                    memory.erase(memory.begin() + bestIdx);
                    memory.insert(memory.begin() + bestIdx, newHole);
                    memory.insert(memory.begin() + bestIdx, newProc);
                } else {
                    memory[bestIdx] = newProc;
                }
            } else {
                waitQ.push(p); // 여전히 대기
            }
        }

        time++;
    }
}

int main() {
    ifstream fin("allocation.inp");
    ofstream fout("allocation.out");

    int n;
    fin >> n;

    vector<Process> base;
    for (int i = 0; i < n; ++i) {
        int a, d, s;
        fin >> a >> d >> s;
        base.push_back(Process(i, a, d, s));
    }

    for (int fit = 0; fit < 3; ++fit) {
        vector<Process> test = base;
        simulate(test, static_cast<FitType>(fit));
        fout << test.back().addr << '\n'; // 마지막 프로세스의 시작 주소만 출력
    }

    fin.close();
    fout.close();
    return 0;
}
