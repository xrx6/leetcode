// 题号:0068  题名:文本左右对齐  难度:困难
// 链接:https://leetcode.cn/problems/text-justification/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己先试:两阶段骨架对,实现层 9 病灶,最小反例单词输入 ["a"]
//       返回空——两个循环都不跑,连 UB 都不经过;词数≥2 触发行封板即踩空 vector
//       下标写 UB,Python 转写响亮 IndexError 复现;标准解+保留原结构的修错版双版
//       入库,Python 属性交叉验证 3 示例+边界+随机 20000 例全对;待自己敲一遍 AC)
//
// 思路:
//   纯模拟,两步走:
//   ① 贪心定行:从 words[i] 起收词,len 记「单词总长+词间最小空格」,只要
//      len+1+words[j]<=maxWidth 就再收一个——「尽可能多放」就是最小空格下不超宽。
//   ② 按行型拼格,只有三种行:
//      最后一行 / 单词成行(0 个间隙)→ 左对齐:词间恰 1 格,行尾补满;
//      普通行 → 两端对齐:总空格=maxWidth-单词总长,base=总/间隙数,extra=总%间隙数,
//      前 extra 个间隙各多 1 格——「左侧多于右侧」就是余数从最左的间隙发。
//   修错版(解二)保留原代码骨架:阶段1 分组存 neww/count,阶段2「轮流往词尾贴空格」
//   ——按间隙从左到右一轮一格地派,整轮派完剩下的自然落在左边间隙,等价于
//   base+extra 公式;行成后顺序拼接词(词尾已带格)。
// 复杂度:
//   时间 O(总字符):每词进出分组各一遍,每行拼接 O(maxWidth),行数 O(词数)。
//   空间 O(答案字符数) 不计返回值则 O(行数);修错版额外 O(行数) 存 count。
// 坑:
//   1. maxWidth 是题目给的「行宽」参数,首句 maxWidth=0 把它毁成「最长单词长度」,
//      后面全对也全错。题面已保证 words[i].length<=maxWidth,行宽根本不用算——
//      这句连同求最大值的循环整个删掉。
//   2. neww 是空 vector,neww[j]=xxx 是 UB:operator[] 不扩容,写的是不存在的
//      元素(对比:27/80 快慢指针是「已开好空间往下标里写」,这边空间根本没开)。
//      空 vector 进元素只有 push_back。
//   3. 贪心判据必须算空格:行内已有 cnt 个词时再进一词,最少还要 cnt 个空格
//      (cnt+1 词 cnt 间隙),cur+cnt+新词长<=maxWidth 才收;只加单词长度必超装。
//   4. 丢词三处,三处独立:分组循环 i<words.size()-1 漏末词(两处 -1 同病);
//      while(start<i) 结束时 start 已==i,再来一句 start++ 把 words[i] 跳过,
//      这个词哪行都不在(回退判据:出新行时 start 应恰好==i,交由 x=words[i] 接管);
//      循环结束后最后一组从未收进 neww——分组必须有「循环外收尾 flush」。
//   5. 行长要按 count[j] 把该行 start..start+count[j]-1 的词全求和;原版用
//      neww[j].size(),而 neww[j] 被反复覆盖赋值后只存「该行最后一个词」,
//      拿一个词的长度当整行单词总长,空格数全错。
//   6. 轮流贴空格三错:while(c>=0) 多跑一轮多贴一格(应 c>0,off-by-one);
//      回绕条件 i==start+count[j] 把行内最后一个词也纳入轮流,行尾被贴上尾随
//      空格,行长必超 maxWidth——间隙只有 count-1 个,只应给前 count-1 个词的
//      尾部派格;修好后单词成行(0 间隙)必须显式走左对齐分支,轮流法对 0 间隙
//      无从分配(原版 count==1 靠「回绕到自身」碰巧得到右侧填满,纯运气)。
//      另:这招会永久改写输入 words,本题不查输入但别养成习惯。
//   7. while(count[j]--) 拼接循环把 start 精确推进 count 个,循环外再 start++
//      就错位跳词,下一行起点从此全歪——「循环已维护的游标,循环外别再动手」。
//   8. 分支该按「最后一行/单词行 vs 普通行」分,不按 neww[j].size()<maxWidth 分:
//      恰好等于 maxWidth 的行会被整个跳过;行长是结果不是行型。
//   9. 坏味道:行号 j 本来连续,unordered_map<int,int> count 换 vector<int> 即可;
//      数据结构语义先定死——neww[j] 到底存「词」还是存「整行」,原版阶段1当它
//      存词、阶段2又当它存行用,两头都错。

// 解法一(标准解:贪心定行+按行型拼格):
class Solution {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> ans;
        int n = words.size();
        for (int i = 0; i < n; ) {
            // ① 贪心定行:words[i..j-1],len=单词总长+词间最小空格(每词后先垫 1 格)
            int j = i + 1, len = words[i].size();
            while (j < n && len + 1 + words[j].size() <= maxWidth)
                len += 1 + words[j++].size();

            int wc = j - i;                        // 本行词数
            bool lastLine = (j == n);
            string line;

            if (lastLine || wc == 1) {
                // ② 左对齐:最后一行&单词成行共用——词间恰 1 格,行尾补满
                for (int k = i; k < j; k++)
                    line += words[k] + (k == j - 1 ? "" : " ");
                line.append(maxWidth - line.size(), ' ');
            } else {
                // ③ 两端对齐:len 含 wc-1 个最小格,先扣掉得单词总长
                int totalSpaces = maxWidth - (len - (wc - 1));
                int gaps = wc - 1;
                int base = totalSpaces / gaps, extra = totalSpaces % gaps;
                for (int k = i; k < j; k++) {
                    line += words[k];
                    if (k < j - 1)                                  // 最后一个词后面不贴
                        line.append(base + ((k - i) < extra), ' '); // 前 extra 个间隙各多 1 格
                }
            }
            ans.push_back(line);
            i = j;
        }
        return ans;
    }
};

// 解法二(修错版:保留首敲版「阶段1分组+阶段2轮流贴空格」的骨架,坑1-8 全修;
//   提交时改名 Solution):
class Solution2 {
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        // ===== 阶段1:贪心分组(修坑1:不动 maxWidth;坑3:判据算空格;坑4:遍历到 n+收尾) =====
        vector<string> neww;
        vector<int> count;                            // 坑9:行号连续,vector 就够
        int cur = 0, cnt = 0;                         // cur=当前行单词字符和(不含空格),cnt=词数
        for (int i = 0; i < (int)words.size(); i++) {
            if (cnt == 0 || cur + cnt + (int)words[i].size() <= maxWidth) {  // 已有 cnt 词,再进一词最少还要 cnt 格
                cur += words[i].size(); cnt++;
            } else {
                neww.push_back("");                   // 坑2:空 vector 只能 push_back
                count.push_back(cnt);
                cur = words[i].size(); cnt = 1;       // words[i] 自己开新行(坑4:不再 start++ 丢它)
            }
        }
        neww.push_back("");                           // 坑4:循环外把最后一组收进行
        count.push_back(cnt);

        // ===== 阶段2:轮流贴空格+拼接(修坑5-8) =====
        int start = 0;
        for (int j = 0; j < (int)neww.size(); j++) {
            int len = 0;                              // 坑5:行长=本行全部单词求和
            for (int k = start; k < start + count[j]; k++) len += words[k].size();
            int c = maxWidth - len;                   // 本行要贴的空格总数
            int gaps = count[j] - 1;                  // 坑6:间隙数=词数-1
            bool lastLine = (j == (int)neww.size() - 1);

            if (!lastLine && gaps > 0) {              // 普通行才轮流贴
                int i = start;
                while (c > 0) {                       // 坑6:c>0,不是 c>=0
                    words[i] += ' ';
                    c--; i++;
                    if (i == start + gaps) i = start; // 坑6:回绕不含行末词
                }
            }

            neww[j] = "";
            if (lastLine || gaps == 0) {              // 坑6/8:末行&单词行→左对齐
                for (int k = 0; k < count[j]; k++) {
                    if (k > 0) neww[j] += ' ';
                    neww[j] += words[start++];
                }
                neww[j].append(maxWidth - neww[j].size(), ' ');
            } else {
                for (int k = 0; k < count[j]; k++)    // 词尾已带格,顺序拼接即成
                    neww[j] += words[start++];
                // 坑7:这里不再多 start++
            }
        }
        return neww;
    }
};
