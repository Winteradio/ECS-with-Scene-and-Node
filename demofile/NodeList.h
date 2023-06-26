#ifndef __NODE_H__
#define __NODE_H__

#include <ECSProject/INode.h>

class RenderNode : public INode
{
    public :
        RenderNode() {};
        RenderNode( MyUUID ID ) : INode( ID ) {}
        virtual ~RenderNode(){};

    public :
        virtual bool Init( Entity& Object )
        {
            Log::Info(" Init RenderNode in %s ", Object.GetID().GetString().c_str() );

            return true;
        }
};

#endif // __NODE_H__