#include <Graph/Graph.h>

namespace ECS
{
	void GraphObserver::onInit()
	{
		m_updated = false;
	}

	void GraphObserver::onUpdate()
	{
		m_updated = true;
	}

	const bool GraphObserver::IsUpdated() const
	{
		return m_updated;
	}
}