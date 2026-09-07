// 链接:https://leetcode.cn/problems/remove-element/?envType=study-plan-v2&envId=top-interview-150
// 状态:独立完成 | 看过提示 | 看题解后理解
// 思路:
//   一两句话,讲清"怎么想到的",不是复述代码做了什么
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int *end=nums.data()+nums.size();
        int *j=nums.data();
        int *i=nums.data();
        while(j<(nums.data()+nums.size()))
        {
            if(*j!=val)
            {
                *(i++)=*j;
            }
            j++;
        }
        return i-nums.data();
    }
};