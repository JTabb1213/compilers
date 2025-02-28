#include "tree.h"

size_t token_vector_index = 0;

void printTokens(const std::vector<Token> &t) // for debugging
{
    std::cout << "vector size is: " << t.size() << std::endl;
    for (const Token &token : t)
    {
        std::cout << "Token: " << token.value << std::endl;
    }
}

bool is_number(const std::string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

TreeNode *E(std::vector<Token> t)
{
    TreeNode *left = T(t);
    // printTokens(t);
    // std::cout << "Here1" << std::endl;
    while (token_vector_index < t.size() && (t[token_vector_index].value == "+" || t[token_vector_index].value == "-"))
    {
        // std::cout << "here";
        std::string op = t[token_vector_index].value;
        token_vector_index++;

        TreeNode *right = T(t);
        left = new TreeNode(op, left, right);
    }

    return left;
}

TreeNode *T(std::vector<Token> t)
{
    TreeNode *left = F(t);

    while (token_vector_index < t.size() && (t[token_vector_index].value == "*" || t[token_vector_index].value == "/" || t[token_vector_index].value == "mod"))
    {
        std::string op = t[token_vector_index].value;
        token_vector_index++;

        TreeNode *right = F(t);
        left = new TreeNode(op, left, right);
    }

    return left;
}

TreeNode *F(std::vector<Token> t)
{
    if (t[token_vector_index].value == "(")
    {                         // for handling the parenthasis.
        token_vector_index++; // skip the ( and process what comes next as an expression until a ) is encountered.
        TreeNode *expr = E(t);
        if (token_vector_index < t.size() && t[token_vector_index].value == ")")
        {
            token_vector_index++; // skip past the ) and return the expression
        }
        else if (token_vector_index > t.size())
        { // if the token index goes out of bounds
            std::cout << "No closing )!" << std::endl;
        }
        // token_vector_index++;
        return expr;
    }
    else if (token_vector_index < t.size() && is_number(t[token_vector_index].value))
    {
        int value = std::stoi(t[token_vector_index].value);
        token_vector_index++;
        return new TreeNode(value);
    }

    if (t[token_vector_index].value == ")")
    { // if a closing ) was found without a (, its an error
        std::cout << "Unexpected symbol: " << t[token_vector_index].value << std::endl;
        token_vector_index++;
    }

    return nullptr; // if neither a ( or a digit isnt found at this level, which should not happen.
}

void printTree(TreeNode *root, std::string indent)
{
    std::string ope;
    if (!root)
        return;

    if (!root->op.empty())
    {
        if (root->op == "+")
        {
            ope = "add";
        }
        else if (root->op == "-")
        {
            ope = "sub";
        }

        else if (root->op == "*")
        {
            ope = "mult";
        }

        else if (root->op == "/")
        {
            ope = "div";
        }

        else if (root->op == "mod")
        {
            ope = "mod";
        }
        else
        {
            ope = "Unknown operator";
        }
        std::cout << indent << root->op << " (" << ope << ")" << std::endl;
    }
    else
    {
        std::cout << indent << root->value << std::endl;
    }

    std::string newIndent = indent + "  ";
    if (root->left)
        printTree(root->left, newIndent);
    if (root->right)
        printTree(root->right, newIndent);
}

void deleteTree(TreeNode *node)
{
    token_vector_index = 0;
    if (node)
    {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}
