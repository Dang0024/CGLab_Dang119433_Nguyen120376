#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"

#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_graph.hpp"
#include "pointlight_node.hpp"

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
    float color[3];
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
    PointLightNode* point_light_node; //ass_3: task 1

    planet planets[10] = {
        {"sun", "root", 0.75, 0, 0, { 0.96f, 0.56f, 0.22f }},
        {"mercury", "sun", 0.1, 0.02, 1.2, { 0.35f, 0.33f, 0.3f }},
        {"venus", "sun", 0.2, 0.04, 2, { 0.94f, 0.9f, 0.55f }},
        {"earth", "sun", 0.25, 0.06, 3, { 0.0f, 0.32f, 0.65f }},
        {"moon", "earth", 0.18, -0.8, 1.5, { 1.0f, 0.99f, 0.84f }},
        {"mars", "sun", 0.15, 0.08, 4, { 0.5f, 0.0f, 0.0f }},
        {"jupiter", "sun", 0.45, 0.2, 7, { 0.9f, 0.58f, 0.0f }},
        {"saturn", "sun", 0.4, 0.2, 9, { 0.9f, 0.75f, 0.54f }},
        {"uranus", "sun", 0.35, 0.15, 10, { 0.69f, 0.893f, 0.93f }},
        {"neptune", "sun", 0.3, 0.1, 12, {0.69f, 0.81f, 0.93f }}
    };

    planet sky = {"sky", "root", 30, 0, 0, {0.0f, 0.0f, 0.0f}};

    // react to key input
    void keyCallback(int key, int action, int mods);
    //handle delta mouse movement input
    void mouseCallback(double pos_x, double pos_y);
    //handle resizing
    void resizeCallback(unsigned width, unsigned height);
    void render() const;

    // ass_1: initialize all planets
    void initPlanets();
    GeometryNode* addPlanet(planet a_planet, Node* parent, int i);

    // ass_1: draw all planets
    void drawPlanets(GeometryNode* a_planet) const;
    void addPlanetTransform(GeometryNode* a_planet) const;
    void bindAndDrawPlanet(glm::fmat4 model_matrix)const;

    // ass_2: draw stars
    void drawStars() const;

    // ass_4: sky sphere
    void createSky() const;

protected:
    // draw all objects

    void initializeShaderPrograms();
    void initializeGeometryForPlanets();
    // ass_2: initialise star
    void initializeGeometryForStars();
    void initializeTextures();
    // update uniform values
    void uploadUniforms();
    // upload projection matrix
    void uploadProjection();
    // upload view matrix
    void uploadView();

    // cpu representation of model
    model_object planet_object;
    model_object star_object;
    texture_object text_objects[11];

    // camera transform matrix
    glm::fmat4 m_view_transform;
    // camera projection matrix
    glm::fmat4 m_view_projection;
};

#endif
