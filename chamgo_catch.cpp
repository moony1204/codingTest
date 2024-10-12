#include <iostream>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

int main() {
    int N, MM, H, K;
    cin >> N >> MM >> H >> K;

    // 도망자 좌표 입력
    vector<vector<int>> arr(MM, vector<int>(3));
    for (int i = 0; i < MM; i++) {
        cin >> arr[i][0] >> arr[i][1] >> arr[i][2];
    }

    // 나무좌표 입력
    set<pair<int, int>> tree;
    for (int i = 0; i < H; i++) {
        int x, y;
        cin >> x >> y;
        tree.insert({x, y});
    }

    // 방향  상 우 하 좌
    int di[] = {0, 0, 1, -1};
    int dj[] = {-1, 1, 0, 0};
    int opp[] = {1, 0, 3, 2};  // 반대방향

    // 방향  상 우 하 좌
    int tdi[] = {-1, 0, 1, 0};
    int tdj[] = {0, 1, 0, -1};

    int mx_cnt = 1, cnt = 0, flag = 0, val = 1;
    int M = (N + 1) / 2;
    int ti = M, tj = M, td = 0;

    int ans = 0;
    for (int k = 1; k <= K; k++) {  // K턴만큼 게임 진행
        // [1] 도망자의 이동(arr)
        for (int i = 0; i < arr.size(); i++) {
            if (abs(arr[i][0] - ti) + abs(arr[i][1] - tj) <= 3) {  // 술래와 거리 3이하인 경우 이동
                int ni = arr[i][0] + di[arr[i][2]];
                int nj = arr[i][1] + dj[arr[i][2]];
                if (1 <= ni && ni <= N && 1 <= nj && nj <= N) {  // 범위내면 술래체크
                    if (ni != ti || nj != tj) {  // 술래위치가 아니면 이동
                        arr[i][0] = ni;
                        arr[i][1] = nj;
                    }
                } else {  // 범위밖=>방향 반대
                    arr[i][2] = opp[arr[i][2]];  // 반대 방향전환 및 저장
                    ni = arr[i][0] + di[arr[i][2]];
                    nj = arr[i][1] + dj[arr[i][2]];
                    if (ni != ti || nj != tj) {
                        arr[i][0] = ni;
                        arr[i][1] = nj;
                    }
                }
            }
        }

        // [2] 술래의 이동
        cnt++;
        ti += tdi[td];
        tj += tdj[td];
        if (ti == 1 && tj == 1) {  // 안쪽으로 동작하는 달팽이
            mx_cnt = N;
            cnt = 1;
            flag = 1;
            val = -1;
            td = 2;  // 초기방향은 아래로(하)
        } else if (ti == M && tj == M) {  // 바깥으로 동작하는 달팽이
            mx_cnt = 1;
            cnt = 0;
            flag = 0;
            val = 1;
            td = 0;
        } else {
            if (cnt == mx_cnt) {  // 방향 변경
                cnt = 0;
                td = (td + val) % 4;
                flag = (flag == 0) ? 1 : 0;  // 두 번에 한 번씩 길이 증가
                if (flag == 0) {
                    mx_cnt += val;
                }
            }
        }

        // [3] 도망자 잡기(술래자리 포함 3칸: 나무가없는 도망자면 잡힘!)
        set<pair<int, int>> tset = {
            {ti, tj},
            {ti + tdi[td], tj + tdj[td]},
            {ti + tdi[td] * 2, tj + tdj[td] * 2}
        };
        for (int i = arr.size() - 1; i >= 0; i--) {
            if (tset.count({arr[i][0], arr[i][1]}) && tree.count({arr[i][0], arr[i][1]}) == 0) {
                arr.erase(arr.begin() + i);
                ans += k;
            }
        }

        // 도망자가 없다면 더이상 점수도 없음
        if (arr.empty()) {
            break;
        }
    }

    cout << ans << endl;
    return 0;
}
