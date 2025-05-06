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

#include "RadamsaMutatorBase.hpp"
#include "RuntimeException.hpp"
#include "VmfRand.hpp"

using std::string;

namespace vmf
{
/**
 * Binary Search Tree implementation
 */
class RadamsaTreeMutatorBase: public RadamsaMutatorBase
{
public:
    struct Node
    {
        size_t value;
        Node* left;
        Node* right;

        Node(size_t value) : value(value), left(nullptr), right(nullptr) {};
        ~Node() = default;
    };

    struct Tree
    {
    private:
        int findCloseBracketIndex(string treeStr, int i, int maxIndex) {
            if (i > maxIndex) {
                throw RuntimeException{"All open brackets must be paired with a close bracket", RuntimeException::CONFIGURATION_ERROR};
            }

            for(
                int j = i, numBracketPairs = 0; 
                j <= maxIndex; 
                ++j
            ) {
                if (treeStr[j] == '(') {
                    ++numBracketPairs;
                }
                else if(treeStr[j] == ')') {
                    --numBracketPairs;

                    if (numBracketPairs == 0) {
                        return j;
                    }
                }
            }

            throw RuntimeException{"All open brackets must be paired with a close bracket", RuntimeException::CONFIGURATION_ERROR};
        }

        // parses a Tree from a string in the form "root(left subtree)(right subtree)"
        Node* buildTree(string treeStr, int i, int maxIndex) {
            if (i > maxIndex) {
                return nullptr;
            }

            // grab current value
            string valueStr = "";
            while(i <= maxIndex && treeStr[i] >= '0' && treeStr[i] <= '9') {
                valueStr += treeStr[i];
                ++i;
            }
            size_t value = static_cast<size_t>(std::stoul(valueStr));


            Node* n = new Node(value);

            // if the current node should have at least one child...
            if(treeStr[i] == '(') {
                int result = findCloseBracketIndex(treeStr, i, maxIndex);

                n->left = buildTree(treeStr, i + 1, result - 1);
                n->right = buildTree(treeStr, result + 2, maxIndex - 1);
            }

            return n;
        }

        void deleteTree(Node* n) {
            if(n != nullptr) {
                deleteTree(n->left);
                deleteTree(n->right);
                delete n;
            }
        }

        Node* findByIndex(Node* n, size_t& indexToFind) {
            if(n == nullptr) {
                return nullptr;
            }

            // check left side
            Node* leftResult = findByIndex(n->left, indexToFind);
            if(leftResult != nullptr) {
                return leftResult;
            }

            // check yourself
            if(indexToFind == 0) {
                return n;
            }

            // check right side
            --indexToFind;
            return findByIndex(n->right, indexToFind);
        }
    
    public:
        Node* root;

        Tree() : root(nullptr) {};
        Tree(string treeStr) {
            root = buildTree(treeStr, 0, treeStr.length() - 1);
        }
        ~Tree() {deleteTree(root);};

        void toString(Node* n, string& s) {
            if(n == nullptr) {
                return;
            }

            s += std::to_string(n->value);

            // if there aren't any children...
            if(n->left == nullptr && n->right == nullptr) {
                return;
            }

            s.push_back('(');
            toString(n->left, s);
            s.push_back(')');

            // this check prevents extra brackets with nothing in them (not necessary for left side)
            if(n->right) {
                s.push_back('(');
                toString(n->right, s);
                s.push_back(')');
            }
        }

        size_t getSize(Node* n) {
            if(n == nullptr) {
                return 0u;
            }

            return 1 + getSize(n->left) + getSize(n->right);
        }

        // insert new node containing "value" into subtree "n"
        Node* insertNode(Node* n, size_t value) {
            // If we've reached the bottom, insert it here
            if(n == nullptr) {
                return new Node(value);
            }

            // Duplicates will go in right-hand side
            if(value < n->value) {
                n->left = insertNode(n->left, value);
            }
            else {
                n->right = insertNode(n->right, value);
            }

            // Return modified subtree
            return n;
        }

        // remove node containing "valueToFind" from subtree "n"
        Node* deleteNodeByValue(Node* n, size_t valueToFind) {
            // we've reached the bottom
            if(n == nullptr) {
                std::cout << "reached the bottom" << std::endl; //    TODO deleteme
                return nullptr;
            }

            std::cout << "n->value == " + std::to_string(n->value) << std::endl; //    TODO deleteme
            std::cout << "valueToFind == " + std::to_string(valueToFind) << std::endl; //    TODO deleteme

            // "value" is in left side
            if(n->value > valueToFind) {
                std::cout << "value is left side" << std::endl; //    TODO deleteme
                n->left = deleteNodeByValue(n->left, valueToFind);
            }
            // "value" is in right side
            else if(n->value < valueToFind) {
                std::cout << "value is right side" << std::endl; //    TODO deleteme
                n->right = deleteNodeByValue(n->right, valueToFind);
            }
            // current node has the value we're looking for
            else {
                std::cout << "found value!" << std::endl; //    TODO deleteme
                
                if(n->left == nullptr) {
                    std::cout << "left is null, returning right" << std::endl; //    TODO deleteme
                    return n->right;
                }

                if(n->right == nullptr) {
                    std::cout << "right is null, returning left" << std::endl; //    TODO deleteme
                    return n->left;
                }

                // two childen; swap current with in-order successor
                Node* successor = n->right;
                while(successor->left != nullptr) {
                    std::cout << "grabbing next left..." << std::endl; //    TODO deleteme
                    successor = successor->left;
                }
                std::cout << "found successor" << std::endl; //    TODO deleteme
                n->value = successor->value;
                n->right = deleteNodeByValue(n->right, successor->value);

            }

            std::cout << "made it to the bottom" << std::endl; //    TODO deleteme
            return n;
        }
    
        Node* deleteNodeByIndex(size_t indexToFind) {
            Node* nodeToDelete = findByIndex(root, indexToFind);

            if(nodeToDelete != nullptr) {
                root = deleteNodeByValue(root, nodeToDelete->value);
            }

            return root;
        }
    };

    RadamsaTreeMutatorBase() = default;
    virtual ~RadamsaTreeMutatorBase() = default;
};
}