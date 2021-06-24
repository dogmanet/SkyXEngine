
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PointEntity.h"

/*! \skydocent base_point
Базовый точечный объект
*/

BEGIN_PROPTABLE(CPointEntity)
 // empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CPointEntity, base_point);
REGISTER_ENTITY(CPointEntity, info_landmark, REC_ICON("dev_info_landmark"));
