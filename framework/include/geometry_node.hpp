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
public:
    GeometryNode(std::string name_, Node* parent_, float size_, float speed_, float dist_);
    model getGeometry();
    void setGeometry(model geo_);
    float getSize();
    float getSpeed();
    float getDist();
};


#endif /* GeometryNode_hpp */
