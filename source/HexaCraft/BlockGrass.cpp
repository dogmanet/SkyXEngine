#include "BlockGrass.h"
#include "BaseBlock.h"

HC_DEFINE_BLOCK(HC_TYPEID(HC_BLOCK_GRASS), CBlockGrass);

HC_BEGIN_MATERIAL_TABLE(CBlockGrass)
	"block_grass_top",
	"block_grass_side"
HC_END_MATERIAL_TABLE(CBlockGrass);
