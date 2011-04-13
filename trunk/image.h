#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <iostream>
#include <vector>

template <class VoxelType> class Image {
	private:
		size_t width;
		size_t height;
		size_t depth;

		std::vector<VoxelType> data;
	public:
		size_t getWidth(void) const {
			return width;
		};
		size_t getHeight(void) const {
			return height;
		};
		size_t getDepth(void) const {
			return depth;
		};
		size_t getSize(void) const {
			return width * height * depth;
		};

		// Parametric constructor.
		Image(size_t aWidth, size_t aHeight, size_t aDepth) {
			width = aWidth;
			height = aHeight;
			depth = aDepth;
			data.resize(width * height * depth);
		}

		// Load from file constructor.
		Image(const std::string &fileName) {
			loadFromFile(fileName);
		}

		// Get the value of a voxel with the specific index.
		VoxelType getVoxel(size_t i) const {
			return data[i];
		}

		// Get the value of a voxel with the specific coords.
		VoxelType getVoxel(size_t x, size_t y, size_t z) const {
			return data[x + getWidth() * y + getWidth() * getHeight() * z];
		}

		// Set the value of a voxel with the specific index.
		void setVoxel(size_t i, VoxelType value) {
			data[i] = value;
		}

		// Set the value of a voxel with the specific coords.
		void setVoxel(size_t x, size_t y, size_t z, VoxelType value) {
			data[x + getWidth() * y + getWidth() * getHeight() * z] = value;
		}


		// \todo Load the image from a fie.
		void loadFromFile(const std::string &fileName) {
			width = 10;  // \todo Read the image width.
			height = 10; // \todo Read the image height.
			depth = 10;  // \todo Read the image depth.

			data.resize(width * height * depth);

			for (size_t i = 0; i < data.size(); ++i) {
				data[i] = 0.1; // \todo Read the value of the i-th voxel.
			}
		}

};

#endif // IMAGE_H_INCLUDED
