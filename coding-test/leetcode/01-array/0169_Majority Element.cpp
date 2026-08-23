// 0169. Majority Element - 대표 풀이 3가지

// 1. Hash Table (`O(n)`, `O(n)`)
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> cnt;
        int ans = 0, maxCnt = 0;

        for (int num : nums) {
            cnt[num]++;
            if (cnt[num] > maxCnt) {
                maxCnt = cnt[num];
                ans = num;
            }
        }

        return ans;
    }
};

// 2. Sorting (`O(n log n)`, `O(1)`~`O(log n)`)
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        return nums[nums.size() / 2];
    }
};

//3. Boyer–Moore Voting Algorithm (`O(n)`, `O(1)`)
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0;
        int count = 0;

        for (int num : nums) {
            if (count == 0)
                candidate = num;

            if (num == candidate)
                count++;
            else
                count--;
        }

        return candidate;
    }
};