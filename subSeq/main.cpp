//
//  main.cpp
//  subSeq
//
//  Created by Kangyang Wu on 26.04.21.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 函数用来判断字符串b是否是字符串a的子序列，即能否通过删除a中的元素得到b
 这里用的方法类似于KMP算法，KMP是为待查找字符串生成next数组，之后可以
 用next数组重复对任意数量的目标字符串使用。这里是为原始字符串a生成一个
 二维dp表，记录了从a中每个位置开每个字母最先出现的位置，使用了动态规划的思想
 从右向左遍历生成。
 f[i][j] = i if a[i] == j + 'a' else f[i+1][j]
 之后便可以重复使用dp表对任意数量的b进行判断
 */
bool subSequence(const string &a, const string &b) {
    int n = a.length(), m = b.length();
    // 初始化dp表
    vector<vector<int>> f(n, vector<int>(26, 0));
    f.push_back(vector<int>(26, n));
    for (int i = n - 1; i > -1; i--) {
        for (int j = 0; j < 26; j++) {
            if (a[i] == j + 'a') {
                f[i][j] = i;
            } else {
                f[i][j] = f[i+1][j];
            }
        }
    }
    
    int add = 0;
    for (int i = 0; i < m; i++) {
        if (f[add][b[i]-'a'] == n) {
            return false;
        }
        add = f[add][b[i]-'a'] + 1;
    }
    return true;
}

int main(int argc, const char * argv[]) {
    string a = "ahbgdc", b = "abc";
    bool is_subseq = subSequence(a, b);
    std::cout << b << (is_subseq ? " is a" : " is not a")
              << " subsquence of " << a << endl;
    return 0;
}
