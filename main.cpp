/*利用数组存储的Huffmantree，循环找最小两个数构造树，利用栈和队列存储输出*/
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

/*树节点*/
struct HuffmanNode {
    char data;
    stack<int> codeStk;
    int lchild, rchild, parent, weight;
    HuffmanNode() : data('0'), lchild(0), rchild(0), parent(0), weight(0) {};
};

struct minNode {
    int tag, weight;

    minNode() : tag(0), weight(100000) {};
};

/*树类，以1开始的数组进行存储，n个叶子节点需要2n-1个存储空间，0位置置空*/
class HuffmanTree {
    HuffmanNode *hTree;
    int size;
public:
    void buildTree();

    void encode(string &str);

    void decode(string &code);

    void showCode(int i)
    {
       stack<int> temp=hTree[i].codeStk;
       while (!temp.empty())
       {
           cout<<temp.top();
           temp.pop();
       }
    };
};

void HuffmanTree::buildTree() {
    /*构建数组，输入数据和权重*/
    cin >> size;
    hTree = new HuffmanNode[2 * size];
    for (int i = 1; i <=size; ++i) {
        char ch;
        cin >> ch;
        while (ch== ' ') {
            cin >> ch;
        }
        hTree[i].data=ch;
    }
    for (int j = 1; j <=size; ++j) {
        cin >> hTree[j].weight;
    }

    int newNodeTag = size + 1;
/*建立赫夫曼树表*/
    while (newNodeTag != 2 * size) {
/*选出权重最小的两个未分配的节点，放入临时队列，使用minNode封装部分数据*/
        queue<minNode> minQ;
        for (int j = 0; j < 2; ++j) {
            minNode minnode;
            for (int i = 1; i <= newNodeTag-1; ++i) {
                if (hTree[i].parent ==0 && hTree[i].weight < minnode.weight) {
                    minnode.tag = i;
                    minnode.weight = hTree[i].weight;
                }
            }
            /*exclude min node*/
            hTree[minnode.tag].parent = -1;
            minQ.push(minnode);
        }
/*新建节点，定义关系*/
        hTree[newNodeTag].weight = minQ.front().weight + minQ.back().weight;
        if(hTree[minQ.front().tag].weight>hTree[minQ.back().tag].weight)
        {
            swap(minQ.front(),minQ.back());
        }
        hTree[newNodeTag].lchild = minQ.front().tag;
        hTree[newNodeTag].rchild = minQ.back().tag;
        hTree[minQ.front().tag].parent = newNodeTag;
        hTree[minQ.back().tag].parent = newNodeTag;
        newNodeTag++;
        while (!minQ.empty()) {
            minQ.pop();
        }
    }

/*输出各个叶子节点的译码*/
    for (int k = 1; k <= size; ++k) {
        int t = k;
        stack<int> tempSt;
        while (hTree[t].parent) {
            int bit=0;
            if (t == hTree[hTree[t].parent].rchild) {
                bit = 1;
            }
            tempSt.push(bit);
            t=hTree[t].parent;
        }
        int i = 0;
        cout<<hTree[k].data<<" :";
        hTree[k].codeStk=tempSt;
        showCode(k);
        cout<<endl;
    }
}

/*编码算法,遍历输出对应编码，错误输入有对应输出*/
void HuffmanTree::encode(string &str) {
    queue<int> Qint;
    bool succeed=true;
    for (int i = 0; i < str.size(); ++i) {
        int j;
        for (j = 1; j <=size; ++j) {
            if(hTree[j].data==str[i])
            {
                Qint.push(j);
            }
        }
        /*输入出错，找不到节点*/
        if(j==size&&str[i]!=hTree[j].data)
        {
            succeed= false;
        }
    }
    if(!succeed)
    {
        cout<<"error";
    } else{
        while (!Qint.empty())
        {
            showCode(Qint.front());
            Qint.pop();
        }
    }
    cout << endl;
}

void HuffmanTree::decode(string &code) {

    bool succeed = true,complete= false;
    queue<char> outStr;
    HuffmanNode p=hTree[2*size-1];
    for (int i = 0; i < code.size(); ++i) {
        if(code[i]=='0')
        {
            if (!p.lchild) {
                succeed=false;
                break;
            } else p = hTree[p.lchild];
        }
        else if(code[i]=='1')
        {
            if(!p.rchild)
            {
                succeed= false;
                break;
            }else p = hTree[p.rchild];
        }
        else succeed=false;
        /*找到一个叶子节点*/
        if(p.lchild==0&&p.rchild==0)
        {
            outStr.push(p.data);
            p=hTree[size*2-1];
            complete= true;
        }
        else{complete= false;}
    }
    if (!succeed||!complete) {
        cout << "error!";
    } else {
        while (!outStr.empty()) {
            cout << outStr.front();
            outStr.pop();
        }
    }
    cout << endl;
}

/*译码算法，遇0跳到左节点，1右节点，直到遇到叶子节点，错误输入有对应输出*/
int main() {
    int t;
    cin >> t;
    while (t--) {
        HuffmanTree *huffmanTree = new HuffmanTree;
        huffmanTree->buildTree();
        string str, code;
        cin >> str >> code;
        huffmanTree->encode(str);
        huffmanTree->decode(code);
    }
}