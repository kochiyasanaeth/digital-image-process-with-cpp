#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<windows.h>
#include<wingdi.h>
#include<cstring>
#include<assert.h>
using namespace std;
unsigned int trans16b_2_10n(uint8_t _buff_[], size_t len) {
	unsigned int  ret = 0,mul = 1;
	for (size_t i = 0; i < len; i++) {
		ret += _buff_[i] * mul;
		mul *= 256;
	}
	return ret;
}
class graph {
public:
	graph() = delete;
	graph(size_t _w, size_t _h) : w(_w), h(_h) { 
		img = new uint32_t*[h];
		for (size_t i = 0; i < h; i++) {
			img[i] = new uint32_t[w];
			memset(img[i], 0, sizeof(uint32_t) * w);
		}
	}	
	size_t get_w() {
		return w;
	}
	size_t get_h() {
		return h;
	}
	uint32_t** get_grh() {
		return img;
	}
private:
	size_t w;
	size_t h;
	uint32_t** img = NULL;
};
class bmp24 {
public:
	bmp24(void) = default;
	~bmp24() {
		delete[] path;
		printf("has delete path");
	}
	bmp24(const char* in_path) {
		path = new char[strlen(in_path)+1];
		memcpy(path, in_path, sizeof(char)* (strlen(in_path) + 1));
		img = fopen(path, "r");
		if (img == NULL) {
			return;
		};
		uint8_t bmp_h[2] = { 0,0 };
		fread(bmp_h, sizeof(uint8_t), 2, img);
		if(bmp_h[0] != 0x42 || bmp_h[1] != 0x4d) {
			printf("file not bmp");
			fclose(img);
			return;
		}
		bmpFlag = true;
		fclose(img);
	}
	bmp24& load(void) {
		if (!bmpFlag) {
			printf("bmp open fail");
			return *this;
		}
		img = fopen(path, "r");
		if (img == NULL) {
			printf("bmp open fail");
			return *this;
		};
		uint8_t info[4] = { 0,0,0,0 };
		if (fseek(img, 2, SEEK_SET)) {
			printf("seek bmp fail");
			return *this;
		}
		if (fread(info, sizeof(uint8_t), 4, img)) {
			bmp_size = trans16b_2_10n(info,4);
		}
		size_t graph_w = 0, graph_h = 0;
		if (fseek(img, 18, SEEK_SET)) {
			printf("seek bmp fail");
			return *this;
		}
		if (fread(info, sizeof(uint8_t), 4, img)) {
			graph_w = trans16b_2_10n(info, 4);
		}
		if (fseek(img, 22, SEEK_SET)) {
			printf("seek bmp fail");
			return *this;
		}
		if (fread(info, sizeof(uint8_t), 4, img)) {
			graph_h = trans16b_2_10n(info, 4);
		}
		gr = new graph(graph_w, graph_h);
		if (fseek(img, 10, SEEK_SET)) {
			printf("seek bmp fail");
			return *this;
		}
		size_t bp;
		if (fread(info, sizeof(uint8_t), 4, img)) {
			 bp = trans16b_2_10n(info, 4);
		}
		printf("log");
		uint32_t data = 0;
		if (bp != 0) {
			fseek(img, bp, SEEK_SET);
			for (size_t high = 0; high < gr->get_h(); high++) {
				for (size_t weight = 0; weight < gr->get_w(); weight++) {
					fread(&(gr->get_grh()[gr->get_h() - high - 1][weight]), sizeof(uint8_t), 3, img);
				}
				if (gr->get_h() * 3 % 4) {
					bp = 4 - ((gr->get_h() * 3) % 4);
					fseek(img, bp, SEEK_CUR);
				}
			}
			for (size_t i = 0; i < gr->get_h(); i++) {
				for (size_t j = 0; j < gr->get_w(); j++) {
					printf("%x ", gr->get_grh()[gr->get_h() - i-1][j]);
				}
				printf("\n");
			}
		}
		return *this;
	}
	size_t get_size(void) { return bmp_size; }
	size_t get_w(void) { if (gr != NULL) return gr->get_w(); return 0; }
	size_t get_h(void) { if (gr != NULL) return gr->get_h(); return 0; }
	char* get_path() { return path; }
private:
	char* path;
	FILE* img;
	bool bmpFlag = false;
	size_t bmp_size = 0;
	graph *gr;
};



int main() {
	bmp24 b1("t1.bmp");
	printf("%s \n", b1.get_path());
	b1.load();
	printf("%u\n", b1.get_size());
	printf("%u %u\n", b1.get_h(), b1.get_w());
	system("pause"); 
	return 0;
}
