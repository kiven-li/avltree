#include "avltree.hxx"
#include <iostream>
#include <exception>

int main()
{
    try
    {
        AVLTree<int> avl;
        for(int i = 1; i < 8; i++)
        {
            avl.Insert(i);
        }
        
        avl.Delete(1);
        avl.Display();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
