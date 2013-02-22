#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
#include "ramNodeIdentifer.h"
#include "ramGlobal.h"

class ramNodeFinder : public ramNodeIdentifer, public ramGlobalShortcut
{
public:

	ramNodeFinder() : ramNodeIdentifer() {}
	ramNodeFinder(const ramNodeIdentifer& copy) : ramNodeIdentifer(copy) {}

	void setTargetName(string name_) { name = name_; }
	void setJointID(int index_) { index = index_; }

	inline bool found()
	{
		if (!hasNodeArray(name))
			return false;

		return index == -1
			   || (index >= 0 && index < getNodeArray(name).getNumNode());
	}

	//

	bool findOne(ramNode &node)
	{
		if (!isValid()) return false;

		if (!hasNodeArray(name))
		{
			if (getNumNodeArray() > 0)
			{
				node = getNodeArray(0).getNode(index);
				return true;
			}
			return false;
		}
		else
		{
			node = getNodeArray(name).getNode(index);
			return true;
		}
	}

	//

	vector<ramNode> findAll()
	{
		vector<ramNode> nodes;

		bool has_target_actor = !name.empty();
		bool has_target_node = index != -1;

		for (int i = 0; i < getNumNodeArray(); i++)
		{
			ramNodeArray &actor = getNodeArray(i);

			if (has_target_actor && name != actor.getName()) continue;

			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && index != n) continue;

				ramNode &node = actor.getNode(n);
				nodes.push_back(node);
			}
		}

		return nodes;
	}

};