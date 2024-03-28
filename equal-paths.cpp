#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int path(Node* n, bool& res){
    if(!res)
        return -1;
    else if(n == nullptr)
        return 0;
    
    int l = path(n -> left, res);
    int r = path (n -> right, res);
    if(l != 0 && r != 0 && l != r)
        res = false;
    return l > r ? l + 1 : r + 1;
}

bool equalPaths(Node * root)
{
    // Add your code below
    bool res = true;
    path(root, res);
    return res;
}

