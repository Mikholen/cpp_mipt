#include <iostream>
#include <memory>
#include <queue>
#include <stack>


class Tree {

public:

    struct Node {

        int value = 0;

        std::shared_ptr<Node> left  = nullptr;
        std::shared_ptr<Node> right = nullptr;
        std::weak_ptr<Node> parent;

        Node(int v) : value(v) {

            std::cout << "Node(" << value << ") created\n";
        }

        ~Node() {

            std::cout << "Node(" << value << ") destroyed\n";
        }
    };

    std::shared_ptr<Node> root = nullptr;

    void traverse_v1() const {

        std::cout << "\nBFS traversal:\n";

        if (!root) return;

        std::queue<std::shared_ptr<Node>> q;
        q.push(root);

        while (!q.empty()) {

            auto current = q.front();
            q.pop();

            std::cout << current->value << " ";

            if (current->left)
                q.push(current->left);

            if (current->right)
                q.push(current->right);
        }

        std::cout << "\n";
    }


    void traverse_v2() const {

        std::cout << "\nDFS traversal (preorder):\n";

        if (!root) return;

        std::stack<std::shared_ptr<Node>> st;
        st.push(root);

        while (!st.empty()) {

            auto current = st.top();
            st.pop();

            std::cout << current->value << " ";

            if (current->right)
                st.push(current->right);

            if (current->left)
                st.push(current->left);
        }

        std::cout << "\n";
    }
};


int main() {

    {
        Tree tree;

        tree.root = std::make_shared<Tree::Node>(1);

        tree.root->left  = std::make_shared<Tree::Node>(2);
        tree.root->right = std::make_shared<Tree::Node>(3);

        tree.root->left->parent  = tree.root;
        tree.root->right->parent = tree.root;

        tree.root->left->left  = std::make_shared<Tree::Node>(4);
        tree.root->left->right = std::make_shared<Tree::Node>(5);

        tree.root->right->left  = std::make_shared<Tree::Node>(6);
        tree.root->right->right = std::make_shared<Tree::Node>(7);

        tree.root->left->left->parent   = tree.root->left;
        tree.root->left->right->parent  = tree.root->left;
        tree.root->right->left->parent  = tree.root->right;
        tree.root->right->right->parent = tree.root->right;

        tree.traverse_v1();
        tree.traverse_v2();
    }
    
}

// Score is 9/10
