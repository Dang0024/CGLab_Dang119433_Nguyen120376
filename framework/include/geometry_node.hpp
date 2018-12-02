//
//  GeometryNode.hpp


#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <stdio.h>
#include "node.hpp"
#include "model.hpp"
#include <iostream>
#include "structs.hpp"

class GeometryNode : public Node{
private:
    model geometry;
    //declare parameters needed to render the planets
    float size;
    float speed;
    float dist;
    float color[3];
    bool print1 = true;
    int index;
public:
    GeometryNode(std::string name_, Node* parent_, float size_, float speed_, float dist_, float color_[3], int index_);
    model getGeometry();
    void setGeometry(model geo_);
    void setColor(float color_[3]);
    void setIndex(int index_);
    float getSize();
    float getSpeed();
    float getDist();
    float* getColor();
    int getIndex();
};


#endif /* GeometryNode_hpp */
