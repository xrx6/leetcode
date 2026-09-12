// 题号:0045  题名:跳跃游戏II  难度:中等
// 链接:https://leetcode.cn/problems/jump-game-ii/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(题解分层拆解后理解,待自己敲一遍 AC)
//
// 思路:
//   "最少几步" + 每跳代价相同 → 无权图最短路 → BFS 分层。按跳数分层:
//   0 跳到 {0},1 跳到 [1..e1],2 跳到 [e1+1..e2]……每层是连续区间(55 同款
//   事实:k 跳能到 i,最后一跳缩短则 k 跳能到任何更前的格,故 dp[i] 非降,
//   dp 表是阶梯,如 [0,1,1,2,2])。答案 = 阶梯涨几级。贪心 = 不填阶梯,
//   只找每级右边界:end=当前层右边界(steps 跳可达),farthest=下一层右边
//   界;扫到 i==end 说明当前层扫完,steps++,end=farthest。
// 复杂度:
//   时间 O(n)  空间 O(1)  (DP 基线 O(n^2):dp[i] = min(dp[j]+1 | j<i 且 j+nums[j]>=i))
// 坑:
//   1. 循环到 i<n-1 为止,不扫最后一格:站上 n-1 就结束,不需要从它再跳。
//      扫进去的话示例1会在 i=4 触发多余的 steps++,答 3 错。(55 扫到 n-1
//      没事:reach 多算不改变 true;计数则会多。)
//   2. farthest 的更新须在 i==end 判定之前:i==end 的格子还属当前层,跳力
//      要先记入下一层边界;反序的话 i=0 时 end=farthest=0,原地卡死。
//   3. n==1 循环不进,返回 0 步,无需特判;题目保证可达,无需 55 的失败检查。
class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        int steps = 0;      // 已跳层数
        int end = 0;        // 当前层的右边界
        int farthest = 0;   // 下一层的右边界
        for (int i = 0; i < n - 1; i++)
        {
            farthest = max(farthest, i + nums[i]);
            if (i == end)
            {
                steps++;
                end = farthest;
            }
        }
        return steps;
    }
};
