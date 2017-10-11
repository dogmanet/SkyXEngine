
#include "redefined_func.h"

void RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{

}

void RFuncMtlSet(ID id, float4x4* world)
{
	switch (Core_RIntGet(G_RI_INT_RENDERSTATE))
	{
	case RENDER_STATE_SHADOW:
		SML_MtlSetMainTexture(0, id);
		SML_LigthsShadowSetShaderOfTypeMat(Core_RIntGet(G_RI_INT_CURRIDLIGHT), SML_MtlGetTypeModel(id), world);
		break;

	case RENDER_STATE_FREE:
		SML_MtlSetMainTexture(0, id);
		Core_RMatrixSet(G_RI_MATRIX_WORLD, &(world ? (*world) : SMMatrixIdentity()));
		//SGCore_ShaderUnBind();
		SML_MtlRenderStd(SML_MtlGetTypeModel(id), world, 0, id);
		break;

	case RENDER_STATE_MATERIAL:
		SML_MtlRender(id, world);
		break;
	}
}

ID RFuncMtlLoad(const char* name, int mtl_type)
{
	return SML_MtlLoad(name, (MtlTypeModel)mtl_type);
}

bool RFuncAIQuadPhyNavigate(float3_t * pos)
{
	static btBoxShape boxfull(btVector3(AIGRID_QUAD_SIZEDIV2, AIGRID_ENTITY_MAX_HEIGHTDIV2, AIGRID_QUAD_SIZEDIV2));
	float3 start = (*pos);
	start.y = pos->y + AIGRID_ENTITY_MAX_HEIGHT;
	float3 end = (*pos);
	//end.y = min->y - AIGRID_ENTITY_MAX_HEIGHT;
	static btDiscreteDynamicsWorld::ClosestConvexResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
	cb = btDiscreteDynamicsWorld::ClosestConvexResultCallback(F3_BTVEC(start), F3_BTVEC(end));

	static btTransform xForm;
	xForm.setOrigin(F3_BTVEC(start));
	xForm.getBasis().setIdentity();
	static btTransform xForm2;
	xForm2.setOrigin(F3_BTVEC(end));
	xForm2.getBasis().setIdentity();
	SXPhysics_GetDynWorld()->convexSweepTest(&boxfull, xForm, xForm2, cb);

	if (cb.hasHit())
	{
		pos->y = cb.m_hitPointWorld[1];
		//quad->IsClose = false;

		static btBoxShape boxoff(btVector3(AIGRID_QUAD_SIZEDIV2, (AIGRID_ENTITY_MAX_HEIGHT - AIGRID_QUADS_CENTERS_MAXHEIGHT) * 0.5, AIGRID_QUAD_SIZEDIV2));

		start = (*pos);
		start.y = pos->y + AIGRID_ENTITY_MAX_HEIGHTDIV2 + AIGRID_QUADS_CENTERS_MAXHEIGHT;
		static btVector3 vec;
		vec = btVector3(F3_BTVEC(start));
		cb = btDiscreteDynamicsWorld::ClosestConvexResultCallback(vec, vec);
		static btVector3 offs;
		for (int x = -1; x <= 1; ++x)
		{
			for (int z = -1; z <= 1; ++z)
			{
				offs[0] = 0.5f*float(x) * 0.01f;
				offs[1] = 1.f * 0.01f;
				offs[2] = 0.5f*float(z) * 0.01f;
				xForm.setOrigin(vec - offs);
				xForm.getBasis().setIdentity();
				xForm2.setOrigin(vec + offs);
				xForm2.getBasis().setIdentity();
				SXPhysics_GetDynWorld()->convexSweepTest(&boxoff, xForm, xForm2, cb);

				if (cb.hasHit())
				{
					return true;
				}
			}
		}
	}
	else
		return true;

	return false;
}

bool RFuncParticlesPhyCollision(const float3 * lastpos, const float3* nextpos, float3* coll_pos, float3* coll_nrm)
{
	if (!lastpos || !nextpos)
		return false;

	if (lastpos->x == nextpos->x && lastpos->y == nextpos->y && lastpos->z == nextpos->z)
		return false;

	btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(*lastpos), F3_BTVEC(*nextpos));
	SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(*lastpos), F3_BTVEC(*nextpos), cb);

	if (cb.hasHit())
	{
		if (coll_pos)
			*coll_pos = BTVEC_F3(cb.m_hitPointWorld);

		if (coll_nrm)
			*coll_nrm = BTVEC_F3(cb.m_hitNormalWorld);

		return true;
	}

	return false;
}