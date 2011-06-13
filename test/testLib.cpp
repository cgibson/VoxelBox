#include "VoxelBox.h"
#include "VolumeMetaData.h"

int main(int argc, char* argv[]) {

    VoxelBox::hello();

    // TEST - SERIALIZATION AND DESERIALIZATION OF BRICKS
    
    BrickGrid *grid = new BrickGrid(64,64,64, 0.1);

    Brick *b1 = grid->brick(0,0,0);

    char *data = VoxelBox::seriializeDensityBrick(b1, 1, 2, 3);

    int pos_x, pos_y, pos_z;

    Brick *b2 = VoxelBox::deserializeDensityBrick(data, &pos_x, &pos_y, &pos_z);

    assert(pos_x == 1);
    assert(pos_y == 2);
    assert(pos_z == 3);

    assert(b1 != NULL);
    assert(b2 != NULL);

    for(int i = 0; i < BRICK_DIM; i++) {
        for(int j = 0; j < BRICK_DIM; j++) {
            for(int k = 0; k < BRICK_DIM; k++) {
                Voxel *v1 = b1->get(i,j,k);
                Voxel *v2 = b2->get(i,j,k);
                assert((*v1)(DENSITY) - (*v2)(DENSITY) < 0.00001);
            }
        }
    }

    free(data);

    // TEST - METADATA SERIALIZATION AND DESERIALIZATION

    VolumeMetaDataV1 md1 = {10, 20, 5, 6, 7};
    data = VoxelBox::serializeMetaData(md1);

    VolumeMetaDataV1 md2 = VoxelBox::deserializeMetaData(data);

    assert(md1.brick_count == md2.brick_count);
    assert(md1.brick_size == md2.brick_size);
    assert(md1.brick_dim_x == md2.brick_dim_x);
    assert(md1.brick_dim_y == md2.brick_dim_y);
    assert(md1.brick_dim_z == md2.brick_dim_z);

    free(data);

    // TEST - VOXEL FILE FORMAT

    VoxelBox::save(grid, "test.brk");

    BrickGrid *grid2 = (BrickGrid*)VoxelBox::load("test.brk");

    assert(grid2 != NULL);

    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {
            for(int k = 0; k < 64; k++) {
                Voxel *v1 = (*grid)(i,j,k);
                Voxel *v2 = (*grid2)(i,j,k);
                assert((*v1)(DENSITY) - (*v2)(DENSITY) < 0.00001);
            }
        }
    }
  return 0;

}
