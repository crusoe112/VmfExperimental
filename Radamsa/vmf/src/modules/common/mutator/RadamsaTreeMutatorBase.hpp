/* =============================================================================
 * Vader Modular Fuzzer
 * Copyright (c) 2021-2023 The Charles Stark Draper Laboratory, Inc.
 * <vmf@draper.com>
 *
 * Effort sponsored by the U.S. Government under Other Transaction number
 * W9124P-19-9-0001 between AMTC and the Government. The U.S. Government
 * is authorized to reproduce and distribute reprints for Governmental purposes
 * notwithstanding any copyright notation thereon.
 *
 * The views and conclusions contained herein are those of the authors and
 * should not be interpreted as necessarily representing the official policies
 * or endorsements, either expressed or implied, of the U.S. Government.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later>
 * ===========================================================================*/

#pragma once

#include <stack>
#include "RadamsaMutatorBase.hpp"
#include "RuntimeException.hpp"
#include "VmfRand.hpp"

using std::string;

namespace vmf
{
/**
 * 
 */
class RadamsaTreeMutatorBase: public RadamsaMutatorBase
{
public:
    RadamsaTreeMutatorBase() = default;
    virtual ~RadamsaTreeMutatorBase() = default;

    struct Node
    {
        string value;
        Node* parent;
        std::vector<Node*> children;

        Node(string v, Node* p = nullptr) : value(v), parent(p) {};
        ~Node() = default;
    };

    struct Tree
    {
    private:
        // parses a Tree from a string in the form ""A(B(C)(D))(E)""
        void buildTree(string treeStr) {
            if(treeStr.empty()) {
                throw RuntimeException{"Tree string is empty", RuntimeException::UNEXPECTED_ERROR};
            }

            std::stack<Node*> stk;
            string value;
            for(size_t i = 0; i < treeStr.length(); ++i) {
                char ch = treeStr[i];

                if(std::isspace(ch)) {
                    continue;
                }
                else if(ch == '(') {
                    if(!value.empty()) {
                        Node* n;
                        if(stk.empty()) {
                            n = new Node(value, nullptr);
                            this->root = n;
                        }
                        else {
                            n = new Node(value, stk.top());
                            stk.top()->children.push_back(n);
                        }

                        stk.push(n);
                        value.clear();
                    }
                    else {
                        Node* toPush = nullptr;
                        // upcomming additional child, re-push root
                        if(stk.empty() && this->root != nullptr) {
                            toPush = this->root;
                        }
                        // upcomming additional child, re-push the most recently popped node
                        else if(!stk.empty() && !stk.top()->children.empty()) {
                            toPush = stk.top()->children.back();
                        }

                        if(toPush != nullptr) {
                            stk.push(toPush);
                        }
                        else {
                            throw RuntimeException{"Unexpected open bracket without a parent node", RuntimeException::UNEXPECTED_ERROR};
                        }
                    }
                }
                else if(ch == ')') {
                    if(stk.empty()) {
                        throw RuntimeException{"Unmatched open bracket in tree string", RuntimeException::UNEXPECTED_ERROR};
                    }

                    if(!value.empty()) {
                        Node* n = new Node(value, stk.top());
                        stk.top()->children.push_back(n);

                        value.clear();
                    }

                    stk.pop();
                }
                else {
                    value += ch;
                }
            }

            if(!stk.empty()) {
                throw RuntimeException{"Unmatched open bracket in tree string", RuntimeException::UNEXPECTED_ERROR};
            }


            // case for TreeStr consisting of a single root node
            if(!value.empty() && this->root == nullptr) {
                this->root = new Node(value);
            }

            return;
        }

        void deleteNode(Node* n) {
            if(n == nullptr) {
                return;
            }

            for(Node* child : n->children) {
                deleteNode(child);
            }

            // erase self from parent's children
            if(n->parent != nullptr) {
                auto& parentChildren = n->parent->children;
                auto it = std::find(parentChildren.begin(), parentChildren.end(), n);
                if(it != parentChildren.end()) {
                    parentChildren.erase(it);
                }
            }

            delete n;
        }
    
    public:
        Node* root = nullptr;

        Tree() {};
        Tree(string treeStr) {
            buildTree(treeStr);
        }
        
        // deleting copy constructor and copy assignment to avoid shallow copies
        Tree(const Tree&) = delete;
        Tree& operator=(const Tree&) = delete;

        // Move constructor
        Tree(Tree&& other) noexcept {
            root = other.root;
            other.root = nullptr;
        }

        // Move assignment operator
        Tree& operator=(Tree&& other) noexcept {
            if(this != &other) {    // prevents self-assignment
                deleteNode(root);

                root = other.root;
                other.root = nullptr;
            }
            return *this;
        }

        ~Tree() {
            deleteNode(this->root);
        };

        string toString(Node* n) {
            if(n == nullptr) {
                return "";
            }

            string treeStr(n->value);

            for(Node* child : n->children) {
                treeStr += "(";
                treeStr += toString(child);
                treeStr += ")";
            }

            return treeStr;
        }

        size_t countNodes(Node* n) {
            if(n == nullptr) {
                return 0u;
            }

            size_t count = 1; // count n itself
            for(Node* child : n->children) {
                count += countNodes(child);
            }

            return count;
        }
    
        void deleteNodeByIndex(size_t indexToDelete) {
            if(this->root == nullptr) {
                throw RuntimeException{"The tree must have at least one node", RuntimeException::USAGE_ERROR};
            }

            // collect all the nodes
            std::vector<Node*> nodes;
            std::stack<Node*> stk;
            
            stk.push(this->root);
            while(!stk.empty()) {
                Node* curr = stk.top();
                stk.pop();
                nodes.push_back(curr);

                for(Node* child : curr->children) {
                    stk.push(child);
                }
            }

            if(indexToDelete < 0 || indexToDelete >= nodes.size()) {
                throw RuntimeException{"The index to delete is out of bounds", RuntimeException::INDEX_OUT_OF_RANGE};
            }

            if(indexToDelete == 0) {
                this->root = nullptr;
            }
            Node* nodeToDelete = nodes[indexToDelete];
            deleteNode(nodeToDelete);
        }
    };
};
}