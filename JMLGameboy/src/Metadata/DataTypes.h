#ifndef JML_DATATYPES
#define JML_DATATYPES

typedef unsigned short	WORD;	//16bit
typedef unsigned char	BYTE;		//8Bit

union DREG
{
	WORD w;

	struct
	{
		BYTE l;
		BYTE h;
	};
};

#endif //JML_DATATYPES