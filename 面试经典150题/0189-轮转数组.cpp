// 题号:0189  题名:轮转数组  难度:中等
// 链接:https://leetcode.cn/problems/rotate-array/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(三次翻转来自官方题解+评论区 1.4K 赞箭头图;AC 0ms)
//
// 思路:
//   右移 k ⇒ 尾部 k 个搬去开头、其余整体后移 ⇒ 本质是两段换位:[A|B] → [B|A]。
//   两段不等长,逐个搬会互相覆盖;「翻转」是 O(1) 空间里现成的整体搬移操作,
//   连做三次凑出换位:整体翻 [A|B] → [rev(B)|rev(A)],B 已到位只差内部顺序,
//   再把前 k 个、剩余各自翻回 → [B|A]。
//   评论区箭头图:nums="----->-->"(k=3) 整体翻 "<--<-----",前 3 翻 "--><-----",
//   剩余翻 "-->----->" 即答案——翻转连箭头方向都替你画好了。
// 复杂度:
//   时间 O(n)  空间 O(1)  ← 进阶要求
// 坑:
//   k 可远大于 n(k≤10⁵ 而 n 可为 1):不先 k %= n,翻转区间按 k 算,越界/结果全错;
//   取模必须第一步做。取模后 k=0 时三个 reverse 全空转,天然安全,无需特判。
// 进阶:
//   官方题解另两种:① 新数组 ans[(i+k)%n]=nums[i],O(n) 空间;② 环状替换:从每个
//   起点沿 (i+k)%n 跳着放,共走 n 步填满所有环,同样 O(1) 空间。二刷补环状替换。
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;
        reverse(nums.begin(), nums.end());
        reverse(nums.begin(), nums.begin() + k);
        reverse(nums.begin() + k, nums.end());
    }
};
