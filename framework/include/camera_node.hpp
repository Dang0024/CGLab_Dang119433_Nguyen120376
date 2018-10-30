//
//  CameraNode.hpp
//
//
//  Created by Yen Dang on 25/10/18.
//

#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <stdio.h>
#include "node.hpp"
#include <glm/glm.hpp>

class CameraNode : public Node{
private:
    bool isPerspective;
    bool isEnabled;
    glm::fmat4 projectionMatrix;
public:
    bool getPerspective();
    bool getEnabled();
    void setEnabled(bool enabl_);
    glm::fmat4 getProjectionMatrix();
    void setProjectionMatrix(glm::fmat4 matrix_);
};
#endif /* CameraNode_hpp */
