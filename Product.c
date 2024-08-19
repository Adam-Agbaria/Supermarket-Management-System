#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"
#include "fileHelper.h"


#define MIN_DIG 3
#define MAX_DIG 5

void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	getBorcdeCode(pProduct->barcode);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH );
		myGets(pProduct->name, sizeof(pProduct->name),stdin);
	} while (checkEmptyString(pProduct->name));
}

void	printProduct(const Product* pProduct)
{
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %10d\n", typeStr[pProduct->type], pProduct->price, pProduct->count);
}

int		saveProductToFile(const Product* pProduct, FILE* fp)
{
	if (fwrite(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error saving product to file\n");
		return 0;
	}
	return 1;
}

int		saveCompressProductToFile(const Product* pProduct, FILE* fp)
{
	BYTE data[3] = { 0 };
	int barcodeCompressCode[BARCODE_LENGTH];

	int len = (int)strlen(pProduct->name);
	if (len > 15)
		len = 15;
	createBarcodeCompressCode(pProduct->barcode, BARCODE_LENGTH, barcodeCompressCode);



	data[0] = ((barcodeCompressCode[0] & 0x3F) << 2) | ((barcodeCompressCode[1] >> 4) & 0x3);
	data[1] = ((barcodeCompressCode[1] & 0xF) << 4) | ((barcodeCompressCode[2] >> 2) & 0xF);
	data[2] = ((barcodeCompressCode[2] & 0x3) << 6) | (barcodeCompressCode[3] & 0x3F);

	if (fwrite(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;


	data[0] = ((barcodeCompressCode[4] & 0x3F) << 2) | ((barcodeCompressCode[5] >> 4) & 0x3);
	data[1] = ((barcodeCompressCode[5] & 0xF) << 4) | ((barcodeCompressCode[6] >> 2) & 0xF);
	data[2] = ((barcodeCompressCode[6]& 0x3) << 6) | (len << 2) | pProduct->type;

	if (fwrite(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;


	if (fwrite(pProduct->name, sizeof(char), len, fp) != len)
		return 0;


	int priceShekels = (int)pProduct->price;
	int priceAgorots = (int)((pProduct->price - priceShekels) * 100);

	data[0] = (BYTE)pProduct->count;	
	data[1] = (priceAgorots << 1) | (priceShekels >> 8);
	data[2] = (priceShekels & 0xFF);
	if (fwrite(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	return 1;
}



int		loadProductFromFile(Product* pProduct, FILE* fp)
{
	if (fread(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error reading product from file\n");
		return 0;
	}
	
	return 1;
}

int		loadCompressProductFromFile(Product* pProduct, FILE* fp)
{
	BYTE data[3];

	if (fread(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	int barcodeCompress[BARCODE_LENGTH];
	barcodeCompress[0] = (data[0] >> 2) & 0x3F;
	barcodeCompress[1] = ((data[0] & 0x3) << 4) | ((data[1] >> 4) & 0xF);
	barcodeCompress[2] = ((data[1] & 0xF) << 2) | ((data[2] >> 6) & 0x3);
	barcodeCompress[3] = data[2] & 0x3F;

	if (fread(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	barcodeCompress[4] = (data[0] >> 2) & 0x3F;
	barcodeCompress[5] = ((data[0] & 0x3) << 4) | ((data[1] >> 4) & 0xF);
	barcodeCompress[6] = ((data[1] & 0xF) << 2) | ((data[2] >> 6) & 0x3);

	pProduct->type = data[2] & 0x3;
	int len = (data[2] >> 2) & 0xF;

	if (fread(pProduct->name, sizeof(char), len, fp) != len)
		return 0;
	pProduct->name[len] = '\0';

	initBarcodeFromCompressInt(pProduct->barcode, BARCODE_LENGTH, barcodeCompress);

	if (fread(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	pProduct->count = data[0];
	int priceAgorots = (data[1] >>1) & 0x7F;
	int priceShekels = ((data[1]& 0x1) << 8) | data[2];
	pProduct->price = priceShekels + priceAgorots / 100.0F;
	return 1;
}

void initBarcodeFromCompressInt(char* barcodeArr, int size, int* codes)
{
	for (int i = 0; i < size; i++)
	{
		if (codes[i] >= 0 && codes[i] <= 9)
			barcodeArr[i] = codes[i] + '0';
		else
		{
			barcodeArr[i] = codes[i] - 10 + 'A';
		}
	}
}

void createBarcodeCompressCode(const char* barcodeArr, int size, int* codes)
{
	for (int i = 0; i < size; i++)
	{
		if (isdigit(barcodeArr[i]))
			codes[i] = barcodeArr[i] - '0';
		else
		{
			codes[i] = barcodeArr[i] + 10 - 'A';
		}
	}
}

void getBorcdeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg,"Code should be of %d length exactly\n"
				"UPPER CASE letter and digits\n"
				"Must have %d to %d digits\n"
				"First and last chars must be UPPER CASE letter\n"
				"For example A12B40C\n",
				BARCODE_LENGTH, MIN_DIG, MAX_DIG);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		printf("Enter product barcode "); 
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts(msg);
			ok = 0;
		}
		else {
			//check and first upper letters
			if(!isupper(temp[0]) || !isupper(temp[BARCODE_LENGTH-1]))
			{
				puts("First and last must be upper case letters\n");
				ok = 0;
			} else {
				for (int i = 1; i < BARCODE_LENGTH - 1; i++)
				{
					if (!isupper(temp[i]) && !isdigit(temp[i]))
					{
						puts("Only upper letters and digits\n");
						ok = 0;
						break;
					}
					if (isdigit(temp[i]))
						digCount++;
				}
				if (digCount < MIN_DIG || digCount > MAX_DIG)
				{
					puts("Incorrect number of digits\n");
					ok = 0;
				}
			}
		}
		
	} while (!ok);

	strcpy(code, temp);
}


eProductType getProductType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);
	getchar();
	return (eProductType)option;
}

const char* getProductTypeStr(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

int		isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

int		compareProductByBarcode(const void* var1, const void* var2)
{
	const Product* pProd1 = (const Product*)var1;
	const Product* pProd2 = (const Product*)var2;

	return strcmp(pProd1->barcode, pProd2->barcode);
}


void	updateProductCount(Product* pProduct)
{
	int count;
	do {
		printf("How many items to add to stock?");
		scanf("%d", &count);
	} while (count < 1);
	pProduct->count += count;
}


void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}