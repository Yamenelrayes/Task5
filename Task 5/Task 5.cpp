#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

struct tree_node {
    string value;
    tree_node* left;
    tree_node* right;
    tree_node(string val) : value(val), left(nullptr), right(nullptr) {}
};

// This function removes the outermost parentheses
void remove_outer_parentheses(const string& expr, string& cleaned) {
    if (expr.front() == '(' && expr.back() == ')') cleaned = expr.substr(1, expr.length() - 2);
    else cleaned = expr;
}

// here i made it so the code findes the index of the operator with the highest precedence in the expression
void max_operator(const string& expr, const vector<int>& ops, int& selectedIndex) {
    int maxPrec = 0;
    selectedIndex = 0;
    for (int index : ops) {
        char op = expr[index];
        int prec = (op == '+' || op == '-') ? 1 : 2; // '+' and '-' have lower precedence than '*' and '/'.
        if (prec > maxPrec) {
            maxPrec = prec;
            selectedIndex = index;
        }
    }
}

// This function builds the expression tree 
void build_expression_tree(const string& expr, tree_node*& node) {
    stack<int> opStack;
    vector<int> ops; 
    string cleanExpr;
    remove_outer_parentheses(expr, cleanExpr); 
    int len = cleanExpr.length();
    for (int i = 0; i < len; ++i) {
        if (cleanExpr[i] == '(') opStack.push(i);
        else if (cleanExpr[i] == ')') opStack.pop();
        else if (opStack.empty() && (cleanExpr[i] == '+' || cleanExpr[i] == '-' || cleanExpr[i] == '*' || cleanExpr[i] == '/')) {
            ops.push_back(i); 
        }
    }
    if (ops.empty()) { 
        node = new tree_node(cleanExpr);
        return;
    }
    int opIndex;
    max_operator(cleanExpr, ops, opIndex); 
    node = new tree_node(string(1, cleanExpr[opIndex]));
    build_expression_tree(cleanExpr.substr(0, opIndex), node->left); 
    build_expression_tree(cleanExpr.substr(opIndex + 1), node->right); 
}

// This function calculates the expression and returns the result.
void eval_expression_tree(tree_node* node, double& result) {
    if (!node->left && !node->right) { 
        result = stod(node->value);
        return;
    }
    double leftValue, rightValue;
    eval_expression_tree(node->left, leftValue); 
    eval_expression_tree(node->right, rightValue);
    if (node->value == "+") result = leftValue + rightValue;
    else if (node->value == "-") result = leftValue - rightValue;
    else if (node->value == "*") result = leftValue * rightValue;
    else result = leftValue / rightValue;
}

//here we compute the depth of the tree for display
void treeDepth(tree_node* node, int& depth) {
    if (!node) {
        depth = 0;
        return;
    }
    int leftDepth, rightDepth;
    treeDepth(node->left, leftDepth);
    treeDepth(node->right, rightDepth);
    depth = 1 + max(leftDepth, rightDepth);
}

// i made this function to fill in extra lines so the tree visual looks good
void fill_lines(vector<string>& lines, tree_node* node, int row, int col, int depth) {
    if (!node || row >= depth) return;
    int branchLength = (1 << (depth - row - 2));
    lines[row][col] = node->value[0];
    if (node->left) fill_lines(lines, node->left, row + 1, col - branchLength, depth);
    if (node->right) fill_lines(lines, node->right, row + 1, col + branchLength, depth);
}

// Function to display the tree
void display_tree(tree_node* root) {
    int depth;
    treeDepth(root, depth); 
    int width = (1 << depth) - 1; 
    vector<string> displayLines(depth, string(width, ' '));
    fill_lines(displayLines, root, 0, width / 2, depth); 
    for (const string& line : displayLines) {
        cout << line << endl; 
    }
}

int main() {
    string expression = "(8+(3*7)-5)";
    tree_node* root;
    build_expression_tree(expression, root); 
    cout << "Tree:" << endl;
    display_tree(root); 
    double result;
    eval_expression_tree(root, result); 
    cout << "\nResult: " << result << endl; 
    return 0;
}
