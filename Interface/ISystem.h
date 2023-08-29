#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "IObject.h"

class ISystem : public IObject
{
    public :
        ISystem() {};
        ISystem( MyUUID ID ) : IObject( ID ) {};
        virtual ~ISystem(){};

    public :
        virtual void Update( float DeltaTime, int ScenIndex, MyUUIDUnSet& EntityID ) = 0;

        virtual void SetNodeType( const std::type_info* Type ) = 0;
        const std::type_info*& GetNodeType() { return m_NodeType; }

    protected :
        const std::type_info* m_NodeType;
};

using ISystemQueue = std::queue< ISystem* >;
using ISystemVector = std::vector< ISystem* >;
using ISystemSet = std::set< ISystem* >;
using ISystemUnSet = std::unordered_set< ISystem* >;

#endif // __ISYSTEM_H__