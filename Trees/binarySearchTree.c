struct node *treeSearchRecursive(struct node *root, int target)
{
    if (root == 0 || root->key == target)
    {
        return root;
    }
    else if (root->key > target)
    {
        return treeSearch(root->left, target);
    }
    else
    {
        return treeSearch(root->right, target);
    }
}

struct node *
treeSearch(struct node *root, int target)
{
    while (root != 0 && root->key != target)
    {
        if (root->key > target)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return root;
}

void treeInsert(struct tree **parent, int key) {
    struct tree *newNode;

    for(;;) {
        if(*parent == 0) {
            *parent = malloc(sizeof(*newNode));
            assert(*parent);

            (*parent)->key = key;
            (*parent)->left = 0;
            (*parent)->right = 0;

            return;
        }

        else if((*parent)->key == key) {
            return;
        }
        else if((*parent)->key > key) {
            parent = &(*parent)->left;
        }
        else {
            parent = &(*parent)->right;
        }
    }
}