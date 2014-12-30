#ifndef JML_CARTIDGE_BUILDER
#define JML_CARTIDGE_BUILDER

#include "Cartidge.h"

class CartidgeBuilder
{
public:
	CartidgeBuilder();
	Cartidge* Build(const char* path);

private:
	char* GetHeader(FILE* file);
	bool CheckLogoIntegrity(const char* header);
	bool ReadCartidgeMetadata(const char* header);
	Cartidge* GetCorrectCartidge(const char* header);
	bool SetRomSize(const char* header, Cartidge* cartidge);
	bool SetRamSize(const char* header, Cartidge* cartidge);
	bool GetCBGFlag(const char* header, Cartidge* cartidge);
	bool GetSBGFlag(const char* header, Cartidge* cartidge);

};
#endif //JML_CARTIDGE_BUILDER