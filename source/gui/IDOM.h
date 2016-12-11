#ifndef _IDOM_H_
#define _IDOM_H_

#ifdef GUI_DLL
#include "GUIbase.h"
#include "ICSS.h"
#else
#include "ICSSstyle.h"
#endif
#include "IEvent.h"

namespace GUI
{
	namespace DOM
	{
		class IDOMnode
		{
		public:
			virtual ~IDOMnode()
			{
			};
			virtual void AppendChild(IDOMnode * pEl, bool regen = true) = 0;
			virtual void RemoveChild(IDOMnode * pEl, bool regen = true) = 0;
			virtual IDOMnode * ParentNode() = 0;

			virtual void SetText(const StringW & text, BOOL build=FALSE) = 0;
			virtual const StringW & GetText() = 0;

			virtual bool IsTextNode() = 0;

			virtual void SetAttribute(const StringW & name, const StringW & value) = 0;
			virtual const StringW & GetAttribute(const StringW & name) = 0;

			
			virtual bool IsChildOf(IDOMnode * pNode) = 0;
			virtual bool IsAfter(IDOMnode * pNode) = 0;

			virtual IDOMnode * PrevSibling() = 0;

			virtual IDOMnode * NextSibling() = 0;

			virtual CSS::ICSSstyle * GetStyle() = 0;

			virtual CSS::ICSSstyle * GetStyleSelf() = 0;

			virtual const Array<IDOMnode*> * GetChilds() = 0;

			virtual void AddPseudoclass(UINT id) = 0;
			virtual bool PseudoclassExists(UINT id) = 0;
			virtual void RemovePseudoclass(UINT id) = 0;

			virtual void DispatchEvent(IEvent & ev) = 0;
			virtual void DispatchClientEvent(IEvent ev, bool * preventDefault) = 0;

			virtual void UpdateStyles() = 0;

			virtual void ClassAdd(const StringW & cls) = 0;

			virtual void ClassRemove(const StringW & cls) = 0;

			virtual void ClassToggle(const StringW & cls, int set = -1) = 0;

			virtual BOOL ClassExists(const StringW & cls) = 0;
		};


		typedef Array<IDOMnode*> IDOMnodeCollection;
	};
};

#endif
