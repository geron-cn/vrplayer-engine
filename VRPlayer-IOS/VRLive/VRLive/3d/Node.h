//
// Created by tony
//
#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include "Ref.h"
#include "Vector3.h"
#include "vector4.h"
#include "Quaternion.h"
#include "Matrix.h"


namespace vrlive {
    class Sprite3D;
    class Camera;

    class Node : public Ref
    {

        friend class Scene;
    public:
        static Node* create(const std::string& id = "");
        
        void setName(const std::string& name) { _id = name; }
        
        const std::string& getName() const { return _id; }
        
        Node* getParent() const { return _parent; }
        
        int getChildrenCount() const { return (int)_children.size(); }
        
        Node* getChildrenByIndex(int index) const { return _children[index]; }
        
        Node* getChildrenByID(const std::string& id, bool childIncluded = true) const;
        
        void setTranslation(const Vector3& pos);
        
        void setRotation(const Quaternion& quat);
                
        void setScale(const Vector3& scale);
        
        void setTransformMatrix( const Matrix& mat ) { _mat = mat; }
        
        Matrix getTransformMatrix() const { return _mat * _scaleMat; }
        
        Matrix getWorldTransformMatrix() const;
        
        void addChild(Node* node);
        
        void removeChild(Node* node);
        
        void removeAllChildren();
        
        void setSprite3D(Sprite3D* sprite);
        
        Sprite3D* getSprite3D() const { return _sprite; }
        
        virtual void draw(Camera* camera);
        
    protected:
        
        
        
        Node();
        virtual ~Node();
        
    protected:
        std::string _id;
        Node* _parent;
        std::vector<Node*> _children;
        Sprite3D* _sprite;
        Matrix _mat;
        Matrix _scaleMat;
    };
}

#endif
