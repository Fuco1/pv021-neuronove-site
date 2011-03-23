#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <iostream>
#include <vector>

class Image {
	private:
		int width;
		int height;
		int depth;

		std::vector<int> data;
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

		int getVoxel(int x, int y, int z) {
			return data[x + getWidth() * y + getWidth() * getHeight() * z];
		}

		// \todo void loadFromFile(const std::string &fileName);
};

#endif // IMAGE_H_INCLUDED
