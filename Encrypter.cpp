#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xxtea.h"

#define SIZE 1024*1024*50

void Decrypt(const char* sourceFilePath, const char* destFilePath, const char* key)
{
	printf("sourceFile:%s\n", sourceFilePath);
	printf("destFile:%s\n", destFilePath);
	printf("key:%s\n", key);

	FILE* infp = 0;
	if (0 != fopen_s(&infp, sourceFilePath, "rb"))
	{
		printf("Read file Error:%s\n", sourceFilePath);
		return;
	}

	char* buffer = (char*)malloc(sizeof(char) * SIZE);
	memset(buffer, 0, sizeof(char) * SIZE);

	int rc = 0;
	int total_len = 0;

	total_len = fread(buffer, sizeof(unsigned char), SIZE, infp);
	printf("Read %s and total_len : %d \n", sourceFilePath, total_len);

	//加密DLL
	size_t len;
	char* decrypt_data = (char*)xxtea_decrypt(buffer, total_len, key, &len);

	printf("Decrypt Successfully and len : %d\n", len);

	//写Dll
	FILE* outfp = 0;
	if (0 != fopen_s(&outfp, destFilePath, "wb+"))
	{
		printf("Open %s Error\n", destFilePath);
		return;
	}

	int rstCount = fwrite(decrypt_data, sizeof(unsigned char), len, outfp);

	fflush(outfp);

	printf("Write len : %d\n", rstCount);

	fclose(infp);
	fclose(outfp);

	free(buffer);
	free(decrypt_data);
}

void Encrypt(const char* sourceFilePath, const char* destFilePath, const char* key)
{
	printf("sourceFile:%s\n", sourceFilePath);
	printf("destFile:%s\n", destFilePath);
	printf("key:%s\n", key);

	FILE* infp = 0;
	if (0 != fopen_s(&infp, sourceFilePath, "rb"))
	{
		printf("Read file Error:%s\n", sourceFilePath);
		return;
	}

	//char buffer[SIZE];
	char* buffer = (char*)malloc(sizeof(char) * SIZE);
	memset(buffer, 0, sizeof(char) * SIZE);

	int rc = 0;
	int total_len = 0;

	total_len = fread(buffer, sizeof(unsigned char), SIZE, infp);
	printf("Read %s and total_len : %d \n", sourceFilePath, total_len);

	//加密DLL
	size_t len;
	char* encrypt_data = (char*)xxtea_encrypt(buffer, total_len, key, &len);

	printf("Encrypt Successfully and len : %d\n", len);

	//写Dll
	FILE* outfp = 0;
	if (0 != fopen_s(&outfp, destFilePath, "wb+"))
	{
		printf("Open %s Error\n", destFilePath);
		return;
	}

	int rstCount = fwrite(encrypt_data, sizeof(unsigned char), len, outfp);

	fflush(outfp);

	printf("Write len : %d\n", rstCount);

	fclose(infp);
	fclose(outfp);

	free(buffer);
	free(encrypt_data);
}

void Usage()
{
	printf("Usage:\n\t Encrypter -encrypt sourceFile destFile key\n\t Encrypter -decrypt sourceFile destFile key");
}

void main(int argc, char* argv[])
{
	if (argc < 5)
	{
		Usage();
		return;
	}

	if (0 == strcmp(argv[1], "-encrypt"))
	{
		Encrypt(argv[2], argv[3], argv[4]);
	}
	else if (0 == strcmp(argv[1], "-decrypt"))
	{
		Decrypt(argv[2], argv[3], argv[4]);
	}
	else
	{
		Usage();
	}
}