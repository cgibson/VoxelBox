#ifndef VOXELBOX_H
#define VOXELBOX_H

#include "Brick.h"
#include "VolumeMetaData.h"
#include <string>

using std::string;

namespace VoxelBox {

    void hello();

    BrickGrid loadAsBrickGrid(const string filename);

    Brick* deserializeDensityBrick(char *data, int *x, int *y, int *z);
    char* seriializeDensityBrick(Brick *brick, int x, int y, int z);

    char *serializeMetaData(VolumeMetaDataV1 metaData);
    VolumeMetaDataV1 deserializeMetaData(char *data);

    BrickGrid load(const string filename);
    
    void save(BrickGrid grid, const string filename);
};

#endif
