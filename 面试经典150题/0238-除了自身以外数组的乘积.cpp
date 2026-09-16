// 题号:0238  题名:除了自身以外数组的乘积  难度:中等
// 链接:https://leetcode.cn/problems/product-of-array-except-self/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己敲一遍,修「空 vector 下标写、递推起点 i=0 应为 1」两处后 AC)
//
// 思路:
//   暴力对每个 i 把其余 n-1 个数重乘一遍是 O(n²),n 到 1e5 会超。观察
//   answer[i] = 「i 左边全部的乘积」×「i 右边全部的乘积」:两半各自对相邻
//   下标只差乘一个数 → 都能递推,pre[i] = pre[i-1]*nums[i-1],右积同理从右
//   往左(前缀和的乘法同胞「前缀积」;递推复用与 DP 转移同源)。空间再压:
//   输出数组白送 → 第一趟直接把左积写进 answer,右积不建数组,第二趟从右
//   往左用一个滚动变量 suffix,先乘进 answer[i] 再吞 nums[i]——进阶的 O(1)
//   额外空间顺带就是它。除法路(总乘积÷nums[i])除被题面禁外还有两个坑:
//   0 导致除零、总乘积不保证在 int 内(题面只保证"去掉一个元素"的乘积)。
// 复杂度:
//   时间 O(n)(两趟递推)  空间 O(1) 额外(输出数组不计)
// 坑:
//   1. 空乘积 = 1 不是 0:answer[0] 与 suffix 初值都是 1。加法版前缀和的
//      "空和 = 0" 换成乘法,单位元要跟着换;初值取 0 会全盘归零。
//   2. 第二趟两行顺序不能反:乘进 answer[i] 时 suffix 必须仍是「i 右边的
//      乘积」(不含 nums[i]);先 suffix *= nums[i] 再乘就把自身也算进去了。
//   3. 0 无需特判:0 入列后其后的左积全变 0,两趟递推自然得出示例 2 的
//      [0,0,9,0,0]——唯一非 0 位恰是 0 所在位,左右积都不含它。
//   4. int 不溢出的底气:全程中间量要么是某段左积/右积,要么就是 answer
//      本身,都在题面保证的 32 位范围内;唯独"总乘积"不在保证之列(n=2
//      的 [1e5,1e5] 总乘积 1e10 爆 int),这也是不走除法的第二个理由。
//   5. 自己敲时复发 45 的老坑:`vector<int> answer;` 后直接 `answer[0]=1`,
//      空 vector 下标写 = UB,operator[] 不检查不扩容;构造时定长
//      `vector<int> answer(n)`(语言参考「空 vector 下标」标本,二次复发)。
//   6. 递推循环体用 i-1,起点却写 i=0:读 answer[-1]/nums[-1] 越界,还把刚
//      设好的 base case 覆盖掉。依赖 i-1 的递推,起点在 base case 下一位
//      (此处 i=1);274 的 citations[k-1] 差一位同族。
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> answer(n);
        answer[0] = 1;                      // 空乘积 = 1(乘法单位元)
        for (int i = 1; i < n; i++)
        {
            answer[i] = answer[i-1] * nums[i-1];
        }
        int suffix = 1;                     // suffix = i 右边的乘积,从右滚过来
        for (int i = n - 1; i >= 0; i--)
        {
            answer[i] *= suffix;            // 此刻 suffix 尚不含 nums[i]
            suffix *= nums[i];              // 吞入 nums[i],供更左边的位使用
        }
        return answer;
    }
};
