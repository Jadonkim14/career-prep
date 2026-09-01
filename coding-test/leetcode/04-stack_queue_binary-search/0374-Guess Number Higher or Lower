// 공식: 정답이 들어 있는 경계를 좁혀가는 이분 탐색

/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is higher than the picked number
 *			      1 if num is lower than the picked number
 *               otherwise return 0
 * int guess(int num);
 */
class Solution {
public:
    int guessNumber(int n) {
        int left = 1, right = n;
        while (left < right) { // 循环直至区间左右端点相同
            int mid = left + (right - left) / 2; // 防止计算时溢出
            if (guess(mid) <= 0) {
                right = mid; // 答案在区间 [left, mid] 中
            } else {
                left = mid + 1; // 答案在区间 [mid+1, right] 中
            }
        }
        // 此时有 left == right，区间缩为一个点，即为答案
        return left;
    }
};

作者：力扣官方题解
链接：https://leetcode.cn/problems/guess-number-higher-or-lower/solutions/824520/cai-shu-zi-da-xiao-by-leetcode-solution-qdzu/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


// 내 풀이: 정답을 직접 찾는 이분 탐색
class Solution {
public:
    int guessNumber(int n) {
        int left = 1, right = n;
        while(left <= right){
            int ans = left + (right - left) / 2;
            int result = guess(ans);

            if(result == 0)
                return ans;

            else if(result == -1)
                right = ans - 1;
            
            else if(result == 1)
                left = ans + 1;            
        }
        return -1;
    }
};