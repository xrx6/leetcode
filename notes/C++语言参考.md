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
