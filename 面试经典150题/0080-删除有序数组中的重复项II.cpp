// 题号:0080  题名:删除有序数组中的重复项 II  难度:中等
// 链接:https://leetcode.cn/problems/remove-duplicates-from-sorted-array-ii/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自创「间隙跳坑法」WA 一次后推倒重来)
//
// 思路:
//   快慢指针原地压实,slow = 已保留个数 = 下一个写入位置。有序 ⇒ 重复必相邻 ⇒
//   只和倒数第 2 个保留值 nums[slow-2] 比:相等 ⇒ 末尾已两份,拒;不等 ⇒ 收。
//   统一公式:留 k 份 ⇔ 和倒数第 k 个保留值比 ⇔ slow 从 k 起步(27/26/80 同骨架)。
// 复杂度:
//   时间 O(n)  空间 O(1)
// 坑:
//   slow 从 2 起步,n=1 时循环不跑、直接 return 2,越过数组 ⇒ 判题机按长度 2
//   打印出数组外的垃圾(输入 [1] 输出 [1,0]),WA 165/170。
//   开头必须特判 if (nums.size() <= 2) return nums.size();(26 从 1 起步,靠 n≥1 约定兜住,不用)
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() <= 2) return nums.size();
        int slow=2;
        for(int fast=2;fast<nums.size();fast++)
        {
            if(nums[fast]!=nums[slow-2])
            {
                nums[slow++]=nums[fast];
            }

        }
        return slow;
    }
};
