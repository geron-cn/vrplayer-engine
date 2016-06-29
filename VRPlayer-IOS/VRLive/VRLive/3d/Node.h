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

    class Node : public Ref
    {

    public:
        static Node* create(const std::string& id = "");
        
        Node* getParent() const { return _parent; }
        
        int getChildrenCount() const { return (int)_children.size(); }
        
        Node* getChildrenByIndex(int index) const { return _children[index]; }
        
        Node* getChildrenByID(const std::string& id, bool childIncluded = true) const;
        
        void setTranslation(const Vector3& pos);
        
        void setRotation(const Quaternion& quat);
        
        void setScale(const Vector3& scale);
        
        void setTransformMatrix( const Matrix& mat ) { _mat = mat; }
        
        const Matrix& getTransformMatrix() const { return _mat; }
        
        Matrix getWorldTransformMatrix() const;
        
        void addChild(Node* node);
        
        void removeChild(Node* node);
        
        void removeAllChildren();
        
        void setSprite3D(Sprite3D* sprite);
        
        Sprite3D* getSprite3D() const { return _sprite; }
        
    protected:
        
        Node();
        virtual ~Node();
        
    protected:
        std::string _id;
        Node* _parent;
        std::vector<Node*> _children;
        Sprite3D* _sprite;
        Matrix _mat;
    };
}

#endif
