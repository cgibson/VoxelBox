#ifndef VOLUME_METADATA_H
#define VOLUME_METADATA_H

#define VOLUME_METADATA_V1_SIZE 20

typedef struct VolumeMetaDataV1_t {
    int brick_size;
    int brick_count;
    int brick_dim_x;
    int brick_dim_y;
    int brick_dim_z;

    
} VolumeMetaDataV1;

#endif