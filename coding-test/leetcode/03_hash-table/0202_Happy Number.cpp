class Solution {
public:
    int getNext(int n) {
        int sum = 0;

        while (n > 0) {
            int digit = n % 10;
            n /= 10;
            sum += digit * digit;
        }

        return sum;
    }

    bool isHappy(int n) {
        unordered_set<int> seen;

        while (n != 1) {
            if (seen.count(n)) {
                return false;
            }

            seen.insert(n);
            n = getNext(n);
        }

        return true;
    }
};

// https://leetcode.cn/problems/happy-number/description/