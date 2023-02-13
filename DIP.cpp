#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<windows.h>
#include<cstring>

class bmp {
public:
	bmp(void) = default;
	~bmp() {
		delete[] path;
		printf("has delete path");
	}
	bmp(const char* in_path) {
		path = new char[strlen(in_path)+1];
		memcpy(path, in_path, sizeof(char)* (strlen(in_path) + 1));
	}
	char* get_path() { return path; }
private:
	char* path;
};

int main() {
	bmp b1("E:\\Graduation design");
	printf("%s ", b1.get_path());
	return 0;
}