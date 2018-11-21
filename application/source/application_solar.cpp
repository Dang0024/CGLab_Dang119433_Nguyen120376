#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

const int count_stars = 5000;	// ass_2: pre-define the number of stars

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
    :Application{resource_path}
    ,planet_object{} // ass_1
    ,star_object{}  // ass_2
    ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
    ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
    initializeGeometryForPlanets();
    initializeGeometryForStars(); // ass_2
    initializeShaderPrograms();
    initPlanets(); // ass_1: create scene graph

    // ass_3: add PointLightNode as child of root
    float light_color[3] = {1.0f, 1.0f, 1.0f};
    point_light_node = new PointLightNode("point_light", root, 1.0, light_color);
    root->addChildren(point_light_node);
}

ApplicationSolar::~ApplicationSolar() {
    glDeleteBuffers(1, &planet_object.vertex_BO);
    glDeleteBuffers(1, &planet_object.element_BO);
    glDeleteVertexArrays(1, &planet_object.vertex_AO);
    // ass_2
    glDeleteBuffers(1, &star_object.vertex_BO);
    glDeleteBuffers(1, &star_object.element_BO);
    glDeleteVertexArrays(1, &star_object.vertex_AO);
}

void ApplicationSolar::render() const {
    // ass_3: set background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.12f, 0.19f, 0.13f, 1.0f);

    drawPlanets(root_child);
    drawStars();
}

// ass_1: initialize all planets
void ApplicationSolar::initPlanets(){

    // find sun and call addPlanet() to create scene graph hierarchical structure
    for(int i=0; i<10; i++){
        planet a_planet = planets[i];
        if(a_planet.parent == "root"){
            printf("init child %s ... \n", a_planet.name.c_str());
            root_child = addPlanet(a_planet, root);
            break;
        }
    }

}

GeometryNode* ApplicationSolar::addPlanet(planet a_planet, Node* parent){

    GeometryNode* a_node = new GeometryNode(a_planet.name,
                                            parent,
                                            float(a_planet.size),
                                            float(a_planet.speed),
                                            float(a_planet.dist),
                                            a_planet.color);

    parent->addChildren(a_node);
    // recursive loop to assign children of the node
    for(int i=0; i<10; i++){
        planet next_planet = planets[i];
        if(next_planet.parent == a_planet.name){
            printf("init child %s ... \n", next_planet.name.c_str());
            addPlanet(next_planet, a_node);
        }
    }

    return a_node;
}

// ass_1: draw all planets
void ApplicationSolar::drawPlanets(GeometryNode* a_planet) const{
    float* color = a_planet->getColor();
    // bind shader to upload unifomr
    glUseProgram(m_shaders.at("planet").handle);

    // ass_3: set diffuse color for shader
    glUniform3f(m_shaders.at("planet").u_locs.at("diffuseColor"), *(color + 0), *(color + 1), *(color + 2));

    addPlanetTransform(a_planet);
    std::vector<Node*> children = a_planet->getChildrenList();
    for(size_t i = 0; i != children.size(); ++i)
    {
        Node* child = children[i];
        GeometryNode* geometry_child = static_cast<GeometryNode*>(child);
        if(geometry_child!=nullptr){
            drawPlanets(geometry_child);
        }
    }
}

void ApplicationSolar::addPlanetTransform(GeometryNode* a_planet) const{

    std::string name = a_planet->getName();
    float size = a_planet->getSize();
    float speed = a_planet->getSpeed();
    float dist = a_planet->getDist();

    glm::fmat4 model_matrix;

    glm::fmat4 parent_model_matrix = a_planet->getParent()->getWorldTransform();

    model_matrix = glm::rotate(parent_model_matrix, float(glfwGetTime() * speed), glm::fvec3{ 0.0f, 1.0f, 0.0f });
    model_matrix = glm::translate(model_matrix, glm::fvec3{ 0.0f, 0.0f, dist });
    model_matrix = glm::scale(model_matrix, glm::fvec3{ size, size, size });
    a_planet->setWorldTransform(model_matrix);

    bindAndDrawPlanet(model_matrix);
}

void ApplicationSolar::bindAndDrawPlanet(glm::fmat4 model_matrix) const{
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));


    // extra matrix for normal transformation to keep them orthogonal to surface
    /* glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));
    */
    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}

// ass2 draw the star
void ApplicationSolar::drawStars() const{
    // printf("render star\n");
    // bind star shader
    glUseProgram(m_shaders.at("star").handle);

    // bind the VAO to draw
    glBindVertexArray(star_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawArrays(star_object.draw_mode, 0, count_stars);

    glEnable(GL_PROGRAM_POINT_SIZE);
    // create random size for star to create twinkle effect
    glPointSize(((float)rand() / (float)RAND_MAX)*1.25+ 1.5);
}

void ApplicationSolar::uploadView() {

    printf("upload view planet\n");
    // vertices are transformed in camera space, so camera transform must be inverted
    glm::fmat4 view_matrix = glm::inverse(m_view_transform);

    // bind planet shader program
    glUseProgram(m_shaders.at("planet").handle);
    // upload matrix to gpu
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                       1, GL_FALSE, glm::value_ptr(view_matrix));


    printf("upload view star\n");
    // ass2_bind shader & upload ViewMatrix for stars
    glUseProgram(m_shaders.at("star").handle);
    glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ViewMatrix"),
                       1, GL_FALSE, glm::value_ptr(view_matrix));

}

void ApplicationSolar::uploadProjection() {

    // upload matrix to gpu
    printf("upload projection planet\n");
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));


    printf("upload projection star\n");
    // ass2_bind shader & upload ProjectionMatrix for stars
    glUseProgram(m_shaders.at("star").handle);
    glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));

}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
    // bind planet shader program
    glUseProgram(m_shaders.at("planet").handle);
    // ass_3: setting the light source
    glUniform3f(m_shaders.at("planet").u_locs.at("lightSource"), 0.0f, 0.0f, 0.0f);
    glUniform1i(m_shaders.at("planet").u_locs.at("shaderSwitch"), 1);
    glUniform1f(m_shaders.at("planet").u_locs.at("lightIntensity"), point_light_node->getLightIntensity());
    float* lightColor = point_light_node->getLightColor();
    glUniform3f(m_shaders.at("planet").u_locs.at("lightColor"), *(lightColor + 0), *(lightColor + 1), *(lightColor + 2));
    uploadView();
    uploadProjection();
}

// intialisation functions -------------------------------------------
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
    // store shader program objects in container
    m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                                {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    // request uniform locations for shader program
    // m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

    // ass_3: init variables for shading
    m_shaders.at("planet").u_locs["lightSource"] = -1;
    m_shaders.at("planet").u_locs["lightIntensity"] = -1;
    m_shaders.at("planet").u_locs["lightColor"] = -1;
    m_shaders.at("planet").u_locs["diffuseColor"] = -1;
    m_shaders.at("planet").u_locs["shaderSwitch"] = -1;

    // ass2_store star shader program objects in container
    m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/star.vert"},
                                              {GL_FRAGMENT_SHADER, m_resource_path + "shaders/star.frag"}}});
    // ass2_request uniform locations for star shader program
    m_shaders.at("star").u_locs["ViewMatrix"] = -1;
    m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

}

// load models
void ApplicationSolar::initializeGeometryForPlanets() {
    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

    // generate vertex array object
    glGenVertexArrays(1, &planet_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(planet_object.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &planet_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

    // generate generic buffer
    glGenBuffers(1, &planet_object.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    planet_object.draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object
    planet_object.num_elements = GLsizei(planet_model.indices.size());

}
// ass_2 initialize stars
void ApplicationSolar::initializeGeometryForStars() {
    // container of floats for star positions and colors
    GLfloat starVertexBuffer[2 * 3 * count_stars];

    int index = 0;
    // create random values for each star
    for (int i = 0; i < count_stars; i++) {

        //3 random position values for x, y, z
        for (int j = 0; j < 3; j++) {
            // position value from -15 to 15
            starVertexBuffer[index] = ((float)rand() / (float)RAND_MAX) * 40 - 15 ;
            index++;
        }
        //3 random color values for r, g, b
        for (int k = 0; k < 3; k++) {
            // color vale from 0.5 to 1.0 for bright colors
            starVertexBuffer[index] = ((float)rand() / (float)RAND_MAX) / 2.0 + 0.5;
            index++;

        }
    }


    // generate vertex array object
    glGenVertexArrays(1, &star_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(star_object.vertex_AO);


    // generate generic buffer
    glGenBuffers(1, &star_object.vertex_BO);
    // bind this as an vertex array buffer containing position and color
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(starVertexBuffer), starVertexBuffer, GL_STATIC_DRAW);


    // activate position on gpu
    glEnableVertexAttribArray(0);
    // position data (3 floats) with offset of 0 & gap of 6 (3 for position & 3 for color)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));
    // activate color on gpu
    glEnableVertexAttribArray(1);
    // color data (3 floats) with offset of 3 & gap of 6 (3 for position & 3 for color)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));

    // generate generic buffer
    glGenBuffers(1, &star_object.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(starVertexBuffer), starVertexBuffer, GL_STATIC_DRAW);


    // store type of primitive to draw
    star_object.draw_mode = GL_POINTS;
    // transfer number of indices to model object
    star_object.num_elements = GLsizei(count_stars);
}

// callback functions for window events -----------------------------------
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
    if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
        uploadView();
    }
    else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
        uploadView();
    }
    //ass_3: additional task: input to switch shader mode
    else if (key == GLFW_KEY_1) {
        glUseProgram(m_shaders.at("planet").handle);
        // ass_3: set shaderSwitch for shader
        glUniform1i(m_shaders.at("planet").u_locs.at("shaderSwitch"),1);
        uploadView();
    }
    else if (key == GLFW_KEY_2) {
        glUseProgram(m_shaders.at("planet").handle);
        // ass_3: set shaderSwitch for shader
        glUniform1i(m_shaders.at("planet").u_locs.at("shaderSwitch"),2);
        uploadView();
    }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
    // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
    // recalculate projection matrix for new aspect ration
    m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
    // upload new projection matrix
    uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
    Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
