#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "ast.h" // 替换为你的 AST 节点头文件路径

class ASTVisualizer {
private:
    std::ofstream out;
    int nodeId = 0;
    std::unordered_map<ASTNode*, int> nodeMap;

    int getNodeId(ASTNode* node) {
        if (nodeMap.count(node)) return nodeMap[node];
        return nodeMap[node] = nodeId++;
    }

    void writeNode(ASTNode* node) {
        if (!node) return;
        int id = getNodeId(node);
        out << "  node" << id << " [label=\"" << node->t << "\"];\n";
        for (ASTNode* child : node->childs ) {
            if (!child) continue;
            int childId = getNodeId(child);
            out << "  node" << id << " -> node" << childId << ";\n";
            writeNode(child); // 递归写子节点
        }
    }

public:
    void visualize(ASTNode* root, const std::string& filename = "ast.dot") {
        out.open(filename);
        out << "digraph AST {\n";
        out << "  node [shape=box];\n";
        writeNode(root);
        out << "}\n";
        out.close();
    }
};