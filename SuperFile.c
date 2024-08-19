#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "General.h"
#include "fileHelper.h"
#include "SuperFile.h"
#include "myMacros.h"


int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, int bCompress)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open supermarket file to write\n");
		return 0;
	}

	int prodCount = getNumOfProductsInList(pMarket);

	int ok;
	if (bCompress)
		ok = saveCompressSuperData(pMarket, fp, prodCount);
	else
		ok = saveUnCompressSuperData(pMarket, fp, prodCount);
	

	if (!ok)
	{
		fclose(fp);
		return 0;
	}

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		int res;
		if (bCompress)
			res = saveCompressProductToFile(pTemp, fp);
		else
			res = saveProductToFile(pTemp, fp);
		if (!res)

		if (!res)
			CLOSE_RETURN_0(fp);
		pN = pN->next;
	}
	fclose(fp);

	saveCustomerToTextFile(pMarket->customerArr,pMarket->customerCount, customersFileName);

	return 1;
}


int saveUnCompressSuperData(const SuperMarket* pMarket, FILE* fp,int prodCount)
{
	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n",1))
	{
		fclose(fp);
		return 0;
	}

	if (!saveAddressToFile(&pMarket->location, fp))
	{
		fclose(fp);
		return 0;
	}


	if (!writeIntToFile(prodCount, fp, "Error write product count\n"))
	{
		fclose(fp);
		return 0;
	}

	return 1;
}

int		saveCompressSuperData(const SuperMarket* pMarket, FILE* fp,int prodCount)
{
	BYTE data[2];

	int len = (int)strlen(pMarket->name);

	data[0] = prodCount >> 2;
	data[1] = (prodCount & 0x3) << 6 | (len & 0x3F);

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	if (fwrite(pMarket->name, sizeof(char), len, fp) != len)
		return 0;

	if (!saveCompressAddressToFile(&pMarket->location, fp))
		return 0;

	return 1;
}


int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName,int bCompress)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	int ok;
	int productCount;
	if (bCompress)
		ok = readCompressSuperData(pMarket, fp, &productCount);
	else
		ok = readUnCompressSuperData(pMarket, fp,&productCount);
	if (!ok)
	{
		fclose(fp);
		return 0;
	}

	if (productCount > 0)
	{
		Product* pTemp;
		for (int i = 0; i < productCount; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			int res;
			if (bCompress)
				res = loadCompressProductFromFile(pTemp, fp);
			else
				res = loadProductFromFile(pTemp, fp);
			if (!res)
			{
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
		}
	}

	fclose(fp);
	if (!ok)
		return 0;

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName,&pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;
}

int		readUnCompressSuperData(SuperMarket* pMarket, FILE* fp,int* prodCount)
{
	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n", 1);
	if (!pMarket->name)
		CLOSE_RETURN_0(fp);

	if (!loadAddressFromFile(&pMarket->location, fp))
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);

	if (!readIntFromFile(prodCount, fp, "Error reading product count\n"))
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	return 1;
}

int		readCompressSuperData(SuperMarket* pMarket, FILE* fp, int* prodCount)
{
	BYTE data[2];

	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	*prodCount = data[0] << 2 | (data[1] >> 6) & 0x3;
	int len = data[1] & 0x3F;

	pMarket->name = (char*)calloc(len + 1, sizeof(char));
	CHECK_NULL_CLOSE_FILE_RETURN_0(pMarket->name, fp);

	if (fread(pMarket->name, sizeof(char), len, fp) != len)
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);

	if (!loadCompressAddressFromFile(&pMarket->location, fp))
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);

	return 1;
}

int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName)
{
	FILE* fp;
	//L_init(&pMarket->productList);
	fp = fopen(fileName, "r");
	int count;
	fscanf(fp, "%d\n", &count);


	//Product p;
	Product* pTemp;
	for (int i = 0; i < count; i++)
	{
		pTemp = (Product*)calloc(1, sizeof(Product));
		myGets(pTemp->name, sizeof(pTemp->name), fp);
		myGets(pTemp->barcode, sizeof(pTemp->barcode), fp);
		fscanf(fp, "%d %f %d\n", &pTemp->type, &pTemp->price, &pTemp->count);
		insertNewProductToList(&pMarket->productList, pTemp);
	}

	fclose(fp);
	return 1;
}