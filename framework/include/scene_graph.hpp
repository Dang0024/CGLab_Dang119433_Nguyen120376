//
//  SceneGraph.hpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <stdio.h>
#include "node.hpp"

class SceneGraph {

static SceneGraph* _instance; //one instance

private:
    std::string name;
    Node* root;
    void setName(std::string name_);
    void setRoot(Node* root_);
public:
    static SceneGraph* getInstance();
    std::string getName();
    Node* getRoot();
};


#endif /* SceneGraph_hpp */
