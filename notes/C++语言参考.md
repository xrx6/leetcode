# C++ 语言参考

> 横向的语言知识库:不管做哪道题都用得上的 C++ 写法与坑。按主题持续追加 · 始于 2026-09-07

## 坑总集(复习价值最高 ⭐)

| 坑 | 症状 | 正确写法 |
|---|---|---|
| `==` 当 `=` 用 | 编译报错/静默无效 | `nums1[i] = nums2[i-m]` |
| `cout << nums1` | vector 没有 << 重载,编译不过 | 删掉,力扣不需要输出 |
| for 循环用逗号 | `for(int i=m, i<n, i++)` 编译不过 | `for(int i=m; i<n; i++)` 三个分号隔开 |
| 循环条件 `\|\|` | 其中一边到 -1 还在被访问 → 越界 | 双指针「比较型」循环用 `&&`(比较需要两边都有效) |
| `*(nums)` | vector 是对象不是指针,不能解引用 | 读元素用 `*j`(指针)或 `nums[j]`(下标) |
| 哨兵用 `<=` | 多走一步,访问 end 位置越界 | `j < end`,哨兵是排他的 |
| 判断方向反了 | 写成 `if(== val) 做点什么` | 应为 `if(!= val) 抄写`,等于 val 的动作是「跳过」 |
| 非 void 函数不写 return | 编译不过(26/80 踩过) | 签名承诺了返回类型就要兑现:26 `return slow+1`,80 `return slow` |
| 空 vector 当计数器 `count[x]++` | 越界写,首用例即崩(169) | 值域小且非负才能数组计数;任意值(±10⁹/负数)用 `unordered_map` |
| map 按下标扫 `j<m.size()` | `m[j]` 访问即插入,size 与 j 同涨 → 死循环 MLE(169) | 遍历用 `for (auto& [k,v] : m)`;只判存在用 `m.count(x)` |
| `int dp[n][2]` 当二维数组 | VLA 是 C99 特性非标准 C++:MSVC 编译错;大 n 爆栈且无法 catch(122) | `vector<vector<int>> dp(n, vector<int>(2))` 或滚动变量;大数组放堆 |
| `vector<int> dp; dp[0]=0` | 空 vector 下标写 = 解引用空指针,UBSan: reference binding to null pointer(45/238 两次;42 同型潜伏) | 构造时定长 `vector<int> dp(n, val)`;`operator[]` 不检查不扩容,大小只走构造/resize/push_back |
| `INT_MAX` 当无穷大 | `dp[j]+1` 溢出成负数,min 结果悄悄错(45) | 「够大的具体数」:答案上界+1(如 10001)或习惯值 0x3f3f3f3f |
| 成员写进构造函数 | `RandomizedSet(){ vector<int> nums; }` 是局部变量,函数结束即销毁,成员函数看不见(380) | 声明放类体 `private:` 下;vector/map 默认构造自动为空,构造函数留空即可 |
| 成员函数漏括号 | `nums.size-1` 把「函数本身」当值用,编译不过(380) | `nums.size()`;C++ 取东西的动作普遍带 `()` |
| 边遍历边 `erase(it)` 后 `++it` | 迭代器失效,跳元素/崩溃 | `it = v.erase(it)` 接管「下一个有效位置」,删除分支不再 `++` |
| `back()`/`end()` 混用 | `*end()` 解引用哨兵 = UB | 要**元素**用 `front()/back()`,要**位置**用 `begin()/end()`;末元素 = `*(end()-1)` |

## sort(`<algorithm>`,力扣免 include)

```cpp
sort(v.begin(), v.end());                    // 默认升序,区间 [begin, end) 左闭右开
sort(v.begin(), v.end(), greater<int>());    // 降序
sort(v.begin(), v.end(), [](int a, int b) {  // 自定义:返回 true 表示 a 排 b 前
    return abs(a) > abs(b);
});
// 普通数组:sort(arr, arr + n);
```

## reverse(`<algorithm>`,力扣免 include)

```cpp
reverse(v.begin(), v.end());           // 整段翻转
reverse(v.begin(), v.begin() + k);     // 只翻前 k 个:区间同样是 [first, last) 左闭右开
```

用法见 `翻转/三次翻转.md`(189)。

## 长度

- vector:`v.size()`(返回无符号 size_t;`v.size()-1` 在空 vector 时下溢成大数,循环会飞)
- string:`length()`(和 size() 等价);vector 没有 length(),别记混
- 普通数组:`sizeof(arr)/sizeof(arr[0])`;C++17 可用 `std::size(arr)`
  - **大坑**:数组传参后退化为指针,sizeof 变成指针大小(8 字节),此法失效 → 长度要当参数传

## 数组名 vs vector 名(最重要的概念区分)

```cpp
int arr[3] = {10,20,30};
arr        // 数组名,表达式中退化成首元素地址
arr[0]     // 值:10
&arr[0]    // 首元素地址,和退化的 arr 一回事
arr[i] ≡ *(arr + i)     // 下标 = 指针算术 + 解引用

vector<int> nums = {10,20,30};
nums       // 对象!不是地址,不会退化
nums[0]    // 值:10
nums.data()     // 底层元素首地址(int*)
&nums[0]        // 同 data()
&nums           // vector 对象自己的地址(另一回事)
// nums + 1 编译不过;cout << nums 编译不过(无 << 重载)
```

内存布局:vector 是两层的——对象本体(约 24 字节,栈上)里存着「数据在哪/多长/容量」,元素本体在堆上。

一句话:**数组名「就是」一段内存的地址(用时退化);vector 名「是」一个管理着另一段堆内存的对象(要地址主动 `.data()`)**。

## 指针算术(单位是「元素」,不是字节)

```cpp
int* end = nums.data() + nums.size();  // 指针 + 整数:前进 n 个元素
                                       // 真实地址 + n × sizeof(int)
return i - nums.data();                // 指针 − 指针:隔着几个元素
                                       // 字节差 ÷ sizeof(int)
// 对称性:(p + n) - p == n
// 哨兵 end:可以算、可以比(j < end),不能解引用(*end 越界)
// 指针 + 指针:非法,编译不过
// 指针相减只在同一数组内才有意义
```

示例(int 4 字节,data()=0x1000,4 个元素):

```
0x1000 [10] ← data()
0x1004 [20]
0x1008 [30]
0x100C [40]
0x1010 (无) ← data()+4 = end
```

## 迭代器:三招合同,指针就是数组的迭代器

```cpp
int arr[5] = {3,0,6,1,5};
int* p = arr;           // 数组名退化成首地址——这就是数组的 begin
int* e = arr + 5;       // 尾后哨兵——这就是数组的 end
for (; p != e; ++p) *p; // 三招:!=(比)、++(走)、*(读)

vector<int> v = {3,0,6,1,5};
auto it = v.begin();    // vector 迭代器 ≈ data() 指针穿马甲
for (; it != v.end(); ++it) *it;
```

- **为什么发明它**:sort/find 想通吃所有容器,只定最小合同「能指着取(`*it`)、能前进(`++`)、能比到头没有(`!=`)」,谁满足就伺候谁。C 的指针天生会三招 → **指针就是数组的迭代器**,`sort(arr, arr+n)` 因此成立,同一份 sort 也吃 vector(`sort(v.begin(), v.end())`)。
- **begin()/end() 是「取起点/哨兵的函数」**:`v.begin()` 指首元素,`v.end()` 指最后一格的**再下一格**(哨兵:可以算、可以比,**不可解引用**)。左闭右开让「空区间」免费表示(begin==end)。
- **范围 for 是迭代器的皮肤**:`for (int x : v)` 展开成 begin/end + 三招循环,裸数组同样适用(`for (int x : arr)` 合法)。
- **能力分级**:vector/数组 = 指针级全能(`it+3`、`it1-it2`、`<`);map/list = 只能 `++/--` 一步步走。所以 find 谁都能伺候,`it - v.begin()`(迭代器换下标)只有指针级的能玩。
- **auto 顺理成章**:迭代器真实类型如 `unordered_map<int,int>::iterator`,没人手写——`auto it = m.find(k);` 让编译器按右边填类型,编译期钉死,不是「万能变量」。三大高频岗位:`auto it = m.find(...)`、`auto it = v.begin()`、`for (auto& [k,v] : m)`。

## class 与 return(力扣提交格式的原理)

- `class Solution` 只是**外壳**:LeetCode 规定的提交打包格式,判题机按 `Solution s; int k = s.removeDuplicates(nums);` 调用。类本身不干活,干活的是里面的**成员函数**。
- 函数签名 `int f(...)` 开头的 int 是对编译器的承诺:非 void 函数**必须 return**,不写编译不过——和写在不在 class 里无关,任何函数都一样。
- 原地修改的题为什么还要返回值:改完的数组尾点是废数据,**有效长度只有写代码的人知道**,判题机自己看不出来。返回值 = 汇报「前 k 个是答案」,判题机只检查前 k 位。
- 对照实例:27 `return i - nums.data()`,26 `return slow + 1`,80 `return slow`——全是这句汇报。

## unordered_map(哈希表)

```cpp
unordered_map<int,int> m;       // 声明:键类型, 值类型
m[x]++;                         // 取值/累加;键不存在自动插入且值初始化为 0
if (m.count(x)) {...}           // 只判存在用 count,别用 m[x](会顺手插入!)
for (auto& [k,v] : m) {...}     // 遍历键值对,k 键 v 值(C++17)
```

- **心智模型**:vector 是一排连续格子,按「位置」取——下标必须小、非负、且开过那么长;unordered_map 是一堆带标签的抽屉,按「值」取——标签可以是任意 int(甚至 string),首次访问自动开抽屉。
- `operator[]` **访问即插入**:169 的 MLE 标本——第二趟 `for(j=0;j<m.size();j++) if(m[j]>...)`,每圈插入键 j,size 与 j 同步涨,循环条件永真,插到内存爆。多数元素恰为负数时才暴露;小非负值的用例全靠侥幸通过。
- `m.size()` 是「**不同键的个数**」,不是值域宽度;map 的键是散的,不能按下标扫,只能遍历键值对。
- `map` vs `unordered_map`:前者红黑树,按键有序,O(log n);后者哈希,无序,平均 O(1)。刷题默认 unordered,只有要按键的顺序遍历时才用 map。
- 数组计数 `vector<int> count(N,0)` 只在「值域小且非负」时可用(构造时定长清零,不是 push_back——push_back 只在末尾追加,不改变「值→格子」的映射)。
- **哈希原理三句话**(380 深入):①哈希函数把键搅成几乎随机的巨大整数(int 基本是自己变身,string 逐字符搅动),同键必同值(确定性),越散越好(均匀);②对桶数取余落桶(和 rand()%n 同一招);③同桶冲突用链挂起。查找 = 算一次哈希 + 走几步短链,与 n 无关 → **平均 O(1)**;最坏全挤一桶退化 O(n),故只承诺「平均」。装填因子 = size/桶数,超 1.0 触发 **rehash**(桶翻倍、全部重挂、迭代器全作废)——所以遍历时别插入。
- **分工三件套**:只判存在 → `count(k)`(纯读探针,0/1,零副作用);判存在+接着读改 → `find(k)` 返回迭代器,`it != m.end()` 为找到,`it->first` 键 `it->second` 值(迭代器像指针,取成员用 `->`);已定位就改**省一次查找**且无插入风险。`m[k]` 仅在「确定存在或本来就想插入」时用。
- **元素住哪桶由哈希值定**,与插入序无关,跨编译器还可能不同——依赖遍历顺序的代码都是错的;要按键有序遍历用 `map`。

## 范围 for(C++11)

```cpp
for (int price : prices) {...}       // price 是每个元素的「拷贝」:只读小对象
for (int& x : v) {...}               // 引用:循环体里改 x 会写回数组
for (const auto& x : v) {...}       // 只读引用:大对象(string/vector)省拷贝
```

- **心智模型**:把容器从头到尾依次取出,每次把元素放进左边的变量——等价于 `for(i...) { int price = v[i]; ... }`。
- **什么时候用**:循环体只要元素的值、不需要下标 i——没有 `i<n` 就没有越界机会(121 只读价格,正合适)。
- **什么时候不能用**:需要下标(和 `i-1` 比较、写另一个数组的位置,如 189 翻转区间)→ 传统下标循环。
- 169 修 MLE 的 `for (auto& [k,v] : m)` 就是它 + **结构化绑定**(pair/键值对一次拆两个变量)。
- 遍历二维:行必须 `auto& row`,否则每行整行拷贝;行内元素 `int x` 拷贝没成本:

```cpp
for (auto& row : dp)
    for (int x : row) {...}
```

## 动态大小的数组:VLA 坑与二维 vector 正解

```cpp
int dp[n][2];                                // VLA:n 运行期才知道 → 栈上动态分
vector<vector<int>> dp(n, vector<int>(2));   // 标准正解:n 行 × 每行 2 个 0
```

- **VLA 是 C99 的东西,不是标准 C++**:GCC 当扩展放行(`-Wpedantic` 能看到警告),MSVC 直接编译错误——力扣能过 ≠ 标准 C++。真正的雷是**栈**:默认 1~8MB,`n×2×4≈240KB`(122)尚安全,10⁵ 级再多一维状态就直奔兆级;爆栈无法 catch,而 vector 分配失败抛 bad_alloc 可救。经验法则:**大数组放堆**,比纠结 VLA 与否更本质(编译期常量的大数组 `int dp[100000][2]` 放栈上同样有爆栈风险,合法≠安全)。
- **构造函数读法**:`vector<T> v(n, val)` = n 份 val 的拷贝;把 T 和 val 都换成 `vector<int>(...)` 就是二维——`vector<int>(2)` 先造临时行 `{0,0}`,外层拷 n 行。
- 访问 `dp[i][j]` 与 C 二维数组**完全同形**(122 里替换声明一行,其余零改动);`dp.size()` 是行数,`dp[0].size()` 是列数。
- 常用四形态:

```cpp
vector<vector<int>> dp(n, vector<int>(m));        // 定长矩阵全 0(DP)
vector<vector<int>> memo(n, vector<int>(m, -1));  // 备忘录全 -1
vector<vector<int>> g(n);                          // n 个空行,行内 push_back(邻接表)
vector<vector<int>> mat = {{1,2,3},{4,5,6}};      // 列表初始化,直接给值
```

- 与 C 二维数组的本质区别:**每行是独立的堆对象,长度可以不一样**(锯齿数组);`g[i].resize(m)` 只改第 i 行。
- 性能一句:`vector<vector<int>>` 是两级跳(dp→行→元素)、n+1 次堆分配;卡常极端场合才用一维 `dp[i*m+j]` 下标换算,刷题与面试不用管。

## 空 vector 下标访问:operator[] 不检查、不扩容(45/238 标本,42 同型潜伏)

```cpp
vector<int> dp;   // 默认构造:size = 0,内部数据指针是 nullptr
dp[0] = 0;        // ← 炸:UBSan 报 reference binding to null pointer
```

- **心智模型**:vector 的大小不是「用着用着就有」,只在**构造 / resize / push_back / insert / clear** 时改变。`operator[]` 只干「往第 i 格读写」一件事——既**不做边界检查**,也**不会扩容**。空 vector 连一格都没有,`dp[0]` 等价于对 nullptr 解引用,未定义行为。
- **为什么平时察觉不到**:`operator[]` 越界不抛异常不报错(会抛 `out_of_range` 的是 `at()`),本地小数据可能「恰好没炸」;力扣开着 UBSan(UndefinedBehaviorSanitizer)才现形。报错行号落在 `stl_vector.h` 是炸在标准库 operator[] 的内部实现处,不是自己代码的行号。
- **正解**:构造时就把格子给足——`vector<int> dp(n, 10001); dp[0] = 0;`(构造函数读法见上一节)。
- **`reserve(n)` 也救不了**:它只加**容量**(提前预留内存),**大小**仍是 0,照样越界。容量 = 预留的空位数,大小 = 实际元素数,只有大小范围内才可下标。
- **对照原生数组**:`int dp[100]; dp[0]=0;` 合法——声明即有 100 格;vector 名字像数组,但格子数必须显式给,这是从数组思维迁到 vector 最容易想当然的一处。
- **238 二次复发(距 45 约一个月)**:`vector<int> answer; answer[0]=1;` 同款炸法——「看过笔记」不等于「长在手上」。下标写 vector 前先问一句「格子谁给的」:要么构造时定长,要么 push_back。
- **42 同型潜伏(2026-09-22,未爆)**:自敲 42 时删了 n==0 判空,`rightmax[n-1]=height[n-1]` 起步——n==0 就是同款越界,靠题面 1≤n 的约束护栏拦住没炸。凡 `v[n-1]` / `v[0]` 起手,先确认 n≥1 有保障;约束拦得住运行时错误,拦不住代码本身长成 UB 形状。
- **同题第二个雷:DP「无穷大」初值别用 INT_MAX**——`dp[j]+1` 在 dp[j] 尚未更新时 = 2147483647+1,溢出成负数,min 全乱,且是**静默**的溢出 UB。用「够大的具体数」:答案上界+1(45 的答案 ≤ n-1 ≤ 9999,取 10001),或最短路类题的习惯值 `0x3f3f3f3f`(≈1.06e9,自身够大、两个相加也不溢出)。

## size 与 capacity:扩容、均摊 O(1)、迭代器失效(380 语言点)

```
栈上(vector 对象,~24字节)        堆上(元素)
┌──────────────┐
│ data ────────┼──→ [10][20][30][?]
│ size = 3     │      ↑合法下标 0..2    ↑第4格:capacity 有、size 无,不可访问
│ capacity = 4 │
└──────────────┘
```

- **跟着 push_back 走(GCC 翻倍策略)**:size 0→1→2→3→4→5,capacity 0→1→2→4→4→8。搬迁只发生在跨 2 的幂时,旧内存随之释放——**此刻之前拿到的迭代器/指针/引用全部作废**(380 里没存过迭代器所以无恙)。搬迁总量 1+2+4+…+n/2 ≈ n,摊到 n 次 push_back 每次 O(1),这就是「均摊」。
- **失效规则表**:

| 动作 | 什么作废 |
|---|---|
| push_back 触发扩容 | 全部作废(旧内存已释放) |
| insert / erase 中间位置 | 被动位置之后的作废 |
| unordered_map 插入触发 rehash | 全部迭代器作废 |

- **边遍历边删的正解**:`it = v.erase(it)`(erase 返回「被删位置现在的新元素」),删除分支不再 `++it`;`erase(it)` 后直接 `++` 是经典崩溃。
- **改大小三函数**:resize(n) 改 size(变大补 0/指定值,变小截断);reserve(n) 只保 capacity ≥ n、size 不变(**救不了下标越界**);clear() size 归 0、capacity 保留。pop_back 只 size−1 不缩内存。
- **at() 调试法**:怀疑越界时把 `v[i]` 临时改 `v.at(i)`,越界当场抛 out_of_range,把静默 UB 变显性报错。
- **两家族**:front()/back() 返回**元素**(int);begin()/end() 返回**位置**(迭代器,end() 是哨兵非元素)。`back()` 对空容器同样是 UB。取末元素三种等价:`v.back()`、`*(v.end()-1)`、`v[v.size()-1]`(最后这种空容器时 size()-1 无符号下溢,坑)。

## auto 与 rand()%n(380 语言点)

- **auto = 类型让编译器按右边表达式填**,编译期钉死,不是「万能变量」(之后装别的类型照样编译错)。零运行开销,纯粹省打字——尤其迭代器那种长类型。
- **rand()%n = 取余造随机下标**:rand() 吐 [0,RAND_MAX] 的伪随机整数,%n 的余数必落 0..n-1(时钟:%12 必落 0..11),恰好是合法下标全体 → 等概率命中每个元素。严格讲 RAND_MAX+1 不被 n 整除时有极微偏差,刷题忽略;工程用 mt19937 + uniform_int_distribution(无偏且可复现)。

## 自测问题(不看上文试试)

1. `arr[2]` 和 `*(arr+2)` 什么关系?
2. `nums.data()` 和 `&nums` 是同一个地址吗?
3. `data()+size()` 指向哪?能解引用吗?
4. 力扣为什么要你把代码包进 `class Solution`?原地修改了数组,为什么还必须 return?
5. `m[x]` 访问一个不存在的键会发生什么?只判断键在不在,该用哪个写法?
6. 为什么 `for(j=0;j<m.size();j++) m[j]` 是死循环?`m.size()` 到底数的是什么?
7. `for (int x : v)` 里改 x,数组会变吗?想写回元素该用哪种声明?什么场景只能退回下标循环?
8. `int dp[n][2]` 在标准 C++ 里合法吗?力扣为什么能过?它真正的风险是什么?大数组的经验法则?
9. `vector<vector<int>> dp(n, vector<int>(2))` 这行怎么读出来?`dp.size()` 和 `dp[0].size()` 各是多少?二维遍历为什么行要用 `auto&`?
10. `vector<int> dp; dp[0] = 0;` 会发生什么?力扣报错里的 stl_vector.h 行号是什么?`reserve(n)` 能救吗,为什么?
11. DP 初始「无穷大」为什么不能写 INT_MAX?两个可用的替代写法是什么?
12. `sort(arr, arr+n)` 的两个参数各是什么?为什么裸数组不用成员函数也能配合 sort/find?
13. vector 和 unordered_map 的迭代器各指向什么?「一个元素」分别是什么?谁能 `it+n`、谁只能 `++`?
14. push_back 触发扩容后,之前保存的迭代器/引用还能用吗?为什么?边遍历边删的正解写法?
15. 只判键存在、判存在+接着改值、确定键存在,分别该用 count/find/operator[] 里的哪个?
16. resize、reserve、clear 分别改的是 size 还是 capacity?reserve 能让下标访问合法吗?
17. nums.back() 和 nums.end() 差在哪?用迭代器写出「最后一个元素」。
18. rand()%n 为什么恰好落在合法下标范围?它轻微不均匀的根源是什么?
