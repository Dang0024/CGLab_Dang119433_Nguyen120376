#include "pointlight_node.hpp"

PointLightNode::PointLightNode(std::string name_, Node* parent_, float lightIntensity_, float lightColor_[3]): Node(name_, parent_)
{
    lightIntensity = lightIntensity_;
    lightColor[0] = lightColor_[0];
    lightColor[1] = lightColor_[1];
    lightColor[2] = lightColor_[2];
}
float PointLightNode::getLightIntensity(){
    return lightIntensity;
}
float* PointLightNode::getLightColor(){
    return lightColor;
}
