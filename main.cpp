#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


struct TreeNode {
    int uid;
    TreeNode* parent;
    std::vector<TreeNode*> children;

    TreeNode(int u, TreeNode* p) : uid(u), parent(p){}
};

/**
 * Returns node using recursive DFS search.
 * @return pointer to found node if node found 
*/
TreeNode* findNode(TreeNode* node, int uid) {
    if (!node) {
        return nullptr;
    }

    if (node->uid == uid) {
        return node;
    }

    TreeNode* foundNode = nullptr;
    for (TreeNode* child : node->children) {
        foundNode = findNode(child, uid);
        if (foundNode) {
            return foundNode;
        }
    }

    return nullptr;
}

void addChild(TreeNode* parent, TreeNode* child) {
    if (parent && child) {
        parent->children.push_back(child);
    }
}
/**
 * Iteratively populates tree from data read from inputfile
 * TODO: advandced error handling when input tree is broken.
*/
TreeNode* createTree(std::ifstream& inputFile) {
    int N;
    inputFile >> N;

    TreeNode* root = nullptr;

    for (int i = 0; i < N; ++i) {
        int uid, parentUid, type;
        inputFile >> uid >> parentUid >> type;

        TreeNode* node = new TreeNode(uid, nullptr);

        if (!root) {
            root = node;
        } else {
            TreeNode* parent = findNode(root, parentUid);
            addChild(parent, node);
            node->parent = parent;
        }
    }

    return root;
}
/**
 * Recursively deletes tree and frees up memory.
*/
void deleteTree(TreeNode* node) {
    if (!node) {
        return;
    }

    for (TreeNode* child : node->children) {
        deleteTree(child);
    }

    delete node;
}

bool isValueInVector(const std::vector<int>& vector, int value) {
    return std::find(vector.begin(), vector.end(), value) != vector.end();
}
/**
 * DFS algorithm finding shortest path between 2 leaf nodes.
 * If no path found, returns empty vector
*/
void dfs(TreeNode* currentNode, int endUid, std::vector<int>& visited, std::vector<int>& shortestPath, int startUid, bool* found) 
{
    visited.push_back(currentNode->uid);

    if (currentNode->uid == endUid) 
    {
        *found = true;
    }
    
    // Traverse the children of the current node if target not found
    for (TreeNode* child : currentNode->children) {
        if (!*found && (!isValueInVector(visited, child->uid)) ) {
            dfs(child, endUid, visited, shortestPath, startUid, found);
        }
    }
    //if children are traversed, but no target found and node has parent, visit parent
    if ( (currentNode->parent) && (!isValueInVector(visited, currentNode->parent->uid)) && !*found )
    {
        dfs(currentNode->parent, endUid, visited, shortestPath, startUid, found);
    }
    
    // If the shortest path is found, backtrack from the endUid to the startUid
    if(*found == true)
    {
        shortestPath.push_back(currentNode->uid);
        return;
    }
}
/**
 * Wrapper for finding shortest path between 2 leaf nodes.
 * @return vector of node id's from destination to source.
 * TODO: better errror handling when nodes are missing
*/
std::vector<int> findShortestPath(TreeNode* root, int startUid, int endUid) {
    // Create a vector to store visited nodes
    std::vector<int> visited;
    TreeNode* startNode = findNode(root, startUid);
    bool found = false;
    // Declare the shortestPath vector
    std::vector<int> shortestPath;
    std::cout << "source " << startUid << " target " << endUid << std::endl;
    // Perform DFS starting from the root
    dfs(startNode, endUid, visited, shortestPath, startUid, &found);

    return shortestPath;
}
/**
 * Main function:
 * Opens file, calls out functions to populate tree.
 * Iteratively reads test cases from file and runs dfs algorithms to find paths.
 * Prints out found paths to console.
 * Closes file, destroys tree and frees up memory.
 * TODO: refactor test reading part into separate function
*/
int main() {
    std::ifstream inputFile("input2.txt");
    if (!inputFile) {
        std::cout << "Failed to open the input file." << std::endl;
        return 1;
    }

    TreeNode* root = createTree(inputFile);
    int K = 0;
    inputFile >> K;
    std::cout << "there are " << K << " test cases:" << std::endl;
    
    for (size_t i = 0; i < K; i++)
    {
        int start, end;
        inputFile >> start >> end;
        std::vector<int> shortestPath = findShortestPath(root, start, end);
        for (auto it = shortestPath.rbegin(); it != shortestPath.rend(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    
    inputFile.close();

    // Clean up the tree
    deleteTree(root);

    return 0;
}
