#include <gdefines.h>

#include "HuffmanCoder.h"


/*    IN sour = uncompressed code usize BYTES
OUT dest = compressed code csize BYTES     */
//////////////////////////////////////////////////////////////////////////////////////////////////
void Huffman::encode(unsigned char *dest, int &csize, unsigned char *sour, int usize)
{
	pheader = (PHUFFHDR)dest;     //huff header
	psour = sour;                //sour buffer
	pdest = dest;                //dest buffer
	filesize = usize;            //uncompressed file size
	csize = 0;                   //fault protection

	symnum = 0;
	bitslast = 8;


	storetree();                 //get freqs and store headers
	buildtree();                 //build htree
	buildbook();                 //build codebook
	for(int i = 0; i < filesize; i++)
		writesymb(*psour++);


	csize = int((pdest - dest) + 1);
	pheader->cmpsize = csize;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/*    IN sour = compressed huffman frame
OUT dest = uncompressed code usize BYTES     */
//////////////////////////////////////////////////////////////////////////////////////////////////
void Huffman::decode(unsigned char *dest, int &usize, unsigned char *sour)
{
	pheader = (PHUFFHDR)sour;     //huff header
	psour = sour;                //sour buffer
	pdest = dest;                //dest buffer
	usize = 0;                   //fault protection

	bitslast = 8;


	readtree();                  //read headers and get freqs
	buildtree();                 //build tree
	while(filesize)
	{
		*pdest++ = readsymb();
		filesize--;
	}


	usize = int(pdest - dest);
}
//////////////////////////////////////////////////////////////////////////////////////////////////



/*                           HUFF ENCODING ROUTINES                              */

/*         init tree buffers
fill headers                             */
void Huffman::storetree(void)
{
	/*               memory init                           */
	memset(tmass1, 0, 256 * sizeof(hufftree));
	memset(tmass2, 0, 256 * sizeof(hufftree));
	memset(smass, 0, 256 * sizeof(huffsymbol));

	temptree = (PHTREE)tmass1;
	htree = (PHTREE)tmass2;
	hsymbol = (PSYMB)smass;

	for(int i = 0; i < 256; i++) temptree[i].s1 = i;               //initilize symbols
	for(int i = 0; i < filesize; i++) temptree[psour[i]].freq++;   //get symb frequencies
	for(int i = 0; i < 256; i++)
		if(temptree[i].freq) symnum++;        //get total symbols

	qsort((void *)temptree, 256, sizeof(hufftree), &compare);      //sort temptree


	/*               headers init                           */
	//memcpy(pheader->magic, "HUFF", 4);
	pheader->symnum = symnum;
	pheader->uncmpsize = filesize;
	pheader->cmpsize = 0;

	pdest += sizeof(huffheader);

	/*   make [  freq  ][C] pairs    */
	for(int i = 0; i < symnum; i++)
	{
		psheader = (PSYMBHDR)pdest;
		psheader->symb = temptree[i].s1;
		psheader->freq = temptree[i].freq;

		pdest += 5;     //sizeof(PSYMBFRQ)
	}
	*pdest = 0;
}

/*         build htree from temptree                */
void Huffman::buildtree()
{
	/*        special case symnum = 1   */
	if(symnum == 1)
	{
		hsymbol[psour[0]].size = 1;
		return;
	}
	/*        build 1 & 2 trees        */
	for(int i = symnum - 2; i >= 0; i--)
	{
		if(temptree[i].parent)
		{                    //if it is compound node yet
			htree[i].pleft = temptree[i].parent;        //to another node 0
			htree[i].pleft->parent = &htree[i];
		}
		else
			htree[i].s1 = temptree[i].s1;               //to final symbol 0

		if(temptree[i + 1].parent)
		{
			htree[i].pright = temptree[i + 1].parent;     //to another node 1
			htree[i].pright->parent = &htree[i];
		}
		else
			htree[i].s2 = temptree[i + 1].s1;	           //to final symbol 1

		if(i)
		{
			temptree[i].freq += temptree[i + 1].freq;     //[i]+[i+1] frequencies for new [i] node
			temptree[i].parent = &htree[i];             //[i] becomes a node
			temptree[i + 1].freq = 0;                     //[i+1] destroyed

			qsort((void *)temptree, (i + 1), sizeof(hufftree), &compare);
		}
	}
}

/*         build codebook from htree        */
void Huffman::buildbook()
{
	unsigned char c;
	PHTREE parent, child;

	/*     build symbols from htree    */
	for(int i = symnum - 2; i >= 0; i--)
	{
		if(!htree[i].pleft)
		{            //0bit symbol
			c = htree[i].s1;
			hsymbol[c].size++;

			child = &htree[i];
			parent = child->parent;
			while(parent)
			{
				if(parent->pleft == child)              //add 0 to symb code
					hsymbol[c].size++;
				if(parent->pright == child)		       //add 1 to symb code
					hsymbol[c].symb |= __int64(1 << hsymbol[c].size++);

				child = parent;                    //child becomes a parent
				parent = child->parent;            //parent becomes 'childs' parent
			}
		}
		if(!htree[i].pright)
		{           //1bit symbol
			c = htree[i].s2;
			hsymbol[c].symb |= __int64(1 << hsymbol[c].size++);

			child = &htree[i];
			parent = child->parent;
			while(parent)
			{
				if(parent->pleft == child)              //add 0 to symb code
					hsymbol[c].size++;
				if(parent->pright == child)             //add 1 to symb code
					hsymbol[c].symb |= __int64(1 << hsymbol[c].size++);

				child = parent;                    //child becomes a parent
				parent = child->parent;            //parent becomes 'childs' parent
			}
		}
	}
}

/*         write codebook symbol to dest    */
void Huffman::writesymb(int c)
{
	for(unsigned int i = 1; i <= hsymbol[c].size; i++)
	{
		*pdest |= ((0x1 & (hsymbol[c].symb >> (hsymbol[c].size - i))) << (--bitslast));

		if(bitslast == 0)
		{
			*(++pdest) = 0;
			bitslast = 8;
		}
	}
}




/*                           HUFF DECODING ROUTINES                              */

/*         read huff headers and get freqs        */
void Huffman::readtree()
{
	/*               memory init                    */
	memset(tmass1, 0, 256 * sizeof(hufftree));
	memset(tmass2, 0, 256 * sizeof(hufftree));

	temptree = (PHTREE)tmass1;
	htree = (PHTREE)tmass2;

	filesize = pheader->uncmpsize;
	symnum = pheader->symnum;
	psour += sizeof(huffheader);

	/*       get freqs      */
	for(int i = 0; i < symnum; i++)
	{
		psheader = (PSYMBHDR)psour;
		temptree[i].freq = psheader->freq;
		temptree[i].s1 = psheader->symb;

		psour += 5;      //sizeof(PSYMBFRQ)
	}
}

/*         decode huff symbol by bit from sour        */
unsigned char Huffman::readsymb()
{
	PHTREE node = &htree[0];

	while(1)
	{
		if(getnextbit())
		{           //next right node
			if(node->pright)
				node = node->pright;
			else
				return node->s2;
		}
		else
		{              //next left node
			if(node->pleft)
				node = node->pleft;
			else
				return node->s1;
		}
	}
}


/*         compare for qsort()              */
int __cdecl Huffman::compare(const void *a, const void *b)
{
	PHTREE t1 = (PHTREE)a;
	PHTREE t2 = (PHTREE)b;

	if(t1->freq == t2->freq) return 0;
	else return ((t1->freq < t2->freq) ? 1 : -1);
}



unsigned int HuffmanCoder::ComputeDecompressedLength(unsigned char * pData, unsigned int iLen)
{
	PHUFFHDR pheader = (PHUFFHDR)pData;
	return(pheader->uncmpsize);
}


void HuffmanCoder::Pack(unsigned char * pData, unsigned int iLen, unsigned char ** pOut, unsigned int * pOutLen)
{
	Huffman coder;
	int i;
	*pOut = new unsigned char[iLen];
	coder.encode(*pOut, i, pData, iLen);
	*pOutLen = i;
}

void HuffmanCoder::UnPack(unsigned char * pData, unsigned int iLen, unsigned char ** pOut, unsigned int * pOutLen)
{
	Huffman coder;
	*pOutLen = ComputeDecompressedLength(pData, iLen);
	*pOut = new unsigned char[*pOutLen];
	int i;
	coder.decode(*pOut, i, pData);
}
