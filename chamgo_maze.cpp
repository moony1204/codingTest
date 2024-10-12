#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int N, M, K;
vector<vector<int>> arr;
int ei, ej;

pair<int, int> find_exit() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (arr[i][j] == -11) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // 비상구를 찾지 못했을 경우
}

tuple<int, int, int> find_square() {
    int mn = N;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (-11 < arr[i][j] && arr[i][j] < 0) { // 사람인 경우
                mn = min(mn, max(abs(ei - i), abs(ej - j)));
            }
        }
    }

    for (int si = 0; si <= N - mn; ++si) {
        for (int sj = 0; sj <= N - mn; ++sj) { // 가능한 모든 시작 위치
            if (si <= ei && ei <= si + mn && sj <= ej && ej <= sj + mn) { // 비상구가 포함된 사각형
                for (int i = si; i <= si + mn; ++i) {
                    for (int j = sj; j <= sj + mn; ++j) {
                        if (-11 < arr[i][j] && arr[i][j] < 0) { // 사람인 경우 리턴!
                            return {si, sj, mn + 1};
                        }
                    }
                }
            }
        }
    }
    return {-1, -1, -1}; // 사각형을 찾지 못한 경우
}

int main() {
    cin >> N >> M >> K;
    arr.resize(N, vector<int>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> arr[i][j];
        }
    }

    for (int _ = 0; _ < M; ++_) {
        int i, j;
        cin >> i >> j;
        arr[i - 1][j - 1] -= 1; // 사람은 -1 (0-based index)
    }

    cin >> ei >> ej;
    arr[ei - 1][ej - 1] = -11; // 비상구 -11

    long long ans = 0;
    int cnt = M;

    for (int _ = 0; _ < K; ++_) {
        // [1] 모든 참가자 이동
        vector<vector<int>> narr = arr; // 새로운 배열 복사

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (-11 < arr[i][j] && arr[i][j] < 0) { // 사람인 경우
                    int dist = abs(ei - i) + abs(ej - j);
                    // 네 방향 (상하우선), 범위 내, 벽이 아니고 <=0, 거리가 dist보다 작으면
                    for (pair<int, int> dir : {make_pair(-1, 0), make_pair(1, 0), make_pair(0, -1), make_pair(0, 1)}) {
                        int ni = i + dir.first, nj = j + dir.second;
                        if (0 <= ni && ni < N && 0 <= nj && nj < N && arr[ni][nj] <= 0 && dist > (abs(ei - ni) + abs(ej - nj))) {
                            ans += arr[i][j]; // 현재 인원수가 이동하는 것이니 이동거리에 누적
                            narr[i][j] -= arr[i][j]; // 이동 처리
                            if (arr[ni][nj] == -11) { // 비상구인 경우
                                cnt += arr[i][j]; // 탈출!
                            } else { // 일반 빈칸 또는 사람 있는 자리
                                narr[ni][nj] += arr[i][j]; // 들어온 인원 추가
                            }
                            break;
                        }
                    }
                }
            }
        }

        arr = narr; // 배열 업데이트
        if (cnt == 0) {
            break;
        }

        // [2] 미로 회전
        int si, sj, L;
        tie(si, sj, L) = find_square();

        narr = arr; // 배열 복사
        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < L; ++j) {
                narr[si + i][sj + j] = arr[si + L - 1 - j][sj + i];
                if (narr[si + i][sj + j] > 0) { // 벽이면 회전 시 1 감소
                    narr[si + i][sj + j] -= 1;
                }
            }
        }
        arr = narr; // 배열 업데이트
        tie(ei, ej) = find_exit(); // 비상구 위치 저장
    }

    cout << -ans << endl; // 총 거리의 음수 출력
    cout << ei + 1 << " " << ej + 1 << endl; // 비상구 위치 출력 (1-based index)
    return 0;
}
