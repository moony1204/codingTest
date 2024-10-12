#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

int ans=0;
int R, C, K;
vector<vector<int>> forest;
vector<vector<int>> golrem;
int golrem_num=2;

int dy[]={-1,0,1,0};
int dx[]={0,1,0,-1};

set<pair<int, int>> exit_set;

int bfs(int row,int col);

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    
    cin >> R >> C >> K ;
    golrem.resize(K, vector<int>(2));
    forest.resize(R+4, vector<int>(C+2, 0));
    
    for(int i=0; i<K; ++i){
        cin >> golrem[i][0] >> golrem[i][1] ;
    }

    for(int i=0; i<R+3; ++i){
        forest[i][0] = 1; //왼쪽벽 1로 채우기
        forest[i][C+1] = 1; //오른쪽 벽 1로 채우기
    }
    for (int i=0; i<C+2; ++i){
        forest[R+3][i] = 1; //바닥 1로 채우기
    }

    int golrem_col, golrem_row, dir;
    // 골렘과 함께 이동하기
    for(int i=0; i<K; i++){
        golrem_col = golrem[i][0];
        golrem_row = 1;
        dir = golrem[i][1];

        while(true){
            // 하강 전진. 1순위
            if (forest[golrem_row+1][golrem_col-1]+ forest[golrem_row+1][golrem_col+1]+forest[golrem_row+2][golrem_col]==0){
                golrem_row++;
            }
            // 서쪽 회전 하강
            else if(forest[golrem_row+1][golrem_col-1]+ forest[golrem_row-1][golrem_col-1]+forest[golrem_row][golrem_col-2]
                    + forest[golrem_row+1][golrem_col-2]+ forest[golrem_row+2][golrem_col-1]==0){
                golrem_col--;
                golrem_row++;
                dir = (dir+3)%4;
            }
            // 동쪽 회전 하강
            else if(forest[golrem_row+1][golrem_col+1]+ forest[golrem_row-1][golrem_col+1]+forest[golrem_row][golrem_col+2]
                    + forest[golrem_row+1][golrem_col+2]+ forest[golrem_row+2][golrem_col+1]==0){
                golrem_col++;
                golrem_row++;
                dir = (dir+1)%4;
            }else {break; }// 하강 끝
        }

        if (golrem_row<4){ // 내려갔어도 숲에 진입 못함

            // 숲 싹 지우기
            forest = vector<vector<int>>(R+4, vector<int>(C+2, 0));
            for(int i=0; i<R+3; ++i){
                forest[i][0] = 1; //왼쪽벽 1로 채우기
                forest[i][C+1] = 1; //오른쪽 벽 1로 채우기
            }
            for (int i=0; i<C+2; ++i){
                forest[R+3][i] = 1; //바닥 1로 채우기
            }
            golrem_num=2; // 골렘번호 초기화

            exit_set.clear();


        } else { // 내려갈만큼 내려감
            // 숲에 골렘 표시
            forest[golrem_row][golrem_col] = forest[golrem_row+1][golrem_col]=forest[golrem_row-1][golrem_col]=forest[golrem_row][golrem_col-1]=forest[golrem_row][golrem_col
            +1]=golrem_num;
            golrem_num++;

            exit_set.insert({golrem_row+dy[dir], golrem_col+dx[dir]});
            ans += bfs(golrem_row, golrem_col);

        }
        
    }

    cout << ans;
    return 0;
}

int bfs(int row, int col){
    queue<pair<int, int>> q;
    vector<vector<int>> v(R+4, vector<int>(C+2, 0));

    int mx_i=0;

    q.push({row, col});
    v[row][col] = 1; // visited check

    while (!q.empty()){
        int r = q.front().first;
        int c = q.front().second;
        q.pop(); //front 빼기

        mx_i = max(mx_i, r); // 현재 max와 queue 속 row 비교

        for(int d=0; d<4; d++){
            int new_r = r+dy[d];
            int new_c = c+dx[d];
            // 방문하지 않았고!
            if (v[new_r][new_c]==0 && 
            (forest[r][c]==forest[new_r][new_c] || (exit_set.count({r, c}) && forest[new_r][new_c] > 1))){
            // 숫자가 같은 골렘이거나                   해당 위치가 출구이고 다음 위치가 골렘이라면
                q.push({new_r, new_c}); // 큐에 뉴 위치 추가
                v[new_r][new_c] = 1; // 이동 체크
            }
        }

    }

    return mx_i-2;
}
