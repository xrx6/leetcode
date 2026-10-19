// 题号:0028  题名:找出字符串中第一个匹配项的下标  难度:简单
// 链接:https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己先试:单指针 i+已配长 s 一路向前,失配 s=0 硬归零,
//   mississippi/issip 反例 -1≠4,最小反例 aab/ab;待自己敲一遍 AC)
//
// 思路:
//   暴力=每个起点整段重比,朴素双循环 O(n·m)。原解想省掉外层起点循环:
//   把「已配到 needle 第几位」装进 s 随身携带,单指针一路向前——方向
//   对了一半(带状态省重复比较,正是 KMP 的出发点),但失配时 s=0 把
//   已比窗口整段丢弃,而下一个起点可能就藏在窗口内部(失配回退)。两条
//   正路:①肯回头(朴素):失配后起点 start+1,i 退回去重比,窗口信息
//   不记账;②不回头带账本(KMP):失配时窗口 [i-s,i-1]=needle[0..s-1]
//   已知相等,窗口内起点 p 有救 ⇔ 窗口末 i-p 格=needle 同长前缀(相等
//   前后缀),能救的最早 p 对应最长者 π[s-1] ⇒ s=π[s-1] 后当前字符再比,
//   更早起点由「最长」二字保证必败(下界证书)。失配去向只依赖 needle
//   ⇒ 预处理成 π 表(压表),主匹配零回退 O(n);造表=needle 对自己跑
//   同一个匹配循环(自己找自己),循环形状与主匹配完全相同。原解主循环
//   与 KMP 骨架逐步同构(含撞末字符 return i-s=i-m+1 的时机),只差
//   失配一行。
// 复杂度:
//   朴素 O(n·m)/O(1)(n,m≤1e4 最坏 1e8 次字符比较,放行但贴上限);
//   KMP O(n+m)/O(m)。
// 坑:
//   1. 原解错因标本:失配 s=0 且 i 照常前进=整段丢弃已比窗口。两层:
//      ①浅:失配的那个字符没和 needle[0] 重比就被消费,最小反例
//      "aab"/"ab"(输出 -1,应 1:i=1 'a' 失配归零,它本可当新起点);
//      ②深:补上重比仍不够——新起点可落在窗口内部,"mississippi"/
//      "issip" 失败窗口 [1,4]="issi",正解起点 4 就在窗口内(应 4 得
//      -1);补丁版 "aaab"/"aab" 仍 -1 应 1。窗口信息要么肯回头重拿
//      (朴素),要么记账(π 表),单指针归零两头都不占。m==1 无状态
//      可丢恒对,两个给定示例无重叠结构 → 70/86 巧合通过。结构上对的:
//      主循环骨架、return i-s 时机全对,只错失配一行。
//   2. 朴素解起点上界 start+m<=n(等价 start<=n-m):剩余不足 m 格免比;
//      m>n 时循环根本不进,自动 -1,不用额外守卫。
//   3. KMP 找到即 return i-m+1(=原解的 i-(m-1),同一时机):s 加到 m
//      后若不早退,下一轮 needle[s] 越界读。
//   4. 库函数彩蛋(未爆):haystack.find(needle) 找不到返回 string::npos
//      =SIZE_MAX,直接 return 靠 size_t→int 截断恰好补码成 -1(截断前
//      属实现定义,C++20 才定死补码),两负默契但属歪打正着,正路写
//      pos==string::npos?-1:pos。
//   5. (未爆,语言层)原解条件 s!=needle.size()-1 在 size_t 域算:m=0
//      时 size()-1 下溢成 SIZE_MAX,s!=SIZE_MAX 恒真,接着 needle[0]
//      越界 UB——本题 m>=1 挡住;带 size()-1 的条件先想空串。

// 解法一(朴素双循环,主解):外层枚举起点,内层逐位对齐,失配弃起点换下一个。
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        for (int start = 0; start + m <= n; start++)  // 起点界:剩余不足 m 格免比(见坑2)
        {
            int j = 0;
            while (j < m && haystack[start + j] == needle[j])
                j++;                                   // 逐位对齐,失配即弃此起点(肯回头重比)
            if (j == m)
                return start;                          // 整段全对,首达即最左
        }
        return -1;
    }
};

// 解法二(KMP 失配表):已配长 j 随身携带不归零,失配跳 pi[j-1] 续比当前
//   字符;造表=needle 对自己跑同一个循环。提交时改名 Solution:
class Solution2 {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        vector<int> pi(m, 0);                          // pi[j]:needle[0..j] 最长相等真前后缀
        int j = 0;
        for (int i = 1; i < m; i++)                    // 造表:自己找自己,形状同主匹配
        {
            while (j > 0 && needle[i] != needle[j])
                j = pi[j - 1];                         // 失配回跳:同一套动作在表内递归
            if (needle[i] == needle[j])
                j++;
            pi[i] = j;
        }
        j = 0;                                         // 已配长:原解的 s,失配不再归零(见坑1)
        for (int i = 0; i < n; i++)
        {
            while (j > 0 && haystack[i] != needle[j])
                j = pi[j - 1];                         // 救回窗口内最长相等前后缀,更早起点必败
            if (haystack[i] == needle[j])
                j++;
            if (j == m)
                return i - m + 1;                      // 找到即退,迟退下一轮越界(见坑3)
        }
        return -1;
    }
};
