// 题号:0169  题名:多数元素  难度:简单
// 链接:https://leetcode.cn/problems/majority-element/?envType=study-plan-v2&envId=top-interview-150
// 状态:看过提示(计数思路自己想的;vector→map 的容器纠错与一趟写法来自提示)
//
// 思路:
//   哈希计数:unordered_map<值,次数> 边扫边加,次数一旦 > n/2 立即返回——
//   过半的元素最多一个,且它一旦在前缀中过半,后面只增不减,不可能被反超。
// 复杂度:
//   时间 O(n)  空间 O(n)
// 坑:
//   1. 起手想 vector 计数:值域 ±10⁹ 且有负数,值做不了下标;空 vector 的
//      count[nums[i]]++ 直接越界崩。数组计数只在「值域小且非负」时可用。
//   2. 第二趟按 j<count.size() 扫 map:operator[] 访问即插入,每圈插一个新键,
//      size 与 j 同步涨,循环永不终止 → MLE(48/54,最后用例多数元素是 -10⁹,
//      j 永远撞不上它;此前小非负值的用例全是侥幸通过)。map 的 size 是
//      「不同键个数」不是值域宽度;遍历要用 for (auto& [val,cnt] : count)。
// 进阶:
//   摩尔投票法 O(n)/O(1):不同值两两抵消,多数元素超过半数必存活到最后。
//   本题未写,二刷补(candidate + count 抵消)。
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int,int>count;
        for(int i=0;i<n;i++)
        {
            if(++count[nums[i]]>n/2)
            {
                return nums[i];
            }
        }
        return 0;
    }
};
