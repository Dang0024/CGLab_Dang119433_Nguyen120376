#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"

#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_graph.hpp"

#include "model.hpp"
#include "structs.hpp"
#include <iostream>
#include <string>
#include <vector>

// gpu representation of model


struct planet {
    std:: string name;
    std:: string parent;
    float size;
    float speed;
    float dist;
};


class ApplicationSolar : public Application {
public:
    // allocate and initialize objects
    ApplicationSolar(std::string const& resource_path);
    // free allocated objects
    ~ApplicationSolar();

    SceneGraph* scene_graph = SceneGraph::getInstance();
    Node* root = scene_graph->getRoot();
    GeometryNode* root_child;

    planet planets[10] = {
        {"sun", "root", 0.75, 0, 0},
        {"mercury", "sun", 0.1, 0.02, 1},
        {"venus", "sun", 0.2, 0.04, 2},
        {"earth", "sun", 0.25, 0.06, 3},
        {"moon", "earth", 0.18, -0.8, 1.5},
        {"mars", "sun", 0.15, 0.08, 4},
        {"jupiter", "sun", 0.45, 0.2, 7},
        {"saturn", "sun", 0.4, 0.2, 9},
        {"uranus", "sun", 0.35, 0.15, 10},
        {"neptune", "sun", 0.3, 0.1, 12}
    };

    // react to key input
    void keyCallback(int key, int action, int mods);
    //handle delta mouse movement input
    void mouseCallback(double pos_x, double pos_y);
    //handle resizing
    void resizeCallback(unsigned width, unsigned height);
    void render() const;

    // ass_1: initialize all planets
    void initPlanets();
    GeometryNode* addPlanet(planet a_planet, Node* parent);

    // ass_1: draw all planets
    void drawPlanets(GeometryNode* a_planet) const;
    void addPlanetTransform(GeometryNode* a_planet) const;
    void bindAndDrawPlanet(glm::fmat4 model_matrix)const;

    // ass_2: draw stars
    void drawStars() const;


protected:
    // draw all objects

    void initializeShaderPrograms();
    void initializeGeometryForPlanets();
    // ass_2: initialise star
    void initializeGeometryForStars();
    // update uniform values
    void uploadUniforms();
    // upload projection matrix
    void uploadProjection();
    // upload view matrix
    void uploadView();

    // cpu representation of model
    model_object planet_object;
    model_object star_object;

    // camera transform matrix
    glm::fmat4 m_view_transform;
    // camera projection matrix
    glm::fmat4 m_view_projection;
};

#endif
