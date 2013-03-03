/****************************************\
*										*
* 				����������				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

class CSoundListenerFMod
{
public:
    //������λ��
	virtual void			SetPos(const fVector3& pos);
	virtual const fVector3&	GetPos(void) const	{ return m_vPos; }

    //�������ƶ��ٶ�(ȱʡΪ0)
	virtual void			SetVelocity(const fVector3& vel);
	virtual const fVector3&	GetVelocity(void) const { return m_vVel; }

    //�����ߵķ����泯�ķ����ͷ���ķ��� (default: (0,0,1), (0,1,0))
	/*
	|     (top)
	|       y    
	|       |  x (left)
	|       | /
	|       -----z (front)
	*/
	virtual void			SetFront(const fVector3& front);
	virtual const fVector3&	GetFont(void) const { return m_vFront; }

	virtual void			SetUp(const fVector3& up);
	virtual const fVector3&	GetUp(void) const { return m_vUp; }

protected:
	void	_updateFModPos(void);

	// Nel Basis
	fVector3			m_vPos;
	fVector3			m_vVel;
	fVector3			m_vFront;
	fVector3			m_vUp;

public:
	CSoundListenerFMod();
	virtual ~CSoundListenerFMod();

	static CSoundListenerFMod* GetMe(VOID) { return s_pMe; }

protected:
	static CSoundListenerFMod*	s_pMe;
};

