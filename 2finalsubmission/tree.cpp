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

TreeNode *E(std::vector<Token> &t)
{
    TreeNode *c, *t1, *t2;
    t1 = T(t);
    while (token_vector_index < t.size() &&
           (t[token_vector_index].id == TOKEN_PLUS || t[token_vector_index].id == TOKEN_MINUS))
    {
        if (t[token_vector_index].id == TOKEN_PLUS)
        {
            token_vector_index++;
            t2 = T(t);
            c = new TreeNode("+", t1, t2);
            t1 = c;
        }
        else if (t[token_vector_index].id == TOKEN_MINUS)
        {
            token_vector_index++;
            t2 = T(t);
            c = new TreeNode("-", t1, t2);
            t1 = c;
        }
    }

    return t1;
}

TreeNode *T(std::vector<Token> &t)
{
    TreeNode *c, *t1, *t2;
    t1 = Ex(t);

    while (t[token_vector_index].id == TOKEN_MULT || t[token_vector_index].id == TOKEN_DIV || t[token_vector_index].value == "mod")
    {
        std::string op = t[token_vector_index].value;
        token_vector_index++;
        t2 = Ex(t);
        c = new TreeNode(op, t1, t2);
        t1 = c;
    }

    return t1;
}

TreeNode *Ex(std::vector<Token> &t)
{
    TreeNode *c, *t1, *t2;

    t1 = F(t);
    while (token_vector_index < t.size() && t[token_vector_index].id == TOKEN_EXP)
    {
        if (t[token_vector_index].id == TOKEN_EXP)
        {
            token_vector_index++;
            t2 = F(t);
            c = new TreeNode("^", t1, t2);
            t1 = c;
        }
    }

    return t1;
}

TreeNode *F(std::vector<Token> &t)
{
    TreeNode *c;

    if (t[token_vector_index].id == TOKEN_MINUS)
    { // if its a negative number
        token_vector_index++;
        TreeNode *operand = F(t);
        return new TreeNode("-", operand, nullptr);
    }

    if (t[token_vector_index].id == TOKEN_PLUS)
    { // if its a positive number
        // std::cout << "HERE1" << std::endl;
        token_vector_index++;
        TreeNode *operand = F(t);
        return new TreeNode("+", operand, nullptr);
    }

    if (is_number(t[token_vector_index].value))
    {
        int value = std::stoi(t[token_vector_index].value);
        token_vector_index++;
        return new TreeNode(value);
    }

    if (t[token_vector_index].id == TOKEN_LPAREN)
    {
        token_vector_index++;
        c = E(t);
        if (token_vector_index == t.size() || t[token_vector_index].id != TOKEN_RPAREN)
        {
            std::cerr << "Expected ')'" << std::endl;
        }
        token_vector_index++;
        return c;
    }

    if (t[token_vector_index].id == TOKEN_RPAREN)
    {
        std::cerr << "Unmatched ')'" << std::endl;
        token_vector_index++;
    }

    std::cerr << "Unexpected token: " << t[token_vector_index].value << std::endl;
    return nullptr;
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
        else if (root->op == "^")
        {
            ope = "pow";
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
