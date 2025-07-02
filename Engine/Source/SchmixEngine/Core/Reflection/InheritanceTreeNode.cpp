#include "InheritanceTreeNode.h"

namespace SchmixEngine
{
	InheritanceTreeNode::InheritanceTreeNode(const ClassInfo* pClassInfo)
		: m_pClassInfo(pClassInfo),
		m_pParent(nullptr),
		m_pCastPtrFromParentFunction(nullptr),
		m_pCastPtrToParentFunction(nullptr)
	{
	}

	const InheritanceTreeNode* InheritanceTreeNode::GetParent() const
	{
		return m_pParent;
	}

	void InheritanceTreeNode::SetParent(InheritanceTreeNode* pParent)
	{
		if (pParent != m_pParent && pParent != this)
		{
			if (m_pParent)
			{
				InheritanceTreeNode* pCurrentParent = const_cast<InheritanceTreeNode*>(m_pParent);

				auto Iterator = std::find(pCurrentParent->m_Children.begin(), pCurrentParent->m_Children.end(), this);
				pCurrentParent->m_Children.erase(Iterator);
			}

			if (pParent)
			{
				InheritanceTreeNode* pNewParent = const_cast<InheritanceTreeNode*>(pParent);
				pNewParent->m_Children.push_back(this);
			}

			m_pParent = pParent;
		}
	}

	const std::vector<const InheritanceTreeNode*>& InheritanceTreeNode::GetChildren() const
	{
		return m_Children;
	}

	const ClassInfo* InheritanceTreeNode::GetClassInfo() const
	{
		return m_pClassInfo;
	}

	void InheritanceTreeNode::SetClassInfo(const ClassInfo* pClassInfo)
	{
		m_pClassInfo = pClassInfo;
	}

	void InheritanceTreeNode::SetCastPtrToParentTypeFunction(PtrCastFunction pCastPtrToParentFunction)
	{
		m_pCastPtrToParentFunction = pCastPtrToParentFunction;
	}

	const void* InheritanceTreeNode::CastPtrToParentType(const void* pSrc) const
	{
		if (m_pCastPtrToParentFunction && pSrc)
			return m_pCastPtrToParentFunction(pSrc);

		return nullptr;
	}

	void InheritanceTreeNode::SetCastPtrFromParentTypeFunction(PtrCastFunction pCastPtrFromParentFunction)
	{
		m_pCastPtrFromParentFunction = pCastPtrFromParentFunction;
	}

	const void* InheritanceTreeNode::CastPtrFromParentType(const void* pSrc) const
	{
		if (m_pCastPtrFromParentFunction && pSrc)
			return m_pCastPtrFromParentFunction(pSrc);

		return nullptr;
	}
}
