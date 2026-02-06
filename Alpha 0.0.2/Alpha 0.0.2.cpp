// Alpha 0.0.2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>

using namespace std;

const double EPSILON = 1e-6;  // 浮点数比较精度

// 存储表达式的结构体
struct Expression {
    string expr;
    double value;

    Expression(const string& e, double v) : expr(e), value(v) {}
};

// 存储所有可能的表达式
vector<Expression> expressions;

// 检查两个浮点数是否相等（考虑精度误差）
bool isEqual(double a, double b) {
    return fabs(a - b) < EPSILON;
}

// 尝试所有可能的运算
void applyOperation(vector<double>& numbers, vector<string>& exprs, int i, int j, char op,
    double result, const string& newExpr) {
    // 保存原来的值
    double num1 = numbers[i];
    double num2 = numbers[j];
    string expr1 = exprs[i];
    string expr2 = exprs[j];

    // 应用运算
    numbers[i] = result;
    exprs[i] = newExpr;

    // 移除第j个元素
    numbers.erase(numbers.begin() + j);
    exprs.erase(exprs.begin() + j);
}

// 恢复原来的值
void restoreOperation(vector<double>& numbers, vector<string>& exprs, int i, int j,
    double num1, double num2, const string& expr1, const string& expr2) {
    // 恢复第j个元素
    numbers.insert(numbers.begin() + j, num2);
    exprs.insert(exprs.begin() + j, expr2);

    // 恢复第i个元素
    numbers[i] = num1;
    exprs[i] = expr1;
}

// 递归求解
bool solve24(vector<double>& numbers, vector<string>& exprs) {
    int n = numbers.size();

    // 如果只剩一个数，检查是否等于24
    if (n == 1) {
        if (isEqual(numbers[0], 24.0)) {
            expressions.push_back(Expression(exprs[0], numbers[0]));
            return true;
        }
        return false;
    }

    bool found = false;

    // 遍历所有可能的数对
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double a = numbers[i];
            double b = numbers[j];
            string exprA = exprs[i];
            string exprB = exprs[j];

            // 加法
            vector<double> newNumbers = numbers;
            vector<string> newExprs = exprs;

            newNumbers[i] = a + b;
            newExprs[i] = " (" + exprA + " +" + exprB + " )";
            newNumbers.erase(newNumbers.begin() + j);
            newExprs.erase(newExprs.begin() + j);

            if (solve24(newNumbers, newExprs)) {
                found = true;
            }

            // 减法（a - b）
            newNumbers = numbers;
            newExprs = exprs;
            newNumbers[i] = a - b;
            newExprs[i] = " (" + exprA + " -" + exprB + " )";
            newNumbers.erase(newNumbers.begin() + j);
            newExprs.erase(newExprs.begin() + j);

            if (solve24(newNumbers, newExprs)) {
                found = true;
            }

            // 减法（b - a）
            newNumbers = numbers;
            newExprs = exprs;
            newNumbers[i] = b - a;
            newExprs[i] = " (" + exprB + " -" + exprA + " )";
            newNumbers.erase(newNumbers.begin() + j);
            newExprs.erase(newExprs.begin() + j);

            if (solve24(newNumbers, newExprs)) {
                found = true;
            }

            // 乘法
            newNumbers = numbers;
            newExprs = exprs;
            newNumbers[i] = a * b;
            newExprs[i] = " (" + exprA + " *" + exprB + " )";
            newNumbers.erase(newNumbers.begin() + j);
            newExprs.erase(newExprs.begin() + j);

            if (solve24(newNumbers, newExprs)) {
                found = true;
            }

            // 除法（a / b），确保b不为0
            if (!isEqual(b, 0.0)) {
                newNumbers = numbers;
                newExprs = exprs;
                newNumbers[i] = a / b;
                newExprs[i] = " (" + exprA + " /" + exprB + " )";
                newNumbers.erase(newNumbers.begin() + j);
                newExprs.erase(newExprs.begin() + j);

                if (solve24(newNumbers, newExprs)) {
                    found = true;
                }
            }

            // 除法（b / a），确保a不为0
            if (!isEqual(a, 0.0)) {
                newNumbers = numbers;
                newExprs = exprs;
                newNumbers[i] = b / a;
                newExprs[i] = " (" + exprB + " /" + exprA + " )";
                newNumbers.erase(newNumbers.begin() + j);
                newExprs.erase(newExprs.begin() + j);

                if (solve24(newNumbers, newExprs)) {
                    found = true;
                }
            }
        }
    }

    return found;
}

// 去重表达式（简单的字符串去重）
vector<string> removeDuplicates(const vector<string>& exprs) {
    set<string> uniqueExprs;
    vector<string> result;

    for (const string& expr : exprs) {
        // 标准化表达式：移除多余的空格
        string normalized = expr;
        normalized.erase(remove(normalized.begin(), normalized.end(), ' '), normalized.end());

        if (uniqueExprs.find(normalized) == uniqueExprs.end()) {
            uniqueExprs.insert(normalized);
            result.push_back(expr);
        }
    }

    return result;
}

// 显示所有解法
void showSolutions(const vector<int>& numbers) {
    cout << "\n数字: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << "\n\n";

    // 转换为double和string
    vector<double> numDouble(numbers.begin(), numbers.end());
    vector<string> exprs;
    for (int num : numbers) {
        exprs.push_back(to_string(num));
    }

    // 清空之前的表达式
    expressions.clear();

    // 求解
    bool hasSolution = solve24(numDouble, exprs);

    if (!hasSolution) {
        cout << "表达式无解！\n";
        return;
    }

    // 提取并去重表达式
    vector<string> allExprs;
    for (const auto& exp : expressions) {
        allExprs.push_back(exp.expr);
    }

    vector<string> uniqueExprs = removeDuplicates(allExprs);

    cout << "找到 " << uniqueExprs.size() << " 种解法：\n";
    cout << "============================================\n";

    for (size_t i = 0; i < uniqueExprs.size(); i++) {
        cout << i + 1 << ". " << uniqueExprs[i] << " = 24\n";

        //// 每显示10个解后暂停一下
        //if ((i + 1) % 10 == 0 && i + 1 < uniqueExprs.size()) {
        //    cout << "\n按回车键继续显示...";
        //    cin.ignore();
        //    cout << endl;
        //}
    }

    cout << "============================================\n";
}

// 显示帮助信息
void showHelp() {
    cout << "\n============================================\n";
    cout << "            24点自动求解器\n";
    cout << "============================================\n";
    cout << "使用方法：\n";
    cout << "1. 输入4个1-13之间的整数\n";
    cout << "2. 程序会自动计算所有可能的表达式\n";
    cout << "3. 每个数字必须使用且只能使用一次\n";
    cout << "4. 可以使用 + - * / 和括号\n";
    cout << "5. 输入 0 0 0 0 退出程序\n";
    cout << "============================================\n\n";
}

int main() {
    showHelp();

    while (true) {
        vector<int> numbers(4);

        cout << "请输入4个数字（用空格分隔，输入0 0 0 0退出）：\n";
        cout << "> ";

        for (int i = 0; i < 4; i++) {
            cin >> numbers[i];
        }

        // 检查是否退出
        if (numbers[0] == 0 && numbers[1] == 0 &&
            numbers[2] == 0 && numbers[3] == 0) {
            cout << "感谢使用！再见！\n";
            break;
        }

        // 验证输入
        bool valid = true;
        for (int num : numbers) {
            if (num < 1 || num > 13) {
                cout << "错误：数字必须在1-13之间！\n\n";
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        // 求解并显示结果
        showSolutions(numbers);
        cout << endl;
    }

    return 0;
}

