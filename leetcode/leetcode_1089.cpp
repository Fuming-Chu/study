class Solution {
public:
    void duplicateZeros(vector<int>& arr) {

        //思路：画图更容易一些
        //1 找到最后一个复写的数
        int cur = 0;
        int dest = -1;
        int n = arr.size();
        while(cur < n)
        {
            if(arr[cur] == 0)
                dest += 2;
            else
                dest++;

            if(dest >= n -1)
                break;
            cur++;
        }
        //处理边界情况
        if( dest == n)
        {
            arr[n-1] = 0;
            cur--;
            dest -= 2;
        }

        //2 开始倒着复写
        while(cur >= 0)
        {
            if(arr[cur])
            {
                arr[dest] = arr[cur];
                dest--;
                cur--;
            }
            else
            {
                arr[dest] = 0;
                arr[dest-1] = 0;
                cur--;
                dest -= 2;
            }

        }
        
    }
};