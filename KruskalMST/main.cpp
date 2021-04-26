//
//  main.cpp
//  KruskalMST
//
//  Created by Kangyang Wu on 22.04.21.
//

#include <iostream>
#include <map>
using namespace std;

/* 并查集(Disjoint-Set)节点(node)的定义 */
typedef struct _dsetNode
{
    // 节点的值/名字
    char data;
    // 当前节点作为代表节点时集合的大小(包含节点的数量)，仅当为代表节点时有意义
    int rank;
    // 节点的父母节点
    _dsetNode *parent;
}dsetNode;

/* 记录某一时刻集合的数目 */
int count = 0;

/* map用节点值/名字来获取节点的地址 */
map<char, dsetNode*> mymap;


/* MakeSet函数根据节点值构建节点，并将其存进mymap中 */
void MakeSet(char data)
{
    dsetNode* node = new dsetNode;
    node->data = data;
    mymap[data] = node;
    // 初始rank为1，代表集合包含1个节点即节点本身
    node->rank = 1;
    // 初始节点的父母节点为节点本身
    node->parent = node;
    // ::(作用域运算符)用来指定访问全局变量里的count
    ::count++;
}

/*
 函数FindSetRoot输入是一个节点的指针，返回该节点所在集合的代表节点的指针，这里用了路径压缩，
 也就是说在查找过程中经过的节点都会直接连在代表节点的后面，以后这些节点的查找时间复杂度就是O(1)。
 */
dsetNode *FindSetRoot(dsetNode *node)
{
    if (node != node->parent)
    {
        node->parent = FindSetRoot(node->parent);
    }
    return node->parent;
}

/* FindSet函数根据节点值来获取该节点所在集合的代表节点值 */
char FindSet(char x)
{
    dsetNode *node = mymap[x];
    node = FindSetRoot(node);
    return node->data;
}

/* isconnected函数检查两个节点所在集合是否相连即是否属于同一集合 */
bool isconnected(char x, char y)
{
    return (FindSet(x) == FindSet(y));
}

/* LinkSet函数用来合并两个集合，rank值小的要合并到rank值大的集合里 */
void LinkSet(char x, char y)
{
    if (isconnected(x, y))
    {
        return;
    }
    dsetNode *node1 = mymap[x];
    dsetNode *node2 = mymap[y];
    if (node1->rank > node2->rank)
    {
        node2->parent = node1;
        // 合并完以后更新rank值
        node1->rank += node2->rank;
    }
    else
    {
        node1->parent = node2;
        node2->rank += node1->rank;
    }
    // 每合并两个集合则总集合数量减1
    ::count--;
}

/* UnionSet函数用来调用LinkSet来合并两个集合 */
void UnionSet(char x, char y)
{
    LinkSet(FindSet(x), FindSet(y));
}

/* freeset用来释放程序运行过程中动态申请的内存，防止内存泄漏 */
void freeset(char arr[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        dsetNode *node = mymap[arr[i]];
        delete node;
    }
}

/* QuickSort原地快速排序 */
void QuickSort(int array[], int low, int high, char edgein[], char edgeout[])
{
    int i = low, j = high;
    // 使用中间位置的值为中轴
    int pivot = array[(low + high)/2];
    
    while (i <= j)
    {
        while (array[i] < pivot)
        {
            ++i;
        }
        while (array[j] > pivot)
            
        {
            --j;
        }
        
        if (i <= j)
        {
            // 交换i和j位置的数
            int i_tmp = array[i];
            array[i] = array[j];
            array[j] = i_tmp;
            
            char c_tmp = edgein[i];
            edgein[i] = edgein[j];
            edgein[j] = c_tmp;
            
            c_tmp = edgeout[i];
            edgeout[i] = edgeout[j];
            edgeout[j] = c_tmp;
            
            ++i;
            --j;
        }
    }
    
    // 递归对子序列进行排序
    if (i < high)
    {
        QuickSort(array, i, high, edgein, edgeout);
    }
    if (j > low)
    {
        QuickSort(array, low, j, edgein, edgeout);
    }
}

/* MSTKruskal函数用来得到最小生成树 */
void MSTKruskal(char vertices[], char edgein[], char edgeout[], int weight[], int n, int m)
{
    // 创建初始化节点
    for (int i = 0; i < n; ++i)
    {
        MakeSet(vertices[i]);
    }
    
    // 根据权重对边界进行非降序排序
    QuickSort(weight, 0, m-1, edgein, edgeout);
    
    int A = 0;
    cout << "最小生成树包含的边界：" << endl;
    /*
     i用来遍历所有边界，count代表目前集合的数目，如果只剩一个集合了，
     就说明已经得到最小生成树了，可以提前结束循环。
     */
    for (int i = 0; i < m && ::count > 1; ++i)
    {
        // 如果当前边界会造成环则忽略，即边界两端节点属于同一集合
        if (isconnected(edgein[i], edgeout[i]))
        {
            continue;
        }
        
        // 将边界添加到最小生成树中，即合并边界两端节点所在的集合
        UnionSet(edgein[i], edgeout[i]);
        A += weight[i];
        cout << "\t" << edgein[i] << "--" << edgeout[i] << endl;
    }
    cout << "最小生成树总共的权重为： " << A << endl;
    
    // 释放用new动态申请的内存，防止内存泄漏
    freeset(vertices, n);
}


int main(int argc, const char * argv[]) {
    /*
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    char edgein[] = {'A', 'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D'};
    char edgeout[] = {'B', 'D', 'C', 'D', 'E', 'D', 'F', 'E', 'F'};
    int weight[] = {6, 5, 3, 8, 2, 2, 6, 5, 3};
     */
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};

    char edgein[]   = {'A', 'A', 'B', 'B', 'C', 'D', 'D', 'E', 'E', 'F', 'G', 'H'};
    char edgeout[]  = {'B', 'D', 'C', 'E', 'F', 'E', 'G', 'F', 'H', 'I', 'H', 'I'};
    int weight[]    = { 4,   2,   6,   4,   1,   6,   4,   3,   2,   2,   4,   3 };
    
    // 节点数目n和边界数目m
    int n = sizeof(vertices) / sizeof(vertices[0]);
    int m = sizeof(weight) / sizeof(weight[0]);
    MSTKruskal(vertices, edgein, edgeout, weight, n, m);
    
    return 0;
}

/*
 输出：
 最小生成树包含的边界：
     B--E
     C--D
     D--F
     A--C
     D--E
 最小生成树总共的权重为： 15
 */
