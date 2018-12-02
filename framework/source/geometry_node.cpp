//
//  GeometryNode.cpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#include "geometry_node.hpp"

GeometryNode::GeometryNode(std::string name_, Node* parent_, float size_, float speed_, float dist_, float color_[3], int index_): Node(name_, parent_){
    size = size_;
    speed = speed_;
    dist = dist_;
    color[0] = color_[0];
    color[1] = color_[1];
    color[2] = color_[2];
    index = index_;
}
model GeometryNode::getGeometry(){
    return geometry;
}
void GeometryNode::setGeometry(model geo_){
    geometry = geo_;
}
void GeometryNode::setColor(float color_[3]){
    color[0] = color_[0];
    color[1] = color_[1];
    color[2] = color_[2];
}
void GeometryNode::setIndex(int index_){
    index = index_;
}
float GeometryNode::getSize(){
    return size;
}
float GeometryNode::getSpeed(){
    return speed;
}
float GeometryNode::getDist(){
    return dist;
}
float* GeometryNode::getColor(){
    return color;
}
int GeometryNode::getIndex(){
    return index;
}
