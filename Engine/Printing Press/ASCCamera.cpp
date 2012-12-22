#include "ASCGenInc.h"
#include "ASCCamera.h"
#include "../Ascension/Ascension.h"
#include "ASCRenderer.h"
#include "ASCMatrix4.h"



ASCCamera::ASCCamera(void)
{
	m_v2Position.SetZero();
}


ASCCamera::~ASCCamera(void)
{
}

ASCVector2 
ASCCamera::GetPos()
{
	return m_v2Position;
}
void 
ASCCamera::SetPos(ASCVector2 v2Pos)
{
	m_v2Position = v2Pos;

	ASCMatrix4 mView = Ascension::Renderer().GetViewMatrix();

	mView.m_fValues[3][0] = m_v2Position.X();
	mView.m_fValues[3][1] = -m_v2Position.Y();

	Ascension::Renderer().SetViewMatrix(mView);
}