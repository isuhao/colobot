// compass.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "D3DEngine.h"
#include "math3d.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "compass.h"




// Constructeur de l'objet.

CCompass::CCompass(CInstanceManager* iMan) : CControl(iMan)
{
	CControl::CControl(iMan);

	m_dir = 0.0f;
}

// Destructeur de l'objet.

CCompass::~CCompass()
{
	CControl::~CControl();
}


// Cr�e un nouveau bouton.

BOOL CCompass::Create(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	CControl::Create(pos, dim, icon, eventMsg);
	return TRUE;
}


// Gestion d'un �v�nement.

BOOL CCompass::EventProcess(const Event &event)
{
	CControl::EventProcess(event);

	if ( event.event == EVENT_LBUTTONDOWN )
	{
		if ( CControl::Detect(event.pos) )
		{
			Event newEvent = event;
			newEvent.event = m_eventMsg;
			m_event->AddEvent(newEvent);
			return FALSE;
		}
	}

	return TRUE;
}


// Dessine le bouton.

void CCompass::Draw()
{
	LPDIRECT3DDEVICE7 device;
	D3DVERTEX2		vertex[4];	// 2 triangles
	FPOINT			p1, p2, p3, c, uv1, uv2;
	D3DVECTOR		n;
	float			dp;

	if ( (m_state & STATE_VISIBLE) == 0 )  return;

	device = m_engine->RetD3DDevice();

	m_engine->SetTexture("button1.tga");
	m_engine->SetState(D3DSTATENORMAL);

	p1.x = m_pos.x;
	p1.y = m_pos.y;
	p2.x = m_pos.x + m_dim.x;
	p2.y = m_pos.y + m_dim.y;

	c.x = (p1.x+p2.x)/2.0f;
	c.y = (p1.y+p2.y)/2.0f;  // centre

	uv1.x = 64.0f/256.0f;
	uv1.y =  0.0f/256.0f;
	uv2.x = 96.0f/256.0f;
	uv2.y = 32.0f/256.0f;

	dp = 0.5f/256.0f;
	uv1.x += dp;
	uv1.y += dp;
	uv2.x -= dp;
	uv2.y -= dp;

	n = D3DVECTOR(0.0f, 0.0f, -1.0f);  // normale

	vertex[0] = D3DVERTEX2(D3DVECTOR(p1.x, p1.y, 0.0f), n, uv1.x,uv2.y);
	vertex[1] = D3DVERTEX2(D3DVECTOR(p1.x, p2.y, 0.0f), n, uv1.x,uv1.y);
	vertex[2] = D3DVERTEX2(D3DVECTOR(p2.x, p1.y, 0.0f), n, uv2.x,uv2.y);
	vertex[3] = D3DVERTEX2(D3DVECTOR(p2.x, p2.y, 0.0f), n, uv2.x,uv1.y);

	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX2, vertex, 4, NULL);
	m_engine->AddStatisticTriangle(2);

	if ( m_state & STATE_ENABLE )
	{
		p1.x = c.x;
		p1.y = c.y+m_dim.x*0.40f;
		p1 = RotatePoint(c, m_dir, p1);
		p1.x = c.x+(p1.x-c.x)*(m_dim.x/m_dim.y);

		p2.x = c.x+m_dim.x*0.20f;
		p2.y = c.y-m_dim.x*0.40f;
		p2 = RotatePoint(c, m_dir, p2);
		p2.x = c.x+(p2.x-c.x)*(m_dim.x/m_dim.y);

		p3.x = c.x-m_dim.x*0.20f;
		p3.y = c.y-m_dim.x*0.40f;
		p3 = RotatePoint(c, m_dir, p3);
		p3.x = c.x+(p3.x-c.x)*(m_dim.x/m_dim.y);

		uv1.x =  96.0f/256.0f;
		uv1.y =  32.0f/256.0f;
		uv2.x = 104.0f/256.0f;
		uv2.y =  64.0f/256.0f;

		uv1.x += dp;
		uv1.y += dp;
		uv2.x -= dp;
		uv2.y -= dp;

		vertex[0] = D3DVERTEX2(D3DVECTOR(p1.x, p1.y, 0.0f), n, uv1.x,uv1.y);
		vertex[1] = D3DVERTEX2(D3DVECTOR(p2.x, p2.y, 0.0f), n, uv1.x,uv2.y);
		vertex[2] = D3DVERTEX2(D3DVECTOR(p3.x, p3.y, 0.0f), n, uv2.x,uv2.y);

		device->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_VERTEX2, vertex, 3, NULL);
		m_engine->AddStatisticTriangle(1);
	}
}


// Gestion des directions de la boussole.

void CCompass::SetDirection(float dir)
{
	m_dir = dir;
}

float CCompass::RetDirection()
{
	return m_dir;
}

