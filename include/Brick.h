#ifndef BRICK_H_
#define BRICK_H_

#include <stdlib.h>
#include <stdio.h>
#include "Voxel.h"

#include <stdio.h>

#define BRICK_DIM 8

class Brick {
public:
	Brick();
	virtual ~Brick();

	inline Voxel *operator () (int i, int j, int k) const
	{
		return m_data[i + (BRICK_DIM * j) + (BRICK_DIM * BRICK_DIM * k)];
	}

	inline Voxel *get(int i, int j, int k) const
	{
		return m_data[i + (BRICK_DIM * j) + (BRICK_DIM * BRICK_DIM * k)];
	}

private:
	Voxel *m_data[BRICK_DIM * BRICK_DIM * BRICK_DIM];
};

class BrickGrid {
public:
	BrickGrid(){};
	BrickGrid(int size_x, int size_y, int size_z);
        BrickGrid(int size_x, int size_y, int size_z, float fill);

	inline Voxel *operator () (int i, int j, int k) const {

            if(i < 0 || j < 0 || k < 0 || i >= m_size_x || j >= m_size_y || k >= m_size_z)
                return m_zero_voxel;

            const int x = i / BRICK_DIM;
            const int y = j / BRICK_DIM;
            const int z = k / BRICK_DIM;

            const int off_x = i % BRICK_DIM;
            const int off_y = j % BRICK_DIM;
            const int off_z = k % BRICK_DIM;

            const Brick* b = m_data[x + (m_brick_size_y * y) + (m_brick_size_y * m_brick_size_z * z)];

            return (*b)(off_x, off_y, off_z);
        }

        inline Brick *brick(int i, int j, int k) const{
            if(i < 0 || j < 0 || k < 0 || i >= m_brick_size_x || j >= m_brick_size_y || k >= m_brick_size_z)
                return NULL;

            return m_data[i + (m_brick_size_y * j) + (m_brick_size_y * m_brick_size_z * k)];
        }

        inline void brick(Brick *brick, int i, int j, int k) const{
            if(i < 0 || j < 0 || k < 0 || i >= m_brick_size_x || j >= m_brick_size_y || k >= m_brick_size_z) {
                printf("Error: no such brick (%d, %d, %d)\n", i, j, k);
                exit(1);
            }
            free(m_data[i + (m_brick_size_y * j) + (m_brick_size_y * m_brick_size_z * k)]);
            m_data[i + (m_brick_size_y * j) + (m_brick_size_y * m_brick_size_z * k)] = brick;
        }

        inline int brickCount() const {
            return m_brick_size_x * m_brick_size_y * m_brick_size_z;
        }

        inline Voxel *get(int i, int j, int k);

	virtual ~BrickGrid();

	inline int size_x() const { return m_size_x; }
	inline int size_y() const { return m_size_y; }
	inline int size_z() const { return m_size_z; }

	inline int brick_size_x() const { return m_brick_size_x; }
	inline int brick_size_y() const { return m_brick_size_y; }
	inline int brick_size_z() const { return m_brick_size_z; }
        void init(int size_x, int size_y, int size_z);
        void fill(int offset, float value);

private:

    Voxel *m_zero_voxel;
    Brick **m_data;
    int m_size_x, m_size_y, m_size_z;
    int m_brick_size_x, m_brick_size_y, m_brick_size_z;

};

#endif /* BRICK_H_ */
