#ifndef _ABILITY_INL_
#define _ABILITY_INL_


AbilityID_t	Ability::AbilityID()
{
	return m_AbilityID;
}

VOID	    Ability::AbilityID(AbilityID_t id)
{ 
	m_AbilityID = id;
}

const CHAR*	Ability::AbilityName()const 
{
	return m_AbilityName;
}

VOID		Ability::AbilityName(const CHAR* name)
{ 
	m_AbilityName = name;
}

INT			Ability::LevelDemand()
{ 
	return m_LevelDemand;
}

VOID		Ability::LevelDemand(INT lvl)
{ 
	m_LevelDemand = lvl;
}

INT			Ability::AbilityLevelLimit()
{ 
	return m_AbilityLevelLimit;
}

VOID		Ability::AbilityLevelLimit(INT lvl)
{ 
	m_AbilityLevelLimit = lvl;
}

INT		Ability::LevelUpConfigIndex()
{
	return m_LevelUpConfigIndex;
}

VOID	Ability::LevelUpConfigIndex(INT idx)
{
	m_LevelUpConfigIndex = idx;
}

INT			Ability::OperationTime()
{ 
	return m_OperationTime;
}

VOID		Ability::OperationTime(INT time)
{ 
	m_OperationTime = time;
}

UINT		Ability::OperationToolID()
{ 
	return m_OperationToolID;
}

VOID		Ability::OperationToolID(INT id)
{
	if( id == -1 || id <= 0 )
	{
		m_OperationToolID = -1;
	}

	m_OperationToolID = id;
}

INT			Ability::PlatformID()
{ 
	return m_PlatformID;
}

VOID		Ability::PlatformID(INT id)
{ 
	m_PlatformID = id;
}

FLOAT		Ability::PlatformDistance()
{ 
	return m_PlatformDistance;
}

VOID		Ability::PlatformDistance(FLOAT dis)
{ 
	m_PlatformDistance = dis;
}

UINT		Ability::OperationActionID()
{ 
	return m_OperationActionID;
}

VOID		Ability::OperationActionID(INT id)
{ 
	m_OperationActionID = id;
}

#endif
