#ifndef POINTLIGHT_NODE_HPP
#define POINTLIGHT_NODE_HPP

#include <stdio.h>
#include "node.hpp"
#include "model.hpp"
#include <iostream>

class PointLightNode : public Node{
private:
    float lightIntensity;
    float lightColor[3];
public:
    PointLightNode(std::string name_, Node* parent_, float lightIntensity_, float lightColor_[3]);
    float getLightIntensity();
    float* getLightColor();

};

#endif
