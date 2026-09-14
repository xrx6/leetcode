// 题号:0380  题名:O(1)时间插入、删除和获取随机元素  难度:中等
// 链接:https://leetcode.cn/problems/insert-delete-getrandom-o1/?envType=study-plan-v2&envId=top-interview-150
// 状态:看题解后理解(自己敲一遍,修「成员写进构造函数/unodered 拼写/size 漏括号」三处后 AC)
//
// 思路:
//   单结构试错:vector 尾插 O(1)、随机下标 O(1),按值删 O(n);unordered_set
//   增删 O(1) 但无下标,getRandom 做不了等概率。短板互补 → 组合:vector nums
//   = 仓库(出「按位置」能力,支撑随机取);unordered_map pos(值→下标)= 登记
//   簿(出「按值」能力,支撑 O(1) 定位)。剩硬骨头:删中间格要 O(n) 挪 →
//   换尾删除:尾元素填进空位 + pop_back,两个 O(1)。顺序可牺牲——题目要的
//   是集合,元素住几号格没人关心(设计题「没说的」一栏里的自由)。
//   两结构语言上无关,靠不变量绑定:「pos 每个键都在 nums 里,且 pos[k] 就是
//   k 的真实下标」——每个操作动仓库必记账;类(私有成员+成员函数)是纪律守护者。
// 复杂度:
//   三操作平均 O(1)(哈希平均、push_back 均摊)  空间 O(n)
// 坑:
//   1. 成员变量写在构造函数里 = 局部变量,函数结束即销毁,成员函数看不见;
//      声明放类体 private: 下。vector/map 默认构造自动为空,构造函数留空即可。
//   2. pos[last]=idx 必须在 pos.erase(val) 之前:删的是尾元素本身时
//      last==val,先 erase 再赋值会把刚注销的键又插回来。
//   3. 忘记更新搬入者(尾元素)的登记 → 不变量破裂,后续删错格/越界,延迟爆
//      炸极隐蔽。
//   4. unordered_set 单挑做不了 getRandom(无下标);rand()%size() 用取余把
//      随机大数折进合法下标 0..n-1。
//   5. 编译器一次只报最前面的错(拼写错遮住了后面两个),修一个再看下一个。
class RandomizedSet {
private:
    vector<int> nums;
    unordered_map<int,int> pos;
public:
    RandomizedSet() {}

    bool insert(int val) {
        if (pos.count(val)) return false;
        nums.push_back(val);
        pos[val] = nums.size() - 1;
        return true;
    }

    bool remove(int val) {
        if (!pos.count(val)) return false;
        int last = nums.back();
        int idx = pos[val];
        nums[idx] = last;
        pos[last] = idx;
        nums.pop_back();
        pos.erase(val);
        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};
