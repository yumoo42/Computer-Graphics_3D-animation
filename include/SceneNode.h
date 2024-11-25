#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>
#include "model.h"
#include "shader.h"
#include "../lib/glm/glm.hpp"

class SceneNode {
public:
    glm::mat4 transformation;
    Model* model;
    std::vector<SceneNode*> children;

    SceneNode(Model* model);
    ~SceneNode();
    
    void setTransformation(const glm::mat4& matrix);
    void draw(Shader& shader, const glm::mat4& parentTransform = glm::mat4(1.0));
    void addChild(SceneNode* child);
    void removeChild(SceneNode* child);
    const glm::mat4& getTransformation() const;
};

#endif // SCENE_NODE_H
