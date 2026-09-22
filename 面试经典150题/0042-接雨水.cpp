// 题号:0042  题名:接雨水  难度:困难
// 链接:https://leetcode.cn/problems/trapping-rain-water/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己先试:邻居落差两趟取min,[4,2,0,3,2,5] 反例 5≠9;待自己敲一遍 AC)
//
// 思路:
//   每根柱子上方能存的水 = 水位 - 柱高,而水位由「左右两侧最高的墙」夹住:
//   water[i] = min(leftMax[i], rightMax[i]) - height[i]。
//   leftMax[i] = max(height[0..i]) 是前缀最大(左边最高的墙,含自己);
//   rightMax[i] = max(height[i..n-1]) 是后缀最大。两端各扫一趟即可得到,
//   再逐点合并取 min、减去柱高求和。和 135/238 同属「从左一遍、从右一遍、
//   合并」骨架:238 合并用乘,135 用 max(两侧要求取更严),本题用 min
//   (水位不能超过较矮的那面墙)。挡水的是盆沿,不是隔壁柱子——原解只看
//   相邻落差,丢的是沿链累计的「最高墙」,和 135 数不等对丢链长同病。
// 复杂度:
//   时间 O(n)  空间 O(n)(双指针/单调栈可压到 O(1)/O(n) 另一路径,进阶方向)
// 坑:
//   1. 原解错因标本:water[i] ≈ min(左邻落差, 右邻落差)。邻居不是墙。
//      [4,2,0,3,2,5] 下标 2(height=0)左邻只给 2,正解 4——左边真正的墙
//      是下标 0 的 4,中间那根本身也在水下;下标 3(height=3)右邻 2 比它
//      矮、原解给 0,正解 1——右墙是 5。同题两向都错,总和 5≠9。
//   2. leftMax 含自己(初值 height[0],递推 max);若要「严格左边」的墙,
//      注意 i=0 没有左墙。合并时 min(leftMax,rightMax) 再减 height[i],
//      别忘了减柱高——柱子占的空间不是水。
//   3. 两端(i=0 / i=n-1)水恒为 0:一侧没有墙,水会从缺口流走。公式自动
//      给出 0(min 一侧是 height[i] 本身),不必特判,但要理解原因。
//   4. 累计的是「至今最高」不是「邻居差」:递推 leftMax[i]=max(leftMax[i-1],
//      height[i]) 才是链 DP;写成 height[i]-height[i-1] 就退回原解。
//   5. 边界:n==0 返回 0;n==1 单柱也返回 0(两趟循环进不去或合并为 0)。
//      本题 n>=1,习惯上仍可判空。

class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if (n == 0) return 0;

        vector<int> leftMax(n), rightMax(n);
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++)
            leftMax[i] = max(leftMax[i - 1], height[i]);   // 左边最高的墙(含己)

        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--)
            rightMax[i] = max(rightMax[i + 1], height[i]); // 右边最高的墙(含己)

        int c = 0;
        for (int i = 0; i < n; i++)
            c += min(leftMax[i], rightMax[i]) - height[i];  // 水位-柱高
        return c;
    }
};
