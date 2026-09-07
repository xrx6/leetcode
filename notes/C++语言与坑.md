# C++ 语言点与坑

> 刷题中实际踩过的坑与语言知识,按主题持续追加 · 始于 2026-09-07

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

## sort(`<algorithm>`,力扣免 include)

```cpp
sort(v.begin(), v.end());                    // 默认升序,区间 [begin, end) 左闭右开
sort(v.begin(), v.end(), greater<int>());    // 降序
sort(v.begin(), v.end(), [](int a, int b) {  // 自定义:返回 true 表示 a 排 b 前
    return abs(a) > abs(b);
});
// 普通数组:sort(arr, arr + n);
```

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

## 自测问题(不看上文试试)

1. `arr[2]` 和 `*(arr+2)` 什么关系?
2. `nums.data()` 和 `&nums` 是同一个地址吗?
3. `data()+size()` 指向哪?能解引用吗?
