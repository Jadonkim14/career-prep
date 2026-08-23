class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = 1e9, maxPrf = 0;
        for(int i=0; i<prices.size(); i++){
            maxPrf = max(maxPrf, prices[i] - minPrice);
            minPrice = min(minPrice, prices[i]);
        }
        return maxPrf;
    }
};