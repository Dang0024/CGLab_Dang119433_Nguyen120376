//
//  Node.cpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#include "node.hpp"

//just a comment

Node::Node(std::string name_, Node* parent_){
    name = name_;
    parent = parent_;
    worldTransform = glm::fmat4{};
}
Node* Node::getParent(){
    return parent;
}
void Node::setParent(Node* parent_){
    parent = parent_;
}
Node* Node::getChildren(std::string name){
    // loop through the list and remove children with this name
    for(std::size_t i = 0; i != children.size(); ++i)
    {
        Node* child = children[i];
        if (child->getName() == name){
            return child;
        }
    }
    return nullptr;
}
std::vector<Node*> Node::getChildrenList() {
    return children;
}
std::string Node::getName(){
    return name;
}
std::string  Node::getPath(){
    return path;
}
int Node::getDepth(){
    return depth;
}
glm::fmat4 Node::getLocalTransform(){
    return localTransform;
}
void Node::setLocalTransform(glm::fmat4 localtransf_){
    localTransform = localtransf_;
}
glm::fmat4 Node::getWorldTransform(){
    return worldTransform;
}
void Node::setWorldTransform(glm::fmat4 worldtrnasf_){
    worldTransform = worldtrnasf_;
}
void Node::addChildren(Node* child){
    children.push_back(child);
}
Node* Node::removeChildren(std::string  name){
    // loop through the list and remove children with this name
    for(std::size_t i = 0; i != children.size(); ++i)
    {
        Node* child = children[i];
        if (child->getName() == name){//delete children at poin i
            children.erase(children.begin()+static_cast<std::vector<double>::difference_type>(i));
            return child;
        }
    }
    return nullptr;
}
