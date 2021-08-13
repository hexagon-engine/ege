#pragma once

#include <ege/core/Component.h>

namespace EGE
{

template<class T>
class BasicComponent : public Component<T>
{
public:
    using ChildType = typename Component<T>::ChildType;
    using KeyType = typename Component<T>::KeyType;

    BasicComponent(ComponentBase& parent, String id = "BasicComponent")
    : Component<T>(parent, id) {}

    BasicComponent(String id = "BasicComponent")
    : Component<T>(id) {}

    SharedPtr<ChildType> getChild(KeyType index) const
    {
        return m_children[index];
    }

    size_t childrenCount() const
    {
        return m_children.size();
    }

    // Replacement for old addSubLoop().
    virtual bool addAndLoadChild(SharedPtr<ChildType> child)
    {
        if(child->onLoad() == EventResult::Failure)
            return false;
        m_children.push_back(child);
        return true;
    }

protected:
    template<class Type, class... Args>
    SharedPtr<Type> addNewChild(Args&&... args)
    {
        auto child = make<Type>(std::forward<Args>(args)...);
        addChild(child);
        return child;
    }

    virtual void addChild(SharedPtr<ChildType> child)
    {
         m_children.push_back(child);
    }

    virtual void removeChild(ChildType& child)
    {
        for(auto it = m_children.begin(); it != m_children.end(); it++)
        {
            if(it->get() == &child)
            {
                m_children.erase(it);
                break;
            }
        }
    }

    virtual void forEachChild(std::function<void(ChildType&)>&& function) final override
    {
        for(auto& it: m_children)
        {
            function(*it);
        }
    }

    SharedPtrVector<ChildType> m_children;
};

}
