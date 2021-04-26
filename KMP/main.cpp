//
//  main.cpp
//  KMP
//
//  Created by Kangyang Wu on 26.04.21.
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
 strStr函数，相当于python中字符串的find函数。
 找出字符串a中b出现的第一个位置(下标从0开始)，不存在返回-1。
 */
int KMP(const string &a, const string &b) {
    int n = a.length(), m = b.length();
    if (m == 0) {
        return 0;
    }
    
    // 计算next数组
    vector<int> next(m);
    for(int i = 1, j = 0; i < m; i++) {
        while (j > 0 && b[i] != b[j]) {
            j = next[j-1];
        }
        if (b[i] == b[j]) {
            j++;
        }
        next[i] = j;
    }

    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && a[i] != b[j]) {
            j = next[j-1];
        }
        if (a[i] == b[j]) {
            j++;
        }
        if (j == m) {
            return i - m + 1;
        }
    }
    return -1;
}

int main(int argc, const char * argv[]) {
    string a = "abeababeabf", b = "abeabf";
    int start_idx = KMP(a, b);
    std::cout << start_idx << endl;
    return 0;
}
