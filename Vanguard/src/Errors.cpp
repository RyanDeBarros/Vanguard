#include "Errors.h"

vg::Error vg::block_index_out_of_range(size_t size, size_t index)
{
	return Error(ErrorCode::BLOCK_INDEX_OUT_OF_RANGE, std::to_string(index) + " out of range for block size " + std::to_string(size));
}
