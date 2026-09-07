// 题号:0027  题名:移除元素  难度:简单
// 链接:https://leetcode.cn/problems/remove-element/?envType=study-plan-v2&envId=top-interview-150
// 状态:独立完成
//
// 思路:
//   快慢指针原地抄写:fast 扫全数组,≠val 才让 slow 接收,返回 slow。
//   写指针永远追不上读指针,覆盖的都是废数据。用裸指针版练指针算术。
// 复杂度:
//   时间 O(n)  空间 O(1)
// 坑:
//   哨兵 end 只能比不能解引用;等于 val 的动作是跳过,不是处理
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