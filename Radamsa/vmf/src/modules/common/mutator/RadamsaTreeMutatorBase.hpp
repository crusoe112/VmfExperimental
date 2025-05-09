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
            // throw std::runtime_error("top of buildTree");      //      TODO deleteme
            if(treeStr.empty()) {
                throw RuntimeException{"Tree string is empty", RuntimeException::UNEXPECTED_ERROR};
            }

            std::stack<Node*> stk;
            string value;
            Node* r = nullptr;

            for(size_t i = 0; i < treeStr.length(); ++i) {
                char ch = treeStr[i];

                if(isalnum(ch)) {
                    value += ch;
                }
                else if(ch == '(') {
                    if(value.empty()) {
                        throw RuntimeException{"Root declared without a value", RuntimeException::UNEXPECTED_ERROR};
                    }

                    Node* n;
                    if(stk.empty()) {   // if stack is empty, then this value should be root
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
                    throw RuntimeException{"Unexpected character in tree string", RuntimeException::UNEXPECTED_ERROR};
                }
            }

            if(!stk.empty()) {
                throw RuntimeException{"Unmatched open bracket in tree string", RuntimeException::UNEXPECTED_ERROR};
            }

            // throw std::runtime_error("before single node tree check");      //      TODO deleteme

            // case for TreeStr consisting of a single root node
            if(!value.empty() && root == nullptr) {
                this->root = new Node(value);
                throw std::runtime_error("root created");      //      TODO deleteme
            }

            // throw std::runtime_error("after single node tree check");      //      TODO deleteme

            return;
        }

        void deleteNode(Node* n) {
            if(n == nullptr) {
                // throw std::runtime_error("node is null, returning");      //      TODO deleteme
                return;
            }

            throw std::runtime_error("before deleting children");      //      TODO deleteme
            for(Node* child : n->children) {
                // throw std::runtime_error("before deleting a child");      //      TODO deleteme
                deleteNode(child);
                // throw std::runtime_error("after deleting a child");      //      TODO deleteme
            }
            
            
            // throw std::runtime_error("before deleting node");      //      TODO deleteme
            delete n;
        }
    
    public:
        Node* root;

        Tree() : root(nullptr) {};

        Tree(string treeStr) {
            buildTree(treeStr);
            // throw std::runtime_error("after buildTree");      //      TODO deleteme
        }
        
        // Move constructor
        Tree(Tree&& other) noexcept {
            throw std::runtime_error("top of move constructor");      //      TODO deleteme
            root = other.root;
            other.root = nullptr;
        }

        // Move assignment operator
        Tree& operator=(Tree&& other) noexcept {
            // throw std::runtime_error("top of move assignment");      //      TODO deleteme
            if(this != &other) {    // prevents self-assignment
                deleteNode(root);
                root = other.root;
                other.root = nullptr;
            }

            // throw std::runtime_error("bottom of move assignment");      //      TODO deleteme
            return *this;
        }

        ~Tree() {
            // throw std::runtime_error("top of destructor");      //      TODO deleteme
            deleteNode(this->root);
        };

        string toString(Node* n) {
            if(n == nullptr) {
                return "";
            }

            // throw std::runtime_error(n->value);      //      TODO deleteme
            string treeStr(n->value);
            // throw std::runtime_error("after");  //          TODO deleteme

            if(!n->children.empty()) {
                treeStr += "(";
                for(Node* child : n->children) {
                    treeStr += toString(child);
                }
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