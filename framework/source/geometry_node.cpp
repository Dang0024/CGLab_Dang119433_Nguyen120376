//
//  GeometryNode.cpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#include "geometry_node.hpp"

GeometryNode::GeometryNode(std::string name_, Node* parent_, float size_, float speed_, float dist_): Node(name_, parent_){
    size = size_;
    speed = speed_;
    dist = dist_;
}
model GeometryNode::getGeometry(){
    return geometry;
}
void GeometryNode::setGeometry(model geo_){
    geometry = geo_;
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
