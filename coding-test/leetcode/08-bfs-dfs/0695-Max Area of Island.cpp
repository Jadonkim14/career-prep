// 方法一：深度优先搜索
class Solution {
private:
    int dr[4] = {-1, 0, 0, 1};
    int dc[4] = {0, -1, 1, 0};

    int visit(vector<vector<int>>& grid, int r, int c) {
        grid[r][c] = 0;
        int sizeIsl = 1;
        int gridRow = grid.size();
        int gridCol = grid[0].size();

        for (int i = 0; i < 4; i++) {
            int newRow = r + dr[i];
            if (newRow < 0 || newRow >= gridRow) continue;

            int newCol = c + dc[i];
            if (newCol < 0 || newCol >= gridCol) continue;

            if (grid[newRow][newCol] == 1) {
                sizeIsl += visit(grid, newRow, newCol);
            }
        }

        return sizeIsl;
    }

public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int row = grid.size();
        int col = grid[0].size();
        int maxArea = 0;
 
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (grid[i][j] == 1) {
                    int temp = visit(grid, i, j);
                    if (temp > maxArea) {
                        maxArea = temp;
                    }
                }
            }
        }

        return maxArea;
    }
};

// 方法二：广度优先搜索
class Solution {
private:
    int dr[4] = {-1, 0, 0, 1};
    int dc[4] = {0, -1, 1, 0};

    int bfs(vector<vector<int>>& grid, int r, int c) {
        int sizeIsl = 1;
        int gridRow = grid.size();
        int gridCol = grid[0].size();
        grid[r][c] = 0;

        queue<pair<int,int>> q;
        q.push({r, c});

        while (!q.empty()) {
            pair<int,int> cur = q.front();
            q.pop();

            for (int i = 0; i < 4; i++) {
                int newRow = cur.first + dr[i];
                int newCol = cur.second + dc[i];

                if (newRow < 0 || newRow >= gridRow
                || newCol < 0 || newCol >= gridCol) continue;

                if (grid[newRow][newCol] == 1) {
                    grid[newRow][newCol] = 0;
                    q.push({newRow, newCol});
                    sizeIsl++;
                }
            }
        }
        
        return sizeIsl;
    }

public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int row = grid.size();
        int col = grid[0].size();
        int maxArea = 0;
 
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (grid[i][j] == 1) {
                    int temp = bfs(grid, i, j);
                    if (temp > maxArea) {
                        maxArea = temp;
                    }
                }
            }
        }

        return maxArea;
    }
};