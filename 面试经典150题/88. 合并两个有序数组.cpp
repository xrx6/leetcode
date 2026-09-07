
// 链接:https://leetcode.cn/problems/merge-sorted-array/?envType=study-plan-v2&envId=top-interview-150
// 状态:看过提示 
// 思路:从数组最后开始比较，两谁大就把谁放到nums1末尾
#include<algorithm>
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i=m-1,j=n-1,k=m+n-1;
        while(j>=0&&i>=0)
        {
            if(nums1[i]>nums2[j])
            {
                nums1[k--]=nums1[i--];
            }
            else
            {
                nums1[k--]=nums2[j--];

            }
        }
        while(j>=0)
        {
            nums1[k--]=nums2[j--];
        }

    }
};