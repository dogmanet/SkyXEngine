#ifndef __FUNC_LADDER_H
#define __FUNC_LADDER_H

#include "PointEntity.h"

class CInfoLadderDismount: public CPointEntity
{
	DECLARE_CLASS(CInfoLadderDismount, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

//##########################################################################

#define LADDER_INITIALLY_DISABLED ENT_FLAG_0

class CFuncLadder: public CPointEntity
{
	DECLARE_CLASS(CFuncLadder, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	void setPos(const float3 &pos) override;

	void renderEditor(bool is3D, bool bRenderSelection, IXGizmoRenderer *pRenderer) override;

	void getMinMax(float3 *min, float3 *max) override;

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut = NULL, float3 *pvNormal = NULL, bool isRayInWorldSpace = true, bool bReturnNearestPoint = false) override;

private:
	void setUpPoint(const float3 &vUp);
	void updateFlags() override;

	void turnOn(inputdata_t *pInputdata);
	void turnOff(inputdata_t *pInputdata);
	void toggle(inputdata_t *pInputdata);

private:
	float3_t m_vUpPoint;
	bool m_isUpSet = false;
	bool m_isEnabled = true;

	output_t m_onPlayerGetOn;
	output_t m_onPlayerGetOff;
};

#endif
