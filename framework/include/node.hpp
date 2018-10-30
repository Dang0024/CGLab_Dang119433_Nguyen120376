//
//  Node.hpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <stdio.h>

class Node {
private:
    Node* parent;
    std::vector<Node*> children;
    std::string name;
    std::string path;
    int depth;
    glm::fmat4 localTransform;
    glm::fmat4 worldTransform;
public:
    Node(std::string name_, Node* parent_);
    Node* getParent();
    void setParent(Node* parent_);
    Node* getChildren(std::string name);
    std::vector<Node*> getChildrenList();
    std::string getName();
    std::string  getPath();
    int getDepth();
    glm::fmat4 getLocalTransform();
    void setLocalTransform(glm::fmat4 localtransf_);
    glm::fmat4 getWorldTransform();
    void setWorldTransform(glm::fmat4 worldtrnasf_);
    void addChildren(Node* child);
    Node* removeChildren(std::string  name);
};

#endif /* Node_hpp */
