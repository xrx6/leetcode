// 题号:0042  题名:接雨水  难度:困难
// 链接:https://leetcode.cn/problems/trapping-rain-water/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己先试:邻居落差两趟取min,[4,2,0,3,2,5] 反例 5≠9;自己敲一遍,一遍过 AC——倒扫顺手累加并成一趟)
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
//   时间 O(n)  空间 O(n)(右墙数组可退化成滚动变量 → O(n) 单数组;双指针/
//   单调栈可压 O(1)/O(n) 另一路径,进阶方向)
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
//   5. 边界:n==1 单柱返回 0(倒扫循环进不去)。自敲版删了 n==0 判空:
//      rightmax[n-1] 在空数组上就是 45/238 的「空 vector 下标」标本,
//      题面 1<=n 是唯一护栏——换到允许空输入的场合要把判空加回来。
//   6. 自敲并趟:倒扫里 rightmax[i] 算完当刻就参与累加,第二、三趟并成
//      一趟;i=n-1 贡献恒 0,循环从 n-2 起步自然略去它。右墙只依赖
//      rightmax[i+1],数组还能退化成滚动变量(238 的 suffix 同款,
//      O(2n)→O(n));左墙退不掉——min 合并要在同一刻拿到两侧,必须有
//      一侧物化好。再往 O(1) 压就是双指针变体。

class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        int c = 0;
        vector<int> leftmax(n, 0);
        vector<int> rightmax(n, 0);
        leftmax[0] = height[0];                            // 左边最高的墙(含己)
        rightmax[n-1] = height[n-1];                       // 右边最高的墙(含己)
        for (int i = 1; i < n; i++)
        {
            leftmax[i] = max(leftmax[i-1], height[i]);
        }
        for (int i = n-2; i >= 0; i--)                     // 倒扫:算右墙顺手累加,并成一趟
        {
            rightmax[i] = max(rightmax[i+1], height[i]);
            c += min(leftmax[i], rightmax[i]) - height[i];  // 水位-柱高
        }
        return c;                                          // i=n-1 水恒 0,从 n-2 起即略去
    }
};
