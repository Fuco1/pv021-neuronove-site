#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <iostream>
#include <vector>

template <class VoxelType> class Image {
	private:
		int width;
		int height;
		int depth;

		std::vector<VoxelType> data;
	public:
		int getWidth(void) {
			return width;
		};
		int getHeight(void) {
			return height;
		};
		int getDepth(void) {
			return depth;
		};

		// Image constructor.
		Image(const std::string &fileName) {
			loadFromFile(fileName);
		}

		// Read the value of a voxel with the specific coords.
		VoxelType getVoxel(int x, int y, int z) {
			return data[x + getWidth() * y + getWidth() * getHeight() * z];
		}

		// \todo Load the image from a fie.
		void loadFromFile(const std::string &fileName) {			
			width = 10;  // \todo Read the image width.
			height = 10; // \todo Read the image height.
			depth = 10;  // \todo Read the image depth.

			data.resize(width * height * depth);

			for (size_t i = 0; i < data.size(); ++i) {				
				data[i] = 0; // \todo Read the value of the i-th voxel.
			}
		}

};

#endif // IMAGE_H_INCLUDED
