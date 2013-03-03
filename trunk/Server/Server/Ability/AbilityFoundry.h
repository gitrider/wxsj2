// AbilityFoundry.h

#ifndef __ABILITYFOUNDRY_H__
#define __ABILITYFOUNDRY_H__

#include "Ability.h"
#include "AbilityCompound.h"

class AbilityFoundry : public AbilityCompound
{
	
public:
	AbilityFoundry(const Ability& a);
	BOOL	ValidateSpecStuff( UINT uItemIndex );

private:
};

#endif // __ABILITYFOUNDRY_H__