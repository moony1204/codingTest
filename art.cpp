#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <cmath>
#include <map>

using namespace std;

int n, m;
vector<vector<int>> arr;

vector<set<pair<int, int>>> groups;
vector<int> nums;


int ans=0;
void bfs(int row, int col, vector<vector<int>> &v);

int dy[]={-1,0,1,0};
int dx[]={0,1,0,-1};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n;

    arr.resize(n, vector<int>(n, 0));
    

    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            cin >> arr[i][j];
        }
    }

    m = n/2; // 삽질을 하지 말자 ~~~ !!!!!

    for (int k=0; k<4; ++k){
        // 예술점수 구하기 bfs
        groups.clear(); // 그룹 재조직 (회전햇으니까)
        nums.clear(); // 점수도 재조직  
        // v 쓰임 as visited
        
        vector<vector<int>> v(n, vector<int>(n, 0));

        // grouping
        for (int i=0; i<n; ++i){
            for (int j=0; j<n; ++j){
                if (v[i][j] == 0) {
                    // unvisited
                    groups.push_back(set<pair<int, int>>()); // 새로운 그룹 추가
                    nums.push_back(arr[i][j]); // 그룹 숫자 추가
                    bfs(i, j, v); 
                }
            }
        }

        // 점수 추가
        int cnt = nums.size(); // 그룹의 수
        for (int i=0; i<cnt-1; ++i){
            for( int j=i+1; j<cnt; ++j){
                int point = (groups[i].size() + groups[j].size()) * nums[i] * nums[j];
                //               그룹 칸수           그룹 칸수        그룹숫자   그룹숫자
                // => 한 인접면당 더해질 점수
                for (const auto& p: groups[i]){
                    int r=p.first;
                    int c=p.second;

                    for (int d=0; d<4; d++){
                        int new_r = r+dy[d];
                        int new_c = c+dx[d];
                        if (groups[j].count(make_pair(new_r, new_c))>0){
                            ans += point;
                        }
                    }
                }
            }
        }
        if (k==3){
            break;
        } // 마지막 회전은 필요없으
        // 회전시키기 

        // narr 사용
        vector<vector<int>> narr(n, vector<int>(n, 0));
        // 십자가 
        for (int i=0; i<n; i++){
            narr[m][i] = arr[i][m];
        }
        for (int i=0; i<n; i++){
            narr[i][m] = arr[m][n-i-1];
        }

        // 십자파트 
        vector<pair<int, int>> startpoints = {
            make_pair(0,0),
            make_pair(0, m+1),
            make_pair(m+1, 0), 
            make_pair(m+1, m+1)
        };

        for (const auto& start: startpoints){
            int s_row = start.first;
            int s_col = start.second;

            for (int i=0; i<m; ++i){
                for (int j=0; j<m; ++j){
                    narr[s_row+i][s_col+j] = arr[s_row + m - j - 1][s_col+i];
                }
            }
        }
        arr = narr;

    }
    
    cout << ans;

    return 0;
}

void bfs(int row, int col, vector<vector<int>> &v){
    // 큐 셋팅
    queue<pair<int, int>> q;
    q.push({row, col});
    v[row][col] = 1;
    groups.back().insert({row, col}); // 마지막 그룹 (새로운 그룹)에 인덱스 추가
    

    while(!q.empty()){
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        for (int d=0; d<4; d++){
            int new_r = r+dy[d];
            int new_c = c+dx[d];

            // new_r와 new_c 의 범위 체크 먼저 하고 arr/v 접근시켜야 함
            if (new_r >=0 && new_r <n && new_c >=0 && new_c < n){
                if (v[new_r][new_c] == 0 && arr[new_r][new_c] == arr[r][c]){
                    q.push({new_r, new_c});
                    v[new_r][new_c] = 1;
                    groups.back().insert({new_r, new_c});
                }

            } 
                
        }
    }
    
}
