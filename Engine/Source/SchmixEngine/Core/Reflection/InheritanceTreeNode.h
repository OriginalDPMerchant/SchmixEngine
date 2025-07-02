#pragma once
#include <vector>

namespace SchmixEngine
{
	class ClassInfo;

	class InheritanceTreeNode
	{
	public:
		typedef const void* (*PtrCastFunction)(const void*);

		InheritanceTreeNode(const ClassInfo* pClassInfo = nullptr);

		const InheritanceTreeNode* GetParent() const;

		void SetParent(InheritanceTreeNode* pParent);

		const std::vector<const InheritanceTreeNode*>& GetChildren() const;

		const ClassInfo* GetClassInfo() const;

		void SetClassInfo(const ClassInfo* pClassInfo);

		void SetCastPtrToParentTypeFunction(PtrCastFunction pCastPtrToParentFunction);

		const void* CastPtrToParentType(const void* pSrc) const;

		void SetCastPtrFromParentTypeFunction(PtrCastFunction pCastPtrFromParentFunction);

		const void* CastPtrFromParentType(const void* pSrc) const;

	private:
		const InheritanceTreeNode* m_pParent;
		std::vector<const InheritanceTreeNode*> m_Children;

		const ClassInfo* m_pClassInfo;

		PtrCastFunction m_pCastPtrToParentFunction;
		PtrCastFunction m_pCastPtrFromParentFunction;
	};
}