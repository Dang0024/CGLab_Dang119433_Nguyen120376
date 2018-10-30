//
//  SceneGraph.cpp
//
//
//  Created by Yen Dang on 25/10/18.
//
#include "scene_graph.hpp"

void SceneGraph::setName(std::string name_){
    name = name_;
}
void SceneGraph::setRoot(Node* root_){
    root = root_;
}

SceneGraph* SceneGraph::getInstance(){  //return instance
    if(_instance==nullptr) //check if this instance is already there or not
        _instance = new SceneGraph();
        _instance->setName("scene_graph");
        _instance->setRoot(new Node("root",nullptr));
    return _instance;
}
std::string SceneGraph::getName(){
    return(name);
}
Node* SceneGraph::getRoot(){
    return(root);
}

SceneGraph* SceneGraph::_instance;
