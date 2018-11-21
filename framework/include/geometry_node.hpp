//
//  GeometryNode.hpp


#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <stdio.h>
#include "node.hpp"
#include "model.hpp"
#include <iostream>

class GeometryNode : public Node{
private:
    model geometry;
    //declare parameters needed to render the planets
    float size;
    float speed;
    float dist;
    float color[3];
    bool print1 = true;
public:
    GeometryNode(std::string name_, Node* parent_, float size_, float speed_, float dist_, float color_[3]);
    model getGeometry();
    void setGeometry(model geo_);
    void setColor(float color_[3]);
    float getSize();
    float getSpeed();
    float getDist();
    float* getColor();
};


#endif /* GeometryNode_hpp */
