class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> s;
        for (int x: nums) {
            if (s.find(x) != s.end()) {
                return true;
            }
            s.insert(x);
        }
        return false;
    }
};

作者：力扣官方题解
链接：https://leetcode.cn/problems/contains-duplicate/solutions/518991/cun-zai-zhong-fu-yuan-su-by-leetcode-sol-iedd/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。