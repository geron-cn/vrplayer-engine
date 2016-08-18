#include "Node.h"
#include "Sprite3D.h"
#include "Action.h"


// for test
#include "../FileUtils/FileUtils.h"

namespace vrlive {
   
    Node* Node::create(const std::string& id)
    {
        auto node = new Node();
        node->_id = id;
        
        return node;
    }
    
    void Node::update(Scene* scene)
    {
        for (auto it : _children) {
            it->update(scene);
        }
    }
    
    void Node::draw(Camera* camera)
    {
        if (!_isVisible)
            return;
        
        if (_sprite)
        {
            _sprite->draw(camera);
        }
        for (auto it : _children) {
            it->draw(camera);
        }
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
            return _parent->getWorldTransformMatrix() * getTransformMatrix();
        }
        return getTransformMatrix();
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
//        Matrix matScale;
        Matrix::createScale(scale, &_scaleMat);
//        _mat.multiply(matScale);
    }
    
    void Node::runAction(Action* action)
    {
        action->setTarget(this);
        ActionMgr::getInstance()->addAction(action);
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
        int len = _children.size();
        for( int i = len -1; i >= 0; i--)
        {
            if(_children[i] == node)
            {
                node->_parent = nullptr;
                node->release();
                _children.erase(_children.begin() + i);
                break;
            }
        }
    }
    
    void Node::removeChild(const std::string& name)
    {
        int len = _children.size();
        for( int i = len -1; i >= 0; i--)
        {
            auto node = _children[i];
            if(node->_id == name)
            {
                node->_parent = nullptr;
                node->release();
                _children.erase(_children.begin() + i);
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
    , _isVisible(true)
    {
        
    }
    Node::~Node()
    {
        LOG("Node destruct %s", _id.c_str() );
        ActionMgr::getInstance()->removeActionByNode(this);
        
        if (_sprite)
        {
            _sprite->release();
            _sprite = NULL;
        }
        LOG("Node %s destruct end",  _id.c_str());
        removeAllChildren();
    }
}
