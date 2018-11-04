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

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
  init_planet();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


void ApplicationSolar::render() const {
  // bind shader to upload uniforms

//  printf ("render...\n\n\n\n");
  glUseProgram(m_shaders.at("planet").handle);


  add_planet(root_child);

}

void ApplicationSolar::init_planet(){

    printf ("init planet...\n");

    for(int i=0; i<10; i++){
        planet a_planet = planets[i];
        if(a_planet.parent == "root"){
            printf("init child %s ... \n", a_planet.name.c_str());
            root_child = init_child(a_planet, root);
            break;
        }
    }

}

GeometryNode* ApplicationSolar::init_child(planet a_planet, Node* parent){
    //create sun geo
    GeometryNode* a_node = new GeometryNode(a_planet.name,
                                  parent,
                                  float(a_planet.size),
                                  float(a_planet.speed),
                                  float(a_planet.dist));

    //add sun geo to root children list
    parent->addChildren(a_node);

    for(int i=0; i<10; i++){
        planet next_planet = planets[i];
        if(next_planet.parent == a_planet.name){
            printf("init child %s ... \n", next_planet.name.c_str());
            init_child(next_planet, a_node);
        }
    }

    return a_node;
}

void ApplicationSolar::add_planet(GeometryNode* a_planet) const{

    add_planet_transform(a_planet);
    std::vector<Node*> children = a_planet->getChildrenList();
    for(size_t i = 0; i != children.size(); ++i)
    {
        Node* child = children[i];
        GeometryNode* geometry_child = static_cast<GeometryNode*>(child);
        if(geometry_child!=nullptr){
            add_planet(geometry_child);
        }
    }
}

void ApplicationSolar::add_planet_transform(GeometryNode* a_planet) const{

    std::string name = a_planet->getName();
    float size = a_planet->getSize();
    float speed = a_planet->getSpeed();
    float dist = a_planet->getDist();

//    printf("\n\n\n");
//    printf("rendering %s...\n", name.c_str());
//    printf("parent: %f \n", a_planet->getParent()->getName().c_str());
//    printf("size: %f \n", size);
//    printf("speed: %f \n", speed);
//    printf("dist: %f \n", dist);

    glm::fmat4 model_matrix;

    glm::fmat4 parent_model_matrix = a_planet->getParent()->getWorldTransform();

    //speed
//    printf("assigning speed...\n");


        model_matrix = glm::rotate(parent_model_matrix, float(glfwGetTime() * speed), glm::fvec3{ 0.0f, 1.0f, 0.0f });


    //distance
//    printf("assigning distance...\n");

        model_matrix = glm::translate(model_matrix, glm::fvec3{ 0.0f, 0.0f, dist });


    //scale
//    printf("assigning scale...\n");


        model_matrix = glm::scale(model_matrix, glm::fvec3{ size, size, size });


    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
       1, GL_FALSE, glm::value_ptr(model_matrix));


    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

    a_planet->setWorldTransform(model_matrix);
}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
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

///////////////////////////// callback functions for window events ////////////
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

//SceneGraph * SceneGraph::_instance = nullptr;
// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
