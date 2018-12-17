#ifndef VSANIMATOM_H
#define VSANIMATOM_H
#include "VSMatrix3X3W.h"
#include "VSGraphic.h"
#include "VSTransform.h"
/*
	

*/
namespace VSEngine2
{
	class VSGRAPHIC_API VSAnimAtom
	{
	public:
		VSAnimAtom()
		{
			Identity();
		}
		~VSAnimAtom()
		{
			
		}
		static VSAnimAtom Create();
		VSVector3	m_fScale;
		VSVector3	m_Pos;
		VSQuat		m_Rotator;
		void GetMatrix(VSMatrix3X3W & OutMatrix)const;
		void Interpolation(const VSAnimAtom & Atom1 , const VSAnimAtom Atom2,VSREAL t);
		void Identity();
		void FromTransfrom(const VSTransform &T);
		void FromMatrix(const VSMatrix3X3W & m);

		void    operator *= (VSREAL f);
		VSAnimAtom operator *  (VSREAL f);

		void    operator += (const VSAnimAtom &Atom);
		VSAnimAtom operator +  (const VSAnimAtom &Atom) const;
		void    operator -= (const VSAnimAtom &Atom);
		VSAnimAtom operator -  (const VSAnimAtom &Atom) const;
		void BlendWith(const VSAnimAtom &Atom);
		VSAnimAtom Blend(const VSAnimAtom &Atom);

		void AddTwo(const VSAnimAtom & Atom1, const VSAnimAtom Atom2);
		void AddTwo(const VSAnimAtom & Atom1);
	};
	
}
#endif