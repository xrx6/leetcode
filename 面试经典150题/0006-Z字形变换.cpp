// 题号:0006  题名:Z字形变换  难度:中等
// 链接:https://leetcode.cn/problems/zigzag-conversion/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(方向开关+行桶/周期公式直写,Python 交叉验证 478 万穷举+2000 随机三方一致;待自己敲一遍 AC)
//
// 思路:
//   输出=按行拼接,列坐标在逐行读出时被整个丢弃 ⇒ 每个字符需要的全部
//   信息只有「行号」——暴力真建 r×c 网格再扫,养的「列」从未被用过。
//   降维三级:①网格→行桶,每行一个串,走到就追加;②行号→方向开关,
//   row 撞 0/r-1 两堵墙翻转方向,整张 Z 图压成「一个行号+一个方向比特」;
//   ③开关→周期公式,行号序列 0,1,…,r-1,r-2,…,1 循环,周期 T=2r-2
//   (一竖 r 格+一斜 r-2 格,端点只计一次)。公式版还能反过来按输出行
//   拉取:行 j 每周期收两格——下行位 base+j、上行位 base+T-j,端行两位
//   重合只收一格;写指针 k 直写输出串,输出即工作区(42/238 家族),
//   每源格恰属一行,写位互不冲突,k 恰走 n 步。模拟→公式是「证明了有
//   闭式就不逐格走」的又一次坍缩,与 DP→贪心的坍缩同族;与 14 逐列
//   比较互为镜像:那边概念网格按列读、这边按行读,网格都不落地。
// 复杂度:
//   两解时间皆 O(n)。解法一 O(n) 空间(桶内字符共 n 个,末了拼一趟);
//   解法二 O(1) 额外空间。暴力基线:真建 r×c 网格(c≈(r-1)·n/T,n/2
//   量级)填 O(n)、逐行扫 O(r·c)。
// 坑:
//   1. numRows==1:横着一排,Z 形状根本不存在。开关版:0 与 r-1 是同一
//      堵墙,首格翻转后 row+=1 出桶,rows[1] 越界 UB(n>=2 必爆,n==1
//      侥幸收工);公式版:T=2r-2=0,base+=T 原地踏步,k 先写穿 ret 再
//      死循环。同一输入两版死法不同,一行守卫 if(numRows==1) return s;
//      两版共用(连独立的暴力网格基线都走不下去:r=1 没有斜线可走)。
//   2. 端行的上行位与下行位重合(公式版):j=0 时 base+T-j=base+T=下一
//      周期的下行位;j=r-1 时 =base+r-1=本周期下行位自身。不跳过 ⇒ 端
//      行字符收两遍、k 超过 n 写穿 ret(r=3 "PAYPALISHIRING" 第 0 行
//      隔离看变 PAAHHNN,正确 PAHN)。上行位只在中间行 0<j<r-1 收。
//   3. 方向开关的初值与墙要配套(解法一):正解=两头都判(row==0||row
//      ==numRows-1)+ 初值 false——首格撞 0 墙自动翻成向下,起步自洽。
//      初值改 true(墙判全):第 2 格 row 即 -1;条件只判底墙:配初值
//      false 第 2 格 -1,配初值 true 撑到第一次回顶后 -1(轨迹实证
//      0,1,2,1,0,-1,…)。方向由撞墙决定,初值只负责「头一次撞墙
//      前别越界」,0 墙承担起步与回顶两职,漏判它必在顶上出界。
//   4. numRows>=n(未爆):只有一竖列,答案就是 s。两版免守卫天然正确
//      ——开关版走不到回头墙,公式版上行位全部越界、被逐周期检查跳过
//      (脚本穷举 r∈[n,n+3] 全对)。早退守卫可省,留着省一趟;解法一
//      桶数取 min(numRows,n),s 短时多出的空桶拼接也无害,纯省内存。

// 解法一(方向开关+行桶,主解):行号靠撞墙翻转,边走边入桶,末了按行拼接。
class Solution {
public:
    string convert(string s, int numRows) {
        int n=s.size();
        if(numRows==1)                              // Z 形状不存在,两版共用守卫(见坑1)
            return s;
        vector<string> rows(min(numRows,n));        // 桶数取 min:s 短时用不满(见坑4)
        int row=0;
        bool goingDown=false;                       // 初值"非向下":首格撞 0 墙翻成向下(见坑3)
        for(char c:s)
        {
            rows[row]+=c;
            if(row==0||row==numRows-1)              // 两堵墙:撞顶或撞底即掉头
                goingDown=!goingDown;
            row+=goingDown?1:-1;
        }
        string ret;
        for(string& r:rows)
            ret+=r;
        return ret;
    }
};

// 解法二(周期公式,直写输出):T=2r-2;按输出行拉取,中间行每周期收
//   两格、端行一格;写指针 k 单调前进,输出即工作区。提交时改名 Solution:
class Solution2 {
public:
    string convert(string s, int numRows) {
        int n=s.size();
        if(numRows==1)                              // T=0 死循环,门口守卫(见坑1)
            return s;
        int T=2*numRows-2;                          // 一竖 r 格+一斜 r-2 格,端点只计一次
        string ret(n,' ');
        int k=0;                                    // 写指针:每源格恰属一行,k 恰走 n 步
        for(int j=0;j<numRows;j++)                  // 输出行 j:收它落在各周期的成员
        {
            for(int base=0;base+j<n;base+=T)
            {
                ret[k++]=s[base+j];                 // 下行位:竖着下落经过 j
                if(j>0&&j<numRows-1&&base+T-j<n)
                    ret[k++]=s[base+T-j];           // 上行位:斜着回升经过 j,仅中间行(见坑2)
            }
        }
        return ret;
    }
};
