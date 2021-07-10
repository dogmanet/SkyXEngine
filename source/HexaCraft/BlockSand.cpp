#include "BlockSand.h"
#include "BaseBlock.h"

HC_DEFINE_BLOCK(HC_TYPEID(HC_BLOCK_SAND), CBlockSand);

HC_BEGIN_MATERIAL_TABLE(CBlockSand)
	"block_sand_top",
	"block_sand_side"
HC_END_MATERIAL_TABLE(CBlockSand);
