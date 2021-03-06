// https://github.com/13959582448/dataStructure-C-
// https://blog.csdn.net/milanla
// author:"hch"

#include<iostream>
#include"queue.h"
using namespace std;

template<typename T>
class BSTnode
{
public:
    T data;
    BSTnode<T>* lchild,*rchild; 
public:
    BSTnode(const T v):data(v),lchild(nullptr),rchild(nullptr){}
};

template<typename T>
class BSTree
{
public:
    BSTnode<T>* root;
public:
    BSTree():root(nullptr){} //构造函数

    virtual BSTnode<T>* create(); //构造二叉排序树
    inline void print(BSTnode<T>*); //方便输出
    virtual bool insertNode(BSTnode<T>*&,const T); //插入一个节点
    void preOrder(BSTnode<T>*); //先序遍历
    void inOrder(BSTnode<T>*); //中序遍历
    void postOrder(BSTnode<T>*); //后序遍历
    void Order(BSTnode<T>*); //层次遍历
    void destroySubtree(BSTnode<T>*); //销毁一个节点和它的子树
    virtual void deleteNode(const T); //删除一个特定的节点
    int getHeight(BSTnode<T>*); //获取某个节点的高度

    ~BSTree(); //析构函数
};

template<typename T>
bool BSTree<T>::insertNode(BSTnode<T>*& node,const T v) //插入节点 "引用"指针
{
    if(!node) //空
    {
        node=new BSTnode<T>(v); //创建一个节点
        return true;
    }
    else if(node->data==v) //相等
        return false; 
    else if (v>node->data) //大于插入右子树
        return insertNode(node->rchild,v);
    else //小于插入左子树
        return insertNode(node->lchild,v);
}

template<typename T>
BSTnode<T>* BSTree<T>::create() //创建排序树
{
    T value;
    cout<<"请输入值(输入完成后按回车和ctrl-d即可结束):";
    while(cin>>value)
    {
        insertNode(root,value);
    }
    cout<<"创建成功"<<endl;
    return root;
}

template<typename T>
inline void BSTree<T>::print(BSTnode<T>* node) //输出
{
    cout<<node->data<<" ";
}

template<typename T>
void BSTree<T>::preOrder(BSTnode<T>* n) //先序遍历
{
    if(n)
    {
        print(n); //先打印本节点
        preOrder(n->lchild); //然后子树
        preOrder(n->rchild); //最后右子树
    }
}

template<typename T>
void BSTree<T>::inOrder(BSTnode<T>* n) //中序遍历
{
    if(n)
    {
        inOrder(n->lchild); //先左子树
        print(n); //然后本节点
        inOrder(n->rchild); //最后右子树
    }
}

template<typename T>
void BSTree<T>::postOrder(BSTnode<T>* n) //后序遍历
{
    if(n)
    {
        postOrder(n->rchild); //先右子树
        postOrder(n->lchild); //然后左子树
        print(n); //最后本节点

    }
}

template<typename T>
void BSTree<T>::Order(BSTnode<T>* n) //层次遍历
{
    BSTnode<T>* tmp;
    queue<BSTnode<T>* > q(100); //这个可以看我写过的队列的定义
    q.enQueue(root);
    while(!q.isEmpty()) //队列不为空
    {
        q.deQueue(tmp);
        cout<<tmp->data<<" ";
        if(tmp->lchild) q.enQueue(tmp->lchild); //左孩子不为空
        if(tmp->rchild) q.enQueue(tmp->rchild); //右孩子不为空
    }
}

template<typename T>
void BSTree<T>::deleteNode(const T value) //删除一个特点的节点
{
    if(!root) return;
    BSTnode<T>* pre=new BSTnode<T>(0); //匹配节点的双亲节点指针
    pre->rchild=root;
    BSTnode<T>* prehead=pre; //prehead只是用于回收pre的空间，因为pre将会移动
    BSTnode<T>* current=root; //用于查询遍历的节点指针
    char turn='r'; //判断匹配节点current是pre的左孩子还是右孩子
    while(current&&current->data!=value)
    {
        if(value>current->data) //转向右子树
        {
            pre=current;
            turn='r'; //说明向右转
            current=current->rchild;
        }
        else //转向左子树
        {
            pre=current;
            turn='l'; //说明向左转
            current=current->lchild;
        }
    }

    if(!current) //节点不存在
    {
        cout<<"找不到匹配的节点"<<endl;
        return;
    }

    if((!current->lchild)&&(!current->rchild)) //匹配节点的左右孩子均为空
    {
        if(turn=='l') pre->lchild=nullptr; //pre->lchild就是current
        else pre->rchild=nullptr; //pre->rchild就是current
    }
    else if(!current->lchild) //只存在右子树
    {
        if(turn=='l') pre->lchild=current->rchild;
        else pre->rchild=current->rchild;
    }
    else if(!current->rchild) //只存在左子树
    {
        if(turn=='l') pre->lchild=current->lchild;
        else pre->rchild=current->lchild;
    }
    else
    {
        
        BSTnode<T>* tmp=nullptr;
        if(turn=='l')
        {
            pre->lchild=current->lchild;
        }
        else
        {
            pre->rchild=current->lchild;
        }
        tmp=current->lchild;
        while(tmp->rchild) tmp=tmp->rchild; //直到匹配节点的左子树的最右边
        tmp->rchild=current->rchild;
    }
    if(current==root) //如果要删除根节点
    {
        BSTnode<T>* ans;
        if(current->lchild) ans=current->lchild;
        else ans=current->rchild;
        root=ans;
        delete current;
        delete pre;
    }
    else
    {
        delete current;
        delete prehead;
    }

}

template<typename T>
int BSTree<T>::getHeight(BSTnode<T>* n) //返回高度
{
    if(!n) return 0; //空的节点高度为0
    int lHeight=getHeight(n->lchild);
    int rHeight=getHeight(n->rchild); //左右子树的高度
    return lHeight>rHeight?(lHeight+1):(rHeight+1); //每个节点的高度是其子树高度+1
}

template<typename T>
void BSTree<T>::destroySubtree(BSTnode<T>* node) //删除某个节点及其子树
{
    if(node->lchild) destroySubtree(node->lchild);
    if(node->rchild) destroySubtree(node->rchild);
    delete node;
}

template<typename T>
BSTree<T>::~BSTree() //析构函数
{
    if(root) destroySubtree(root); //如果根节点不为空
}
