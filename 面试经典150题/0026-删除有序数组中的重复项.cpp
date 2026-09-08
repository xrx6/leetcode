// 题号:0026  题名:删除有序数组中的重复项  难度:简单
// 链接:https://leetcode.cn/problems/remove-duplicates-from-sorted-array/?envType=study-plan-v2&envId=top-interview-150
// 状态:看过提示(先卡进 while+for 嵌套循环,回到 27 的快慢指针套路后一次 AC)
//
// 思路:
//   有序 ⇒ 重复必相邻 ⇒ 「留不留」只需和最后一个保留值 nums[slow] 比一次。
//   这里 slow = 最后一个保留下标(和 27 差一档):先走再写 nums[++slow],
//   返回 slow+1。与 27/80 的统一公式:留 k 份 ⇔ 和倒数第 k 个保留值比。
// 复杂度:
//   时间 O(n)  空间 O(1)
// 坑:
//   比较对象必须取自保留区(nums[slow]),不能写 nums[fast-1]——fast 前面的
//   格子可能已被覆盖成新值。返回是下标+1,别照抄 27 的 return slow。
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int slow=0;

        for(int fast=slow+1;fast<nums.size();fast++)
        {
            if(nums[fast]!=nums[slow])
            {
                nums[++slow]=nums[fast];
            }
        }
        return slow+1;
    }
};
