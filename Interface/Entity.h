#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "IEntity.h"

class Entity : public IEntity
{
    public :
        Entity() {}
        virtual ~Entity() {}
};

#endif // __ENTITY_H__