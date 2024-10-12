#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

// 방향: 상, 우, 하, 좌
int di[] = {-1, 0, 1, 0};
int dj[] = {0, 1, 0, -1};

void push_unit(int start, int dr, map<int, vector<int>>& units, vector<vector<int>>& arr, vector<int>& damage, set<int>& pset) {
    queue<int> q;  // push 후보를 저장
    q.push(start); // 초기 데이터 append
    pset.insert(start);

    while (!q.empty()) {
        int cur = q.front(); // q에서 데이터 한개 꺼냄
        q.pop();
        
        int ci = units[cur][0];
        int cj = units[cur][1];
        int h = units[cur][2];
        int w = units[cur][3];
        int k = units[cur][4];

        // 명령받은 방향 진행, 벽이 아니면, 겹치는 다른 조각이면 => 큐에 삽입
        int ni = ci + di[dr];
        int nj = cj + dj[dr];
        
        for (int i = ni; i < ni + h; i++) {
            for (int j = nj; j < nj + w; j++) {
                if (arr[i][j] == 2) { // 벽!! => 모두 취소
                    return;
                }
                if (arr[i][j] == 1) { // 함정인 경우
                    damage[cur] += 1; // 데미지 누적
                }
            }
        }

        // 겹치는 다른 유닛 있는 경우 큐에 추가 (모든 유닛 체크)
        for (const auto& idx : units) {
            int unit_idx = idx.first;
            if (pset.count(unit_idx)) continue; // 이미 움직일 대상이면 체크할 필요 없음

            int ti = units[unit_idx][0];
            int tj = units[unit_idx][1];
            int th = units[unit_idx][2];
            int tw = units[unit_idx][3];

            // 겹치는 경우
            if (ni <= ti + th - 1 && ni + h - 1 >= ti && nj <= tj + tw - 1 && nj + w - 1 >= tj) {
                q.push(unit_idx);
                pset.insert(unit_idx);
            }
        }
    }

    // 명령 받은 기사는 데미지 입지 않음
    damage[start] = 0;

    // 이동, 데미지가 체력 이상이면 삭제 처리
    for (const auto& idx : pset) {
        int si = units[idx][0];
        int sj = units[idx][1];
        int h = units[idx][2];
        int w = units[idx][3];
        int k = units[idx][4];

        if (k <= damage[idx]) { // 체력보다 더 큰 데미지면 삭제
            units.erase(idx);
        } else {
            int ni = si + di[dr];
            int nj = sj + dj[dr];
            units[idx] = {ni, nj, h, w, k - damage[idx]};
        }
    }
}

int main() {
    int N, M, Q;
    cin >> N >> M >> Q;

    // 벽으로 둘러싸서, 범위 체크 안하고, 범위 밖으로 밀리지 않게 처리
    vector<vector<int>> arr(N + 2, vector<int>(N + 2, 2));
    for (int i = 1; i <= N; i++) {
        arr[i][0] = arr[i][N + 1] = 2; // 벽 처리
        for (int j = 1; j <= N; j++) {
            cin >> arr[i][j]; // 유닛 정보 입력
        }
    }
    for (int j = 0; j <= N + 1; j++) {
        arr[0][j] = arr[N + 1][j] = 2; // 벽 처리
    }

    map<int, vector<int>> units; // 유닛 저장
    vector<int> init_k(M + 1, 0); // 초기 체력 저장
    vector<int> damage(M + 1, 0); // 데미지 누적

    for (int m = 1; m <= M; m++) {
        int si, sj, h, w, k;
        cin >> si >> sj >> h >> w >> k;
        units[m] = {si, sj, h, w, k};
        init_k[m] = k; // 초기 체력 저장
    }

    for (int _ = 0; _ < Q; _++) { // 명령 입력받고 처리
        int idx, dr;
        cin >> idx >> dr;
        if (units.count(idx)) {
            set<int> pset; // 이동할 유닛 번호 저장
            push_unit(idx, dr, units, arr, damage, pset); // 명령받은 유닛 처리
        }
    }

    int ans = 0;
    for (const auto& idx : units) {
        ans += init_k[idx.first] - units[idx.first][4];
    }
    cout << ans << endl;

    return 0;
}
