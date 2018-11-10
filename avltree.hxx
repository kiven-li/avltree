#ifndef        __AVLTREE_H__
#define        __AVLTREE_H__

#include <iostream>

template <typename T>
class AVLNode
{
public:
    T data;
    AVLNode<T>* pLeft;
    AVLNode<T>* pRight;
    AVLNode<T>* pParent;
    int nHeight;
};

template <typename T>
class AVLTree
{
public:
    AVLTree() : pRoot(NULL), nNodeCount(0){}
    virtual ~AVLTree(){ DeleteTree(&pRoot); }
public:
    virtual AVLTree<T>& Insert(T data);
    virtual AVLTree<T>& Delete(T data);
    virtual int Find(T data) const;
    virtual int GetNodeCount() const;
    virtual void Display() const;

private:
    int Max(int a, int b) const;
    int Height(const AVLNode<T>* pNode) const;
    AVLNode<T>* CreateNode(T data);
    AVLNode<T>* DeleteNode(T data, AVLNode<T>* pNode);
    AVLNode<T>* BalanceAdjust(AVLNode<T>* pNode);
    AVLNode<T>* RotateLeft(AVLNode<T>* pNode);
    AVLNode<T>* RotateRight(AVLNode<T>* pNode);
    AVLNode<T>* RotateLeftRight(AVLNode<T>* pNode);
    AVLNode<T>* RotateRightLeft(AVLNode<T>* pNode);
    void DeleteTree(AVLNode<T>** ppRoot);
    void PrintTree(AVLNode<T>* pNode) const;

    AVLTree(const AVLTree<T>&) {}
    AVLTree<T>& operator=(const AVLTree<T>&) {}

private:
    AVLNode<T>* pRoot;
    int nNodeCount;
};

template <typename T>
int AVLTree<T>::Max(int a, int b) const
{
    return (a > b ? a : b);
}

template <typename T>
int AVLTree<T>::Height(const AVLNode<T>* pNode) const
{
    if (NULL == pNode)
        return -1;

    return pNode->nHeight;
}

template <typename T>
AVLTree<T>& AVLTree<T>::Insert(T data)
{
    if(pRoot == NULL)
    {
        pRoot = CreateNode(data);

        return *this;
    }

    AVLNode<T>* pInsertNode = pRoot;

    while(pInsertNode != NULL)
    {
        if(data < pInsertNode->data)
        {
            if(pInsertNode->pLeft == NULL)
            {
                pInsertNode->pLeft = CreateNode(data);
                pInsertNode->pLeft->pParent = pInsertNode;

                pRoot = BalanceAdjust(pInsertNode->pLeft);
                break;
            }

            pInsertNode = pInsertNode->pLeft;
        }
        else if(data > pInsertNode->data)
        {
            if(pInsertNode->pRight == NULL)
            {
                pInsertNode->pRight = CreateNode(data);
                pInsertNode->pRight->pParent = pInsertNode;

                pRoot = BalanceAdjust(pInsertNode->pRight);
                break;
            }

            pInsertNode = pInsertNode->pRight;
        }
        else
        {
            return *this;
        }
    }

    return *this;
}

template <typename T>
AVLTree<T>& AVLTree<T>::Delete(T data)
{
    //std::cout << "Delete nData = " << nData << std::endl;
    //std::cout << "pNode->nData = " << pNode->nData << std::endl;
    //std::cout << "pPNode->nData = " << pPNode->nData << std::endl;

    AVLNode<T>* pCurNode = pRoot;

    while(pCurNode != NULL)
    {
        if(data > pCurNode->data)
        {
            pCurNode = pCurNode->pRight;
        }
        else if(data < pCurNode->data)
        {
            pCurNode = pCurNode->pLeft;
        }
        else
        {
            pRoot = DeleteNode(data, pCurNode);
            break;
        }
    }


    if(pCurNode == NULL)
        std::cout << "没有找到元素 nData = " << data << std::endl;
    //int x;
    //std::cin >> x;

    return *this;//没有找到要删除的元素    
}

template <typename T>
AVLNode<T>* AVLTree<T>::DeleteNode(T data, AVLNode<T>* pNode)
{
    nNodeCount--;

    if(pNode->pLeft && pNode->pRight)//删除节点有左右子树
    {
        AVLNode<T>* pLMaxNode = pNode->pLeft;//删除节点左子树中最大的节点
        AVLNode<T>* pLMaxPNode = pNode;//删除节点左子树中最大节点的父节点

        //将删除节点左孩子的最大节点替换删除节点,然后删除该节点
        if(pLMaxNode->pRight == NULL)
        {
            pNode->data = pLMaxNode->data;
            pNode->pLeft = pLMaxNode->pLeft;

            if(pLMaxNode->pLeft != NULL)
                pLMaxNode->pLeft->pParent = pNode;
        }
        else
        {
            while(pLMaxNode->pRight)
            {
                pLMaxPNode = pLMaxNode;
                pLMaxNode = pLMaxNode->pRight;
            }
            pNode->data = pLMaxNode->data;

            if(pLMaxNode == pLMaxPNode->pRight)//将替换后的删除节点删除
                pLMaxPNode->pRight = pLMaxNode->pLeft;
            else if(pLMaxNode == pLMaxPNode->pLeft)
                pLMaxPNode->pLeft = NULL;
        }
        
        delete pLMaxNode;

        return BalanceAdjust(pLMaxPNode);
    }
    else if(pNode->pLeft)//删除节点只有左子树
    {
        AVLNode<T>* pLeft = pNode->pLeft;

        pNode->data = pLeft->data;

        pNode->pLeft = pLeft->pLeft;
        if (pLeft->pLeft != NULL)
            pLeft->pLeft->pParent = pNode;

        pNode->pRight = pLeft->pRight;
        if (pLeft->pRight != NULL)
            pLeft->pRight->pParent = pNode;
        
        delete pLeft;

        return BalanceAdjust(pNode);
    }
    else if(pNode->pRight)//删除节点只有右子树
    {
        AVLNode<T>* pRight = pNode->pRight;

        pNode->data = pRight->data;

        pNode->pLeft = pRight->pLeft;
        if (pRight->pLeft != NULL)
            pRight->pLeft->pParent = pNode;

        pNode->pRight = pRight->pRight;
        if (pRight->pRight != NULL)
            pRight->pRight->pParent = pNode;
        
        delete pRight;

        return BalanceAdjust(pNode);
    }
    else//删除节点没有子树
    {
        AVLNode<T>* pPNode = pNode->pParent;

        if(pPNode->pLeft == pNode)
            pPNode->pLeft = NULL;
        else if(pPNode->pRight == pNode)
            pPNode->pRight = NULL;

        delete pNode;

        return BalanceAdjust(pPNode);
    }
}

template <typename T>
AVLNode<T>* AVLTree<T>::BalanceAdjust(AVLNode<T>* pNode)
{
    AVLNode<T>* pRoot;
    AVLNode<T>* pPNode;
    
    while(pNode != NULL)//删除节点的子节点进行平衡
    {
        pPNode = pNode->pParent;

        bool bIsLeft = false;
        if(pPNode != NULL && pNode == pPNode->pLeft)
            bIsLeft = true;

        pNode->nHeight = Max(Height(pNode->pLeft), Height(pNode->pRight)) + 1;

        if (Height(pNode->pLeft) - Height(pNode->pRight) == 2)    // AVL树不平衡  执行LL型或者LR型旋转
        {
            if (Height(pNode->pLeft->pLeft) - Height(pNode->pLeft->pRight) == -1)
                pNode = RotateLeftRight(pNode);
            else 
                pNode = RotateLeft(pNode);

            if(pPNode != NULL && bIsLeft)
                pPNode->pLeft = pNode;
            else if(pPNode != NULL)
                pPNode->pRight = pNode;
        }
        else if(Height(pNode->pLeft) - Height(pNode->pRight) == -2)    // AVL树不平衡  执行RR型或者RL型旋转
        {
            if (Height(pNode->pRight->pLeft) - Height(pNode->pRight->pRight) == 1)
                pNode = RotateRightLeft(pNode);
            else
                pNode = RotateRight(pNode);

            if (pPNode != NULL && bIsLeft)
                pPNode->pLeft = pNode;
            else if(pPNode != NULL)
                pPNode->pRight = pNode;
        }

        pRoot = pNode;
        pNode = pPNode;
    }

    return pRoot;
}

template <typename T>
AVLNode<T>* AVLTree<T>::CreateNode(T data)
{
    nNodeCount++;

    AVLNode<T>* pNewNode = new AVLNode<T>();
    pNewNode->data = data;
    pNewNode->nHeight = 0;
    pNewNode->pLeft = pNewNode->pRight = NULL;

    return pNewNode;
}

template <typename T>
int AVLTree<T>::Find(T data) const
{
    AVLNode<T>* pFindNode = pRoot;
    while(pFindNode)
    {
        if(data < pFindNode->data)
            pFindNode = pFindNode->pLeft;
        else if(data > pFindNode->data)
            pFindNode = pFindNode->pRight;
        else
            return 1;//pFindNode->data;
    }
    
    return 0;
}

template <typename T>
AVLNode<T>* AVLTree<T>::RotateLeft(AVLNode<T>* pNode)//左单
{
    AVLNode<T>* pLeftChild;

    pLeftChild = pNode->pLeft;
    pNode->pLeft = pLeftChild->pRight;
    pLeftChild->pRight = pNode;

    pLeftChild->pParent = pNode->pParent;
    pNode->pParent = pLeftChild;

    if(pNode->pLeft)
        pNode->pLeft->pParent = pNode;

    // 结点的位置改变,节点高度要重新计算
    pNode->nHeight = Max(Height(pNode->pLeft), Height(pNode->pRight)) + 1;
    pLeftChild->nHeight = Max(Height(pLeftChild->pLeft), pNode->nHeight) + 1;

    return pLeftChild;
}

template <typename T>
AVLNode<T>* AVLTree<T>::RotateRight(AVLNode<T>* pNode)//右单
{
    AVLNode<T>* pRightChild;

    pRightChild = pNode->pRight;
    pNode->pRight = pRightChild->pLeft;
    pRightChild->pLeft = pNode;

    pRightChild->pParent = pNode->pParent;
    pNode->pParent = pRightChild;

    if(pNode->pRight)
        pNode->pRight->pParent = pNode;

    // 结点的位置改变,节点高度要重新计算
    pNode->nHeight = Max(Height(pNode->pLeft), Height(pNode->pRight)) + 1;
    pRightChild->nHeight = Max(Height(pRightChild->pRight), pNode->nHeight) + 1;

    return pRightChild;
}

template <typename T>
AVLNode<T>* AVLTree<T>::RotateLeftRight(AVLNode<T>* pNode)//左双
{
    pNode->pLeft = RotateRight(pNode->pLeft);

    return RotateLeft(pNode);
}

template <typename T>
AVLNode<T>* AVLTree<T>::RotateRightLeft(AVLNode<T>* pNode)//右双
{
    pNode->pRight = RotateLeft(pNode->pRight);

    return RotateRight(pNode);
}

// 后序遍历树以删除树
template <typename T>
void AVLTree<T>::DeleteTree(AVLNode<T>** ppRoot)
{
    if (NULL == ppRoot || NULL == *ppRoot)
        return;

    DeleteTree(&((*ppRoot)->pLeft));
    DeleteTree(&((*ppRoot)->pRight));
    delete ppRoot;
    *ppRoot = NULL;

    //std::cout << "AVLTree delete completed !" << std::endl;
}

template <typename T>
int AVLTree<T>::GetNodeCount() const
{
    return nNodeCount;
}

template <typename T>
void AVLTree<T>::Display() const
{
     PrintTree(this->pRoot);
}

template <typename T>
void AVLTree<T>::PrintTree(AVLNode<T>* pNode) const
{
    if (NULL == pRoot)
        return;

    if (NULL == pNode)
    {
        return;
    }

    static int n = 0;
    
    if(pNode == pRoot)
    {
        std::cout << "[" << ++n << "]nData = " << pNode->data << ",nParentData= 0 ,";

        if(pNode->pLeft)
            std::cout << "nLeftData= " << pNode->pLeft->data << " ,";
        if(pNode->pRight)
            std::cout << "nRightData= " << pNode->pRight->data << " ,";

        std::cout << "nHeight = " << pNode->nHeight << std::endl;
    }
    else
    {
        std::cout << "[" << ++n << "]nData = " << pNode->data << ",nParentData= " << pNode->pParent->data << " ,";

        if(pNode->pLeft)
            std::cout << "nLeftData= " << pNode->pLeft->data << " ,";
        if(pNode->pRight)
            std::cout << "nRightData= " << pNode->pRight->data << " ,";

        std::cout << "nHeight = " << pNode->nHeight << std::endl;
    }
    PrintTree(pNode->pLeft);
    PrintTree(pNode->pRight);
}
#endif    //__AVLTREE_H__

