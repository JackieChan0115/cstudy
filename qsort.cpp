/**
 * 快速排序的非递归实现方式
 * 递归的快速排序很好写，那么非递归就有些难度了
 * 在实际生产环境中，能不用递归就不要用递归
 *
 * 本题源自于leetcode中的一道简单题目，628,三个数的最大乘积，链接如下
 * https://leetcode-cn.com/problems/maximum-product-of-three-numbers/submissions/
 *
 * Author: JackieChan
 * 2020.11.26
 */ 
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class Solution {
public:
	// 求三个数组最大乘积的入口函数
    int maximumProduct(vector<int>& nums) {
		// 非递归快速排序
        qsort(nums);
        int n = nums.size();
		// 三个数最大乘积，要么是前两个数和最后一个数的乘积
        int value1 = nums[0] * nums[1] * nums[n-1];
		// 要么是最后三个数的乘积
        int value2 = nums[n-1] * nums[n-2] * nums[n-3];

        return max(value1,value2);
    }

    // 快速排序的非递归方法
    void qsort(vector<int>& nums){
        //需要记录每个待排区间
        int n = nums.size();
        stack<int> st; // 使用栈来保存每个待排区间
		st.push(0); // 首先放入最大的整个区间
		st.push(n-1);
		int mid = 0;
        while(!st.empty()){
            int right = st.top(); //注意， 栈顶是区间右边界
            st.pop();
            int left = st.top();
            st.pop();
            mid = sortRange(nums, left, right);
			// mid是每次快速排序定好位置的那个中间值，用它来划分左右区间
            if(mid > left+1){
                st.push(left);
                st.push(mid-1);
            }
            if(mid < right-1){
                st.push(mid+1);
                st.push(right);
            }
        }

    }
	
    // 数组， 左边端点，右边端点， 都被包含
	// 返回值：每次快速排序中pivot放置的位置的坐标
    int sortRange(vector<int>& nums, int left, int right){
        int pivot = findPivot(nums, left, right); // 寻找一个pivot值
        int l = left, r = right;
        //比pivot大的在右侧，比pivot大的在左侧
        while(left < right){
            while(left < right && nums[right] >= pivot) right -= 1;
            nums[left] = nums[right];
            while(left < right && nums[left] <= pivot) left += 1;
            nums[right] = nums[left];
        }
        nums[left] = pivot;
        return left;
    }
	
	// 在左端点，右端点和中间值 三个值里面取一个中间值
    int findPivot(vector<int>& nums, int left, int right){
        int mid = (right - left)/2 + left;
        if(nums[left] < nums[mid]){
            int temp = nums[left];
            nums[left] = nums[mid];
            nums[mid] = temp;
        }
        if(nums[right] < nums[mid]){
            int temp = nums[right];
            nums[right] = nums[mid];
            nums[mid] = temp;
        }
        if(nums[left] > nums[right]){
            int temp = nums[left];
            nums[left] = nums[right];
            nums[right] = temp;
        }
        return nums[left];
    }
};

int main(){
	Solution sol;
	// vector<int> vtor ={1,2,-1,3,-20,4};
	vector<int> vtor = {82,231,-43,543,-66,323,-88,-891,322,-545,32,645};
	int ans = sol.maximumProduct(vtor);
	cout << "resullt is => " << ans << endl;
	for(int i = 0; i < vtor.size(); i++){
		cout << vtor[i] << " ";
	}
	cout << endl;
	return 0;
}
