//
//  CameraNode.cpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#include "camera_node.hpp"

bool CameraNode::getPerspective(){
    return isPerspective;
}
bool CameraNode::getEnabled(){
    return isEnabled;
}
void CameraNode::setEnabled(bool enabl_){
    isEnabled = enabl_;
}
glm::fmat4 CameraNode::getProjectionMatrix(){
    return projectionMatrix;
}
void CameraNode::setProjectionMatrix(glm::fmat4 matrix_){
    projectionMatrix = matrix_;
}
