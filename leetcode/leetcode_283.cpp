class Solution {
public:

    //方法1
    //void moveZeroes(vector<int>& nums) {
    //     for (int cur = 0, dest = -1; cur < nums.size();) {
    //         if (nums[cur] == 0) {
    //             cur++;
    //         } else {
    //             swap(nums[dest + 1], nums[cur]);
    //             dest++;
    //             cur++;
    //         }
    //     }
    // }

    //方法2
        void moveZeroes(vector<int>& nums)
        {
        for (int cur = 0, dest = -1; cur < nums.size();cur++) {
            if (nums[cur] != 0) 
                swap(nums[++dest], nums[cur]);
        }
    }
};