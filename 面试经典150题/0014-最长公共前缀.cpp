// 题号:0014  题名:最长公共前缀  难度:简单
// 链接:https://leetcode.cn/problems/longest-common-prefix/?envType=study-plan-v2&envId=top-interview-150
// 状态:独立完成(自己先试,随机交叉验证全对;坑2 erase 二参钳制恰好无害,坑3 隐式退出,坑4 声明赋值两步)
//
// 思路:
//   公共性有一票否决:任何一条串在第 k 格不同,前缀就长不过 k ⇒ 候选只
//   会被后来的串截短,永远不会变长 ⇒ 拿 strs[0] 当候选,每来一条截到与
//   它一致(横向折叠:LCP(a,b,c)=LCP(LCP(a,b),c),结合律)。截短分两刀:
//   长度刀(候选不得长于当前串)+ 内容刀(第一个失配格起全不要)。纵向
//   视角等价:按列扫,列内全票相同才前进——同一件事,按串折还是按列走。
//   候选只缩不涨与 238 滚动乘、42 滚动 rightMax 同族:跑一趟,答案变量
//   单调收敛,无需回头。
// 复杂度:
//   时间 O(S)(S=全部字符数,每格至多被比一次;erase 单次 O(当前候选长),
//   每串至多两刀,总量仍 O(S));空间 O(L) 存候选副本(L≤200)。
//   对照:纵向逐列 O(S)/O(1)(以 strs[0] 为参照,不拷贝候选);
//   排序后只比字典序首尾两条 O(nL·log n)。
// 坑:
//   1. 两刀缺一不可:只有内容刀,s="flow"、t="fl" 时逐字全等、没有失配,
//      循环走完 s 仍是 "flow"(该截到 "fl");只有长度刀,首格失配也截
//      不掉。一个管 min 长度、一个管失配点,两刀先后互换则等价。
//   2. erase 二参钳制(未爆):s.erase(pos,s.size()) 第二参数远超剩余
//      长度,标准库自动钳成「从 pos 删到尾」,恰好无害——这层宽容是
//      钳制给的,不是「删 len 个」的语义;一参数版 s.erase(pos) 语义
//      就是删到尾,更短更准(语言参考·string 删段)。
//   3. 隐式退出(未爆):内容刀下去后没有 break,靠循环条件 j<s.size()
//      每轮重求值自己变假退出——s 长度恰变成 j,j++ 后必假,语义必然
//      成立,但要求读者知道「条件每轮重算、size() 是实时值」;显式
//      break 一眼明白。与 58 悬空 c++ 同为「不写断句、靠别处兜底」:
//      那边是控制流巧合,这边是数学必然。
//   4. 小冗余:原提交 string s; 先默认构造空串、再 s=strs[0]; 赋值,
//      string s=strs[0]; 一步拷贝构造到位;本入库版已合并。

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string s=strs[0];                          // 候选:从第一条出发,之后只被截短
        int n=strs.size();

        for(int i=1;i<n;i++)
        {
            if(s.size()>strs[i].size())            // 长度刀:候选不长于当前串(见坑1)
                s.erase(strs[i].size(),s.size());
            for(int j=0;j<strs[i].size()&&j<s.size();j++)
            {
                if(s[j]==strs[i][j])
                    continue;
                else
                {
                    s.erase(j,s.size());           // 内容刀:失配格起全不要(见坑3)
                }
            }
        }
        return s;
    }
};
