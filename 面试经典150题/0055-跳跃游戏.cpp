// 题号:0055  题名:跳跃游戏  难度:中等
// 链接:https://leetcode.cn/problems/jump-game/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(题解逐步拆解后理解,待自己敲一遍 AC)
//
// 思路:
//   只问"能不能到",不问"怎么到"→ 不用记路线,不用枚举走法。关键事实:
//   能到达的格子必是从 0 起连成一片的连续区间——去任何格子 j 的路径总有
//   "最后一跳",把它缩短一点即可落在途中任意格。所以可达集合的全部信息
//   = 右边界。贪心维护 reach:站得上的格子(i<=reach)用它推远边界
//   reach = max(reach, i+nums[i]);一旦 i>reach 即出现永远补不上的缺口,
//   返回 false;reach>=n-1 则 true。
//   本质是 DP 的坍缩:dp[i](是否可达)数组长这样 [真真真...真假假...],
//   一条分界线,整表压成一个变量;且 reach 是所有走法的并集边界,没有
//   丢弃任何路——零风险贪心,区别于"砍树"型贪心(如找零钱)。
// 复杂度:
//   时间 O(n)  空间 O(1)  (DP 基线 O(n^2):dp[i] = 存在 j<i, dp[j] 且 j+nums[j]>=i)
// 坑:
//   1. i>reach 的判断必须在更新 reach 之前——格子要先"确认站得上去"才有
//      资格贡献跳力。反例 [0,100]:先更新的话会拿站不上去的下标 1 的 100
//      作弊,误判 true(正确答案 false)。
//   2. n==1 起点即终点:i=0 时 reach>=0=n-1 天然返回 true,无需特判。
//   3. i+nums[i] 最大约 1e4+1e5,int 不溢出。
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = nums.size(), reach = 0;
        for (int i = 0; i < n; i++)
        {
            if (i > reach) return false;
            reach = max(reach, i + nums[i]);
            if (reach >= n - 1) return true;
        }
        return true;
    }
};
