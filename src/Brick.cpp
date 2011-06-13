#include "Brick.h"
#include <math.h>

Brick::Brick() {

	for(int i = 0; i < BRICK_DIM * BRICK_DIM * BRICK_DIM; i++) {
		//m_data[i] = new Voxel(4, 0.0);
		m_data[i] = new Voxel(4);
	}
}

Brick::~Brick() {
	// TODO Auto-generated destructor stub
}

BrickGrid::BrickGrid(int size_x, int size_y, int size_z) {

    init(size_x, size_y, size_z);
}

BrickGrid::BrickGrid(int size_x, int size_y, int size_z, float val) {

    init(size_x, size_y, size_z);
    fill(0, val);
}

void
BrickGrid::init(int size_x, int size_y, int size_z)
{
    m_size_x = size_x;
    m_size_y = size_y;
    m_size_z = size_z;

    m_brick_size_x = ceil(size_x / (float)BRICK_DIM);
    m_brick_size_y = ceil(size_y / (float)BRICK_DIM);
    m_brick_size_z = ceil(size_z / (float)BRICK_DIM);

    int size = m_brick_size_x * m_brick_size_y * m_brick_size_z;

    m_data = new Brick*[size];

    for(int i = 0; i < size; i++) {
            m_data[i] = new Brick();
    }

    m_zero_voxel = new Voxel(4);
}

void
BrickGrid::fill(int offset, float value)
{
    for(int i = 0; i < m_size_x; i++) {
        for(int j = 0; j < m_size_y; j++) {
            for(int k = 0; k < m_size_z; k++) {
                get(i,j,k)->set(offset, value);
            }
        }
    }
}

BrickGrid::~BrickGrid() {

}

inline Voxel*
BrickGrid::get(int i, int j, int k) {

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