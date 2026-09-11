// 题号:0121  题名:买卖股票的最佳时机  难度:简单
// 链接:https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(一趟维护历史最低价的思路由分层提示推出,定稿为官方题解的范围 for 写法;AC)
//
// 思路:
//   固定卖出日 j,它唯一该配的买入价是左边历史最低 min(prices[0..j-1]),而前缀
//   最小值可以一趟增量维护 ⇒ 一遍扫只维护两个变量:minprice(历史最低)和
//   maxprofit(答案),每天问一句"如果今天卖最多赚多少"。所有交易按卖出日分类
//   不重不漏,每个卖出日都配上它能配的最优买入,全局最大不会漏。
// 复杂度:
//   时间 O(n)  空间 O(1)
// 坑:
//   1. 先算 profit 再更新 minprice:今天卖只能配"今天之前"的最低价。两行换序结果
//      其实一样(自己减自己是 0,赢不了 maxprofit),但这个顺序不用想该特例。
//   2. minprice 初值 1e9 是"无穷大"哨兵(价格 ≤10⁵ 永远够小);1e9 是 double 字面量,
//      初始化给 int 时隐式截断,更严谨是 <climits> 的 INT_MAX,或 prices[0] 起步从
//      下标 1 扫。maxprofit 初值 0 天然覆盖"一路跌不交易",无需特判。
//   3. for (int price : prices) 范围 for:price 是每个元素的拷贝,改它不影响数组;
//      循环体只要值、不要下标时用它,没有 i<n 可写错。要改元素写 int&,只读大对象
//      写 const auto&。169 修 MLE 的 for (auto& [val,cnt] : count) 就是它+结构化绑定。
// 进阶:
//   本题是股票系列状态机 DP 的退化版:dp[i][持有/不持有],本题限买一次 ⇒ 状态里
//   "持有"只能从 -prices 转移。122 可多次、123/188 限 k 次、309 冷冻期、714 手续费,
//   同一台状态机换约束,二刷串成一条线。
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int inf = 1e9;
        int minprice = inf, maxprofit = 0;
        for (int price: prices) {
            maxprofit = max(maxprofit, price - minprice);
            minprice = min(price, minprice);
        }
        return maxprofit;
    }
};
