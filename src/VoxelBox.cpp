#include "VoxelBox.h"
#include "VolumeMetaData.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>

#define DENSITY_BRICK_SIZE 2054

using std::ofstream;
using std::ifstream;
using std::ios;

namespace VoxelBox {

    void hello()
    {
        printf("Hello, VoxelBox.\n");
    }

    BrickGrid loadAsBrickGrid(const string filename)
    {
        ifstream myFile(filename.c_str(), ios::in | ios::binary);

        // MetaData
        char *data = new char[VOLUME_METADATA_V1_SIZE];

        if(!myFile.read(data, VOLUME_METADATA_V1_SIZE)) {
            printf("Error reading metadata\n");
            exit(1);
        }
        int *dataPtr = (int*)data;
        VolumeMetaDataV1 metaData = deserializeMetaData(data);
        free(data);
        
        // Create grid
        BrickGrid grid = BrickGrid(metaData.brick_dim_x * BRICK_DIM,metaData.brick_dim_y * BRICK_DIM,metaData.brick_dim_z * BRICK_DIM);;

        int pos_x, pos_y, pos_z;
        for(int i = 0; i < metaData.brick_count; i++) {
            data = new char[DENSITY_BRICK_SIZE];
            if(!myFile.read(data, DENSITY_BRICK_SIZE)) {
                printf("Error reading brick %d\n", i);
                exit(1);
            }
            Brick *brick = deserializeDensityBrick(data, &pos_x, &pos_y, &pos_z);
            grid.brick(brick, pos_x, pos_y, pos_z);
            free(data);
        }
        myFile.close();

        return grid;
    }

    BrickGrid load(const string filename)
    {
        std::string::const_iterator pos = std::find(filename.begin(), filename.end(), '.');
        std::string type(pos+1, filename.end());


        if(type == "brk") {
            return loadAsBrickGrid(filename);
        }else{
            printf("File type '%s' not supported\n", type.c_str());
            exit(1);
        }
        
    }

    char* seriializeDensityBrick(Brick *brick, int x, int y, int z)
    {
        char *data = new char[DENSITY_BRICK_SIZE];

        short *x_pos = (short*)(data + 0);
        short *y_pos = (short*)(data + 2);
        short *z_pos = (short*)(data + 4);
        char *voxelPtr = (data + 6);
        float *tmpPtr = (float*)voxelPtr;

        *x_pos = x;
        *y_pos = y;
        *z_pos = z;

        for(int i = 0; i < BRICK_DIM; i++) {
            for(int j = 0; j < BRICK_DIM; j++) {
                for(int k = 0; k < BRICK_DIM; k++) {
                    Voxel *v = brick->get(i,j,k);
                    *tmpPtr = (*v)(DENSITY);
                    tmpPtr++;
                }
            }
        }

        return data;
    }

    Brick* deserializeDensityBrick(char *data, int *x, int *y, int *z)
    {
        Brick *brick = new Brick();

        short *x_pos = (short*)(data + 0);
        short *y_pos = (short*)(data + 2);
        short *z_pos = (short*)(data + 4);
        char *voxelPtr = (data + 6);
        float *tmpPtr = (float*)voxelPtr;

        *x = *x_pos;
        *y = *y_pos;
        *z = *z_pos;

        for(int i = 0; i < BRICK_DIM; i++) {
            for(int j = 0; j < BRICK_DIM; j++) {
                for(int k = 0; k < BRICK_DIM; k++) {
                    Voxel *v = brick->get(i,j,k);
                    v->set(0, *tmpPtr);
                    tmpPtr++;
                }
            }
        }
        return brick;
    }

    char *serializeMetaData(VolumeMetaDataV1 metaData)
    {
        char *data = new char[VOLUME_METADATA_V1_SIZE]; // 2 ints

        int *ptr = (int*)(data + 0);

        *ptr++ = metaData.brick_size;
        *ptr++ = metaData.brick_count;
        *ptr++ = metaData.brick_dim_x;
        *ptr++ = metaData.brick_dim_y;
        *ptr++ = metaData.brick_dim_z;

        ptr = (int*)(data + 0);

        return data;
    }

    VolumeMetaDataV1 deserializeMetaData(char *data)
    {
        VolumeMetaDataV1 metaData;

        int *ptr = (int*)(data + 0);

        metaData.brick_size = *ptr++;
        metaData.brick_count = *ptr++;
        metaData.brick_dim_x = *ptr++;
        metaData.brick_dim_y = *ptr++;
        metaData.brick_dim_z = *ptr++;

        return metaData;
    }

    void save(BrickGrid grid, const string filename)
    {
        ofstream myFile;
        myFile.open (filename.c_str(), ios::out | ios::binary);

        // MetaData
        VolumeMetaDataV1 metaData = {DENSITY_BRICK_SIZE, grid.brickCount(), grid.brick_size_x(), grid.brick_size_y(), grid.brick_size_z()};
        char *data = serializeMetaData(metaData);
        myFile.write(data, VOLUME_METADATA_V1_SIZE);
        free(data);

        for(int i = 0; i < grid.brick_size_x(); i++) {
            for(int j = 0; j < grid.brick_size_y(); j++) {
                for(int k = 0; k < grid.brick_size_z(); k++) {
                    data = seriializeDensityBrick(grid.brick(i,j,k), i,j,k);
                    myFile.write(data, DENSITY_BRICK_SIZE);
                    free(data);
                }
            }
        }
        myFile.close();
    }

    /*
     * VoxelBox BRK format:
     *
     * MetaData Version (1 byte)
     * MetaData Size in Bytes(4 bytes)
     * MetaData (x bytes)
     *
     * VoxelData Size in Bytes (4 bytes)
     * VoxelData (x bytes)
     *----------------------------------
     *
     * MetaData Version 1:
     *
     * Brick Size in Bytes (4 bytes)
     * Brick Dim X in bytes (4 bytes)
     * Brick Dim Y in bytes (4 bytes)
     * Brick Dim Z in bytes (4 bytes)
     * Brick Count (4 bytes)
     *
     * FORCES BRICKS TO IDENTIFY THEIR LOCATION INFORMATION WITHIN THE GRID
     */

    /*
     * Density Brick Format:
     * x_pos (2 bytes)
     * y_pos (2 bytes)
     * z_pos (2 bytes)
     * voxel data (8*8*8*4 bytes)
     *
     * TOTAL: 2054 bytes
     */
}
