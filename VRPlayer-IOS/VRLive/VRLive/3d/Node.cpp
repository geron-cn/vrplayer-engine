#include "Node.h"
#include "Sprite3D.h"

namespace vrlive {
   
    Node* Node::create(const std::string& id)
    {
        auto node = new Node();
        node->_id = id;
        
        return node;
    }
    
    
    Node* Node::getChildrenByID(const std::string& id, bool childIncluded) const
    {
        for (auto it : _children) {
            if (it->_id == id)
                return it;
        }
        if (childIncluded)
        {
            for (auto it : _children) {
                auto node = it->getChildrenByID(id);
                if (node)
                   return node;
            }
        }
        return nullptr;
    }
    
    Matrix Node::getWorldTransformMatrix() const
    {
        if (_parent)
        {
            return _parent->getWorldTransformMatrix() * _mat;
        }
        return _mat;
    }
    
    void Node::setTranslation(const Vector3& pos)
    {
        _mat.m[12] = pos.x;
        _mat.m[13] = pos.y;
        _mat.m[14] = pos.z;
    }
    
    void Node::setRotation(const Quaternion& quat)
    {
        Matrix mat;
        Matrix::createRotation(quat, &mat);
        memcpy(_mat.m, mat.m, sizeof(float) * 12);
    }
    
    void Node::setScale(const Vector3& scale)
    {
        Matrix matScale;
        Matrix::createScale(scale, &matScale);
        _mat.multiply(matScale);
    }
    
    void Node::addChild(Node* node)
    {
        node->addRef();
        
        auto parent = node->_parent;
        if (parent)
        {
            parent->removeChild(node);
        }
        _children.push_back(node);
        node->_parent = this;
    }
    
    void Node::removeChild(Node* node)
    {
        for (int k = 0; k < (int)_children.size(); k++) {
            if (_children[k] == node)
            {
                node->_parent = nullptr;
                node->release();
                _children.erase(_children.begin() + k);
                break;
            }
        }
    }
    
    void Node::removeAllChildren()
    {
        for (auto it : _children) {
            it->release();
        }
        _children.erase(_children.begin(), _children.end());
    }
    
    void Node::setSprite3D(Sprite3D* sprite)
    {
        _sprite = sprite;
        _sprite->_node = this;
        _sprite->addRef();
    }
    
    Node::Node()
    : _sprite(nullptr)
    , _parent(nullptr)
    {
        
    }
    Node::~Node()
    {
        
    }
    
}
