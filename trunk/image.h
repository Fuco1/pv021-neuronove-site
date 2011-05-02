#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include "net.h"

//DataItem is defined in net.h
template <class DataType> class DataItem; 

template <class VoxelType> class Image: public DataItem<VoxelType> {
	private:
		size_t width;
		size_t height;
		size_t depth;

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

		// Parametric constructor.
		Image(size_t aWidth, size_t aHeight, size_t aDepth) {
			width = aWidth;
			height = aHeight;
			depth = aDepth;
			this->data.resize(width * height * depth);
		}

		// Load from file constructor.
		Image(const std::string &fileName) {
			loadFromFile(fileName);
		}

		// Get the value of a voxel with the specific index.
		VoxelType getVoxel(size_t i) const {
			//return data[i];
			return this->getData(i);
		}

		// Get the value of a voxel with the specific coords.
		VoxelType getVoxel(size_t x, size_t y, size_t z) const {
			//return data[x + getWidth() * y + getWidth() * getHeight() * z];
			return getData(x + getWidth() * y + getWidth() * getHeight() * z);
		}

		// Set the value of a voxel with the specific index.
		void setVoxel(size_t i, VoxelType value) {
			//data[i] = value;
			setData(i, value);
		}

		// Set the value of a voxel with the specific coords.
		void setVoxel(size_t x, size_t y, size_t z, VoxelType value) {
			//data[x + getWidth() * y + getWidth() * getHeight() * z] = value;
			setData(x + getWidth() * y + getWidth() * getHeight() * z, value);
		}

		// \todo Load the image from a file.
		void loadFromFile(const std::string &fileName) {
			std::ifstream input(fileName.c_str());
			if (input.fail()) {
				std::cerr << "Error: Could not read " << fileName << std::endl;
				exit(1);
			}
			this->fileName = fileName;

			input >> this->width;
			input >> this->height;
			input >> this->depth;
			this->data.resize(width * height * depth);

			for (size_t i = 0; i < this->data.size(); ++i) {
				VoxelType voxel;
				input >> voxel;
				setVoxel(i, voxel);
			}

			input.close();

			/* \todo Remove this as soon as the proper image loading (above) is tested.
			width = 10;  // \todo Read the image width.
			height = 10; // \todo Read the image height.
			depth = 10;  // \todo Read the image depth.

			this->data.resize(width * height * depth);

			for (size_t i = 0; i < this->data.size(); ++i) {
				setVoxel[i] = 0.1; // \todo Read the value of the i-th voxel.
			}
			*/
		}

};

#endif // IMAGE_H_INCLUDED
