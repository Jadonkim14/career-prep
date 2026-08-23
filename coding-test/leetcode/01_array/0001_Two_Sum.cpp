#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> um;
        for(int i=0; i<nums.size(); i++){
            auto it = um.find(target - nums[i]);
            if(it != um.end()){
                return {it->second, i};
            }
            um[nums[i]] = i;
        }
        return {};
    }
};