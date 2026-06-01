#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "ast.h"
class ASTVisualizer {
private:
    std::ofstream out;
    int nodeId = 0;
    std::unordered_map<ASTNode*, int> nodeMap;

    int getNodeId(ASTNode* node) {
        if (nodeMap.count(node)) return nodeMap[node];
        return nodeMap[node] = nodeId++;
    }

    // 根据节点类型获取颜色
    std::string getNodeColor(ASTNode* node) {
        if (dynamic_cast<ProgramNode*>(node)) return "#bbdefb";      // 程序根节点 - 浅蓝色
        if (dynamic_cast<FunctionNode*>(node)) return "#c8e6c9";     // 函数节点 - 浅绿色
        if (dynamic_cast<BlockNode*>(node)) return "#fff9c4";        // 代码块 - 浅黄色
        if (dynamic_cast<TypeNode*>(node)) return "#ffcc80";         // 类型节点 - 橙色
        if (dynamic_cast<NameNode*>(node)) return "#ffcc80";         // 名称节点 - 橙色
        if (dynamic_cast<IntNode*>(node)) return "#ffcc80";          // 整型常量 - 浅紫色
        if (dynamic_cast<DoubleNode*>(node)) return "#ffcc80";       // 浮点常量 - 浅紫色
        if (dynamic_cast<OpNode*>(node)) return "#ffcc80";           // 基本表达式 - 橙色
        if (dynamic_cast<ReturnNode*>(node)) return "#f8bbd0";       // return语句 - 粉色
        if (dynamic_cast<IfNode*>(node)) return "#c5e1a5";           // if语句 - 浅绿
        if (dynamic_cast<WhileNode*>(node)) return "#c5e1a5";        // while语句 - 浅绿
        return "#e0f7fa"; // 默认颜色 - 浅青色
    }

    // 生成节点标签文本
    std::string getNodeLabel(ASTNode* node) {
        std::string label = node->t.first;

        // 为特定节点添加额外信息
        if (auto nameNode = dynamic_cast<NameNode*>(node)) {
            label += ": " + nameNode->t.second->getInfo();
        }
        else if (auto typeNode = dynamic_cast<TypeNode*>(node)) {
            label += ": " + typeNode->t.second->getInfo();
        }
        else if (auto intNode = dynamic_cast<IntNode*>(node)) {
            label += ": " + intNode->t.second->getInfo();
        }
        else if (auto doubleNode = dynamic_cast<DoubleNode*>(node)) {
            label += ": " + doubleNode->t.second->getInfo();
        }
        else if (auto opNode = dynamic_cast<OpNode*>(node)) {
            label += ": " + opNode->t.second->getInfo();
        }

        return label;
    }

    void writeNode(ASTNode* node) {
        if (!node) return;
        int id = getNodeId(node);

        // 获取节点颜色和标签
        std::string color = getNodeColor(node);
        std::string label = getNodeLabel(node);

        // 写入带颜色的节点
        out << "  node" << id << " [label=\"" << label << "\", style=filled, fillcolor=\"" << color << "\"];\n";

        // 处理子节点 - 只处理非空子节点
        for (ASTNode* child : node->childs) {
            if (child) { // 只处理有效的子节点
                int childId = getNodeId(child);
                out << "  node" << id << " -> node" << childId << ";\n";
                writeNode(child); // 递归写子节点
            }
            // 忽略空指针子节点
        }
    }

public:
    void visualize(ASTNode* root, const std::string& filename = "ast1.dot") {
        out.open(filename);
        out << "digraph AST {\n";
        out << "  node [shape=box, style=\"rounded,filled\", fontname=\"Arial\"];\n";
        out << "  edge [arrowhead=vee];\n";
        out << "  rankdir=TB; // 从上到下的布局\n\n";

        writeNode(root);

        out << "}\n";
        out.close();

        std::cout << "AST 可视化已保存为: " << filename << "\n";
        std::cout << "使用 Graphviz 渲染: dot -Tpng " << filename << " -o ast.png\n";
    }
};