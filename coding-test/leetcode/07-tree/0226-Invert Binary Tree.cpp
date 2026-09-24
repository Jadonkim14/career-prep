// 方法一：DFS
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;

        swap(root->left, root->right);
        // =
        // TreeNode* temp = root->left; 
        // root->left = root->right; 
        // root->right = temp;
        //
        invertTree(root->left);
        invertTree(root->right);

        return root;
    }
};

// 方法二：BFS
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;

        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* cur = q.front(); q.pop();
            swap(cur->left, cur->right);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }

        return root;
    }
};