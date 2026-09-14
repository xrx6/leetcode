// 题号:0274  题名:H指数  难度:中等
// 链接:https://leetcode.cn/problems/h-index/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(降序版+桶计数版逐步拆解后理解,待自己敲一遍 AC)
//
// 思路:
//   h = 最大的 h 使「至少 h 篇被引 ≥ h」(篇数与被引数的双门槛,同一数)。
//   定义 f(h) = 被引 ≥ h 的篇数,h 越大门槛越高,f 单调不增 → 从大往小首个
//   成立的即答案,判定不必每个 h 重数。降序排序后判定塌缩成一句:
//   「至少 k 篇 ≥ k」⟺「降序第 k 篇 ≥ k」——第 k 高的都 ≥ k 则前 k 篇全
//   ≥ k;反之能 ≥ k 的最多 k-1 篇。直觉:柱高=被引、从高到低排,h 是能塞进
//   阶梯左上角的最大正方形边长。break 合法性也是单调:值只降 k 只涨。
// 复杂度:
//   时间 O(n log n)(排序主导)  空间 O(1)
// 坑:
//   1. h 上限是 n(总共只有 n 篇):[1000] 的答案是 1 不是 1000——「篇数」和
//      「被引数」是两个量纲;桶计数必须 min(c,n) 截断。
//   2. 降序判定是 1-based:「第 k 篇 ≥ k」,代码写 citations[k-1] >= k,两个
//      k 差一位,最易手滑;升序写法(找首个 citations[i] >= n-i)同理易差一。
//   3. h 可以为 0(全 0 论文),h 初始 0 兜底,无需特判。
// 进阶:
//   桶计数 O(n):值域 ≤1000 且非负(169 同款信号)。cnt[v] = 恰好被引 v 次的
//   篇数(>n 截进 cnt[n]),从 h=n 往下 total += cnt[h],total 恰为 f(h)(「≥h
//   = 桶 h 及之后所有桶之和」,每降一档添一桶,免重算),首个 total ≥ h 返回。
//   排序版答「第 k 高是谁」,计数版答「≥h 有几篇」,判定 f(h) ≥ h 不变。
//   275(输入已升序) = 升序判定套二分,在题单二分查找区。
class Solution {
public:
    int hIndex(vector<int>& citations) {
        sort(citations.begin(), citations.end(), greater<int>());
        int h = 0;
        for (int k = 1; k <= (int)citations.size(); k++)
        {
            if (citations[k-1] >= k) h = k;
            else break;
        }
        return h;
    }
};
