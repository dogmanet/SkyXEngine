#ifndef _HuffmanCoder_H_
#define _HuffmanCoder_H_


/*       huffman trees            */
typedef struct hufftree
{
	unsigned int freq;       //s1 freq
	unsigned char s1;        //0...255:codes       left symb
	unsigned char s2;        //                    right symb
	hufftree* pleft;         //0
	hufftree* pright;        //1
	hufftree* parent;        //parent node
} *PHTREE;

typedef struct huffsymbol
{
	unsigned int size;       //symbol size in bits
	__int64 symb;            //max 256 bits
} *PSYMB;


/*       huffman file headers            */
typedef struct huffheader
{
	//char magic[4];           //'HUFF'
	int symnum;              //total symbnum
	int uncmpsize;           //uncompressed file size
	int cmpsize;             //compressed file size
} *PHUFFHDR;

typedef struct symbheader
{
	unsigned int freq;       //its freq
	unsigned char symb;      //symbol
} *PSYMBHDR;



class Huffman
{
public:
	Huffman()
	{
	};
	//Huffman(const Huffman& huffman);
	//~Huffman();

	// Operators
	//const Huffman& operator=(const Huffman& huffman);

	// Operations
	void encode(unsigned char *dest, int &csize, unsigned char *sour, int usize);
	void decode(unsigned char *dest, int &usize, unsigned char *sour);

	// Access
	// Inquiry
	inline int get_uncompressed_size(unsigned char *sour);

protected:
private:
	Huffman(const Huffman& huffman);
	const Huffman& operator=(const Huffman& huffman);


	PHTREE temptree;                              //tmass1 first tree
	PHTREE htree;                                 //tmass2 second huff tree
	PSYMB hsymbol;                                //smass symbols codebook

	unsigned char tmass1[256 * sizeof(hufftree)];             //first tree with freq codes
	unsigned char tmass2[256 * sizeof(hufftree)];             //last built tree
	unsigned char smass[256 * sizeof(huffsymbol)];            //symbols codebook buffer [code][size] pairs

	unsigned char *psour;                                  //source pointer
	unsigned char *pdest;                                  //dest pointer

	PHUFFHDR pheader;                              //huffman file header
	PSYMBHDR psheader;                             //[ freq ][C] pairs

	int filesize;                                 //uncompressed file size
	int symnum;                                   //total different symbols
	int bitslast;


	static int __cdecl compare(const void *a, const void *b);    //for sorting routine

	void storetree(void);                         //memory and huff headers init
	void buildtree(void);                         //htree building
	void buildbook(void);                         //build codebook [code][size] pairs
	void writesymb(int c);                        //write codebook symbol

	void readtree(void);                          //read headers and get freqs
	unsigned char readsymb();                     //read symbol from htree
	inline int getnextbit();                      //get bits from sour

};

inline int Huffman::get_uncompressed_size(unsigned char *sour)
{
	pheader = (PHUFFHDR)sour;
	return(pheader->uncmpsize);
}

/*             get bits from sour             */
inline int Huffman::getnextbit()
{
	if(bitslast)
	{
		return(0x1 & (*psour >> (--bitslast)));
	}
	else
	{
		psour++;
		bitslast = 8;
		return(0x1 & (*psour >> (--bitslast)));
	}
}

class HuffmanCoder
{
	static unsigned int ComputeDecompressedLength(unsigned char * pData, unsigned int iLen);
public:
	static void Pack(unsigned char * pData, unsigned int iLen, unsigned char ** pOut, unsigned int * pOutLen);
	static void UnPack(unsigned char * pData, unsigned int iLen, unsigned char ** pOut, unsigned int * pOutLen);
};

#endif
