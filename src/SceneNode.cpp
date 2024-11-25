#include "../include/SceneNode.h"

SceneNode::SceneNode(Model* model) : model(model), transformation(glm::mat4(1.0f)) {}

SceneNode::~SceneNode() {
    for (SceneNode* child : children) {
        delete child;
    }
    children.clear();
}

void SceneNode::setTransformation(const glm::mat4& matrix) {
    transformation = matrix;
}

const glm::mat4& SceneNode::getTransformation() const {
    return transformation;
}

void SceneNode::draw(Shader& shader, const glm::mat4& parentTransform) {
    glm::mat4 globalTransformation = parentTransform * transformation;
    if (model) {
        shader.use();
        shader.setMat4("model", globalTransformation);
        model->Draw(shader);
    }
    for (SceneNode* child : children) {
        child->draw(shader, globalTransformation);
    }
}

void SceneNode::addChild(SceneNode* child) {
    children.push_back(child);
}

void SceneNode::removeChild(SceneNode* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}