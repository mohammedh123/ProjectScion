#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML\Graphics.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#include "Attribute.h"
#include "Behavior.h"
#include "IComponent.h"

#include <memory>
#include <vector>

class Entity
{
    friend class ScionEngine;

    ScionEngine* game;
    Entity(){}
    
    std::vector<std::unique_ptr<IComponent>> components;
    std::vector<Attribute*> attributes;
    std::vector<Behavior*> logicalBehaviors, renderBehaviors;
public:
    void AddBehavior(Behavior* b) { if(b->IsRenderingBehavior()) renderBehaviors.push_back(b); else logicalBehaviors.push_back(b); }
    
    #define ENTITY_PP_PARAM_LIST(z, n, data) BOOST_PP_CAT(P, n)&& BOOST_PP_CAT(p, n)
    #define ENTITY_PP_PARAM_PASS(z, n, data) BOOST_PP_CAT(p, n)
    #define ENTITY_PP_PARAM_TYPE(z, n, data) BOOST_PP_CAT(P, n)

    #define BOOST_PP_LOCAL_MACRO(n) \
        template<typename T BOOST_PP_ENUM_TRAILING_PARAMS(n, typename P) > \
        Entity* AddComponent(BOOST_PP_ENUM(n, ENTITY_PP_PARAM_LIST, _)) \
        { \
            components.push_back(std::unique_ptr<IComponent>(new T(BOOST_PP_ENUM(n, ENTITY_PP_PARAM_PASS, _)))); \
            return this; \
        } \
        /**/

    #define BOOST_PP_LOCAL_LIMITS (0, 10)
    #include BOOST_PP_LOCAL_ITERATE()

    inline const std::vector<Attribute*>& GetAttributes() const { return attributes; }
};

#endif