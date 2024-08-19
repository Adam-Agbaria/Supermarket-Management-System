#pragma once

#include <stdio.h>
#include "Supermarket.h"

int		saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
			const char* customersFileName,int bCompress);
int		saveUnCompressSuperData(const SuperMarket* pMarket, FILE* fp,int prodCount);
int		saveCompressSuperData(const SuperMarket* pMarket, FILE* fp,int prodCount);

int		loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
			const char* customersFileName, int bCompress);
int		readUnCompressSuperData(SuperMarket* pMarket, FILE* fp, int* prodCount);
int		readCompressSuperData(SuperMarket* pMarket, FILE* fp, int* prodCount);

//int		loadCustomerFromTextFile(SuperMarket* pMarket, const char* customersFileName);


int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName);
