#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pe.h"


/*�ж��Ƿ���PE�ļ�, �������ƣ� ��ʱ��MZ��У��*/
BOOLEAN isPE(DosHeader * dosheader, NtHeader * ntheader)
{
	if (dosheader->signtrue == IMAGE_DOS_SIGNATURE
		&& ntheader->signature == IMAGE_NT_SIGNATURE) //MZ �ߵͣ����ļ������෴
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/*
��ȡPEͷ���ļ�ƫ����
*/
int getNtHeaderoffset(DosHeader *dosheader) {
	return dosheader->e_lfanew;
}

/*��ȡrva���ڽ�*/
ImageSectionHeader* getSectionHeaderByRva(int rva, ImageSectionHeader *imageSectionHeader, int len)
{
	int i = 0;
	for (i = 1; i <= len; i++)
	{
		if (!imageSectionHeader)
		{
			return NULL;
		}
        if (imageSectionHeader->virtualAddress <= rva && 
                rva < imageSectionHeader->virtualAddress + imageSectionHeader->misc.virtualSize){
            return imageSectionHeader;
        }
        imageSectionHeader = imageSectionHeader + 1;
	}
    return NULL;
}


void printImageFileHeaderInfo(NtHeader *ntHeader)
{
	ImageFileHeader * imageFileHeader;
	imageFileHeader = &(ntHeader->imageFileHeader);
	printf("\n");
	printf("PE�ļ�ͷ��Ϣ��\n");
	printf("����CPU:0x%04x\n", imageFileHeader->machine);
	printf("����Ŀ:0x%04x\n", imageFileHeader->numberOfSections);
	printf("����ʱ��:0x%04x\n", imageFileHeader->timeDateStamp);
	printf("��ѡͷ��С0x%04x\n", imageFileHeader->sizeOfOptionalHeader);
	printf("�ļ���Ϣ��־:0x%04x\n", imageFileHeader->characteristics);
}

/*��ȡ��rva*/
/*int getAdjustSectionAlignment(NtHeader *ntHeader, int virtualAddress)
{
    ImageOptionalHeader32 * optionalHeader = &(ntHeader->imageOptionalHeader32);
    int sectionAlignment = optionalHeader->sectionAlignment;
    int fileAlignment = optionalHeader->fileAlignment;
    if (sectionAlignment < 0x1000)
    {
        sectionAlignment = fileAlignment;
    }
    if (virtualAddress && virtualAddress % sectionAlignment)
    {
        return sectionAlignment * ( virtualAddress / sectionAlignment);
    }
    return virtualAddress;
}*/

/*��ȡ�����ļ��е�ƫ����*/
/*int getAdjustFileAlignment(NtHeader *ntHeader, int pointerToRawData)
{
    ImageOptionalHeader32 * optionalHeader = &(ntHeader->imageOptionalHeader32);
    int fileAlignment = optionalHeader->fileAlignment;
    if (fileAlignment < 0x200)
    {
        return pointerToRawData;
    }
    return (pointerToRawData/fileAlignment)*0x200;
}*/

/*rva��ȡ���ڱ���*/
int rva2fileoffset(int rva, ImageSectionHeader *sectionHeader, int sectionLen)
{
    ImageSectionHeader* tmpSectionHeader = getSectionHeaderByRva(rva, sectionHeader, sectionLen);
	if (!tmpSectionHeader)
	{
		return NULL;
	}
	return (tmpSectionHeader->pointerToRawData + (rva - tmpSectionHeader->virtualAddress));
}

/*rva��ȡ��ƫ��*/
int rva2SectionOffset(int rva, ImageSectionHeader *sectionHeader, int sectionLen)
{
	ImageSectionHeader* tmpSectionHeader = getSectionHeaderByRva(rva, sectionHeader, sectionLen);
	if (!tmpSectionHeader)
	{
		return NULL;
	}
	return tmpSectionHeader->pointerToRawData;
}
/*
��ȡ�ж��ٸ��ڱ�
*/
int getNumberOfSections(NtHeader *ntHeader)
{
	ImageFileHeader * imageFileHeader;
	imageFileHeader = &(ntHeader->imageFileHeader);
	return imageFileHeader->numberOfSections;
}

void printNtHeaderInfo(NtHeader *ntHeader)
{
	ImageOptionalHeader32 * optionalHeader;
	optionalHeader = &(ntHeader->imageOptionalHeader32);
	printf("\n");
	printf("��ѡͷ��Ϣ��\n");
	printf("����ִ�����:0x%08x\n", optionalHeader->addressOfEntryPoint);
	printf("����ڵ���ʼ:0x%08x\n", optionalHeader->baseOfCode);
	printf("���ݽڵ���ʼrva:0x%08x\n", optionalHeader->baseOfData);
	printf("���������װ�ص�ַimagebase:0x%08x\n", optionalHeader->imageBase);
	printf("��ʼ��ʱ������ջ�Ĵ�С:0x%08x\n", optionalHeader->sizeOfStackReserve);
	printf("��ʼ��ʱ�����ĶѵĴ�С:0x%08x\n", optionalHeader->sizeOfHeapReserve);
}

/*���ļ��е�ƫ����*/
int getPointerToRawData(ImageSectionHeader *imageSectionHeader)
{
	return imageSectionHeader->pointerToRawData;
}

/*�������Ϣ*/
void printSectionInfo(ImageSectionHeader *imageSectionHeader, int len)
{
	int i = 0;
	printf("\n");
	printf("�ڱ���Ϣ��\n");
	for (i = 1; i <= len; i++)
	{
		printf("��%d����Ϣ:\n", i);
		printf("����:%s\n", imageSectionHeader->name);
		printf("�����������ַ����ʵ����:0x%08x\n", imageSectionHeader->misc.physicalAddress);
		printf("������ RVA ��ַ:0x%08x\n", imageSectionHeader->virtualAddress);
		printf("���ļ��е�ƫ����:0x%08x\n", imageSectionHeader->pointerToRawData);
		printf("������:0x%08x\n", imageSectionHeader->characteristics);
		printf("\n");
		imageSectionHeader = imageSectionHeader + 1;
	}
}

/*��ӡ�������Ϣ*/
void printImportSectionInfo(char * data, ImageImportDirectory *imageImportDirectory, ImageSectionHeader * sectionHeaders, int size, int numberOfSections)
{
	int i = 0;
	printf("size:%d", size);
	int len = size / sizeof(ImageImportDirectory);
	int offset = 0;
	printf("len:%d\n", len);
	printf("�������Ϣ��\n");
	for (i = 0; i < len; i++)
	{
		if (!imageImportDirectory->name)
		{
			return ;
		}
		offset = rva2fileoffset(imageImportDirectory->name, sectionHeaders, numberOfSections);
		if (!offset)
		{
			break;
		}
		printf("��������:%s\n", data + offset);
		//printf("��������:%s\n", imageImportDirectory->name);
		printf("ʱ���:0x%08x\n", imageImportDirectory->timeDateStamp);
		//printf("Data�����rvaֵ:0x%08x\n", imageImportDirectory->dummy.importThunk);
		printf("Data�����rvaֵ:0x%08x\n", imageImportDirectory->firstThunk);
		printf("������������:0x%08x\n", imageImportDirectory->forwarderChain);
		printf("\n");
		imageImportDirectory = imageImportDirectory + 1;
	}
}

/*��ȡIAT��rva*/
ImageThunkData32 * getIATrva(char *data, ImageImportDirectory *imageImportDirectory, ImageSectionHeader * sectionHeader, int sectionLen)
{
	ImageThunkData32 *imageThunkData32 = NULL;
	//printf("0x%x,0x%x,", imageImportDirectory->dummy.originalFirstThunk, imageImportDirectory->firstThunk);
	imageThunkData32 = (ImageThunkData32 *)(data + rva2fileoffset(imageImportDirectory->dummy.originalFirstThunk, sectionHeader, sectionLen));
	return imageThunkData32;
}

/*���IAT��Ϣ*/
void printIATInfo(char *data, ImageThunkData32 * imageThunkData32, ImageSectionHeader * sectionHeader, int sectionLen)
{
	int offset = 0;
	ImageImportByName *imageImportByName = NULL;
	while (imageThunkData32->u1.addressOfData)
	{
		int offset = rva2fileoffset(imageThunkData32->u1.addressOfData, sectionHeader, sectionLen);
		imageImportByName = (ImageImportByName *)(data + offset);
		printf("������:%s\n", imageImportByName->name);
		imageThunkData32 = imageThunkData32 + 1;
	}
}

/*������еĵ��뺯����Ϣ*/
void printImportInfo(char *data, ImageImportDirectory *imageImportDirectory, ImageSectionHeader * sectionHeader, int sectionLen)
{
	ImageThunkData32 *imageThunkData32 = getIATrva(data, imageImportDirectory, sectionHeader, sectionLen);
	printf("\n���뺯���б�:\n");
	printIATInfo(data, imageThunkData32, sectionHeader, sectionLen);
}

/*���������б�*/
void printExportFunctionInfo(char * data, ImageExportDirectory *imageExportDirectory, ImageSectionHeader * sectionHeaders, ImageSectionHeader * headerSectionHeaders,int numberOfSections)
{
	int i = 0;
	int offset = 0;
	int *nameAddr = NULL;
	int *funcAddr = NULL;
	if (imageExportDirectory->name == NULL)
	{
		return;
	}
	offset = rva2fileoffset(imageExportDirectory->name, sectionHeaders, numberOfSections);
	if (!offset)
	{
		return;
	}
	funcAddr = imageExportDirectory->addressOfFunctions;
	for (i = 1; i <= imageExportDirectory->numberOfFunctions; i++)
	{
		printf("�������%d��rva:0x%08x\n", i, funcAddr);
		int offset = rva2fileoffset(funcAddr, sectionHeaders, numberOfSections);
		int *func_rva = (int *)(data + offset);
		int rav_offset = rva2fileoffset(*func_rva, headerSectionHeaders, numberOfSections);
		//int section_offse = rva2SectionOffset(*func_rva, headerSectionHeaders, numberOfSections);
		printf("����ƫ����:0x%08x\n", rav_offset);
		printf("����%d�ļ�ƫ������0x%08x\n", i, offset);
		funcAddr = funcAddr + 1;
	}
	nameAddr = (int *)(data + rva2fileoffset(imageExportDirectory->addressOfNames, sectionHeaders, numberOfSections));
	for (i = 0; i < imageExportDirectory->numberOfNames; i++)
	{
		char *name = (data + rva2fileoffset((*nameAddr), sectionHeaders, numberOfSections));
		printf("��������%s\n", name);
		nameAddr = nameAddr + 1;
	}
	printf("\n");
}

/*��ӡ��������Ϣ*/
void printExportSectionInfo(char * data, ImageExportDirectory *imageExportDirectory, ImageSectionHeader * sectionHeaders, ImageSectionHeader * headerSectionHeaders, int size, int numberOfSections)
{
	int i = 0;
	printf("size:%d", size);
	int len = size / sizeof(ImageExportDirectory);
	int offset = 0;
	printf("len:%d\n", len);
	printf("��������Ϣ��\n");
	for (i = 0; i < len; i++)
	{
		if (imageExportDirectory->name == NULL)
		{
			break;
		}
		offset = rva2fileoffset(imageExportDirectory->name, sectionHeaders, numberOfSections);
		if (!offset)
		{
			break;
		}
		printf("��������:%s\n", data + offset);
		printf("ʱ���:0x%08x\n", imageExportDirectory->timeDateStamp);
		printf("����������:0x%08x\n", imageExportDirectory->numberOfFunctions);
		printf("�����ֵ�������������:0x%08x\n", imageExportDirectory->numberOfNames);
		printf("�����������ļ�ƫ����:0x%08x\n", imageExportDirectory->addressOfFunctions);
		printf("�����ֵ����������ļ�ƫ����:0x%08x\n", imageExportDirectory->addressOfNames);
		printExportFunctionInfo(data, imageExportDirectory, sectionHeaders, headerSectionHeaders, numberOfSections);
		printf("���뺯����ŵ��ļ�ƫ����������):0x%08x\n", imageExportDirectory->addressOfNameOrdinals);
		printf("\n");
		imageExportDirectory = imageExportDirectory + 1;
	}
}

/*��ӡÿһ��*/
void printline(char * hex, int len)
{
	int i = 0;
	if (len == 0 || hex == NULL || len > 0x10)
		return;
	for (i = 0; i < len; i++)
	{
		// ��ӡÿ���ֽ�
		printf("%02x ", *(hex + i));
	}
	printf("\n");	//ÿ�д�ӡ��س�
}


/*��ӡ*/
// ��ӡһ���ڴ�Ķ�����
void printhex(char * hex, int len)
{
	int i;
	int lastnum = len & 0xF;	//�������һ���ж��ٸ��ֽڣ�����պ���������Ϊ0
	int line = len >> 4;		//�����ж����У��������һ�в��������
	for (i = 0; i < line; i++)
	{
		// ��ӡÿһ�У�ÿ��16���ֽ�
		printline(hex + i * 0x10, 0x10);
	}
	if (lastnum)
	{
		// ���һ�в���16�ֽ�
		if (line) {

			printline(hex + lastnum * 0x10, lastnum);
		}
		else {  // ���ֻ��һ��
			printline(hex, lastnum);
		}
	}

}

/*��ȡ�ļ�����*/
int getFileLen(FILE *fp)
{
	int filesize = 0;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	return filesize;
}

/*������ת��Ϊ�Ƿ�*/
char * boolean2string(int bool)
{
	if (bool)
	{
		return "��";
	}
	else
	{
		return "��";
	}

}
/*��ȡ���е����� �޸��� ��ʹ��fgets��ʹ��fread*/
char* getFileData(FILE *fp, char *str, int filesize)
{
	size_t len;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	memset(str, 0, filesize + 1);
	rewind(fp);
	len = fread(str, 1, filesize, fp);

	if (len != filesize)
	{
		return NULL;
	}

	return str;
}

/*������*/
void main(int argc, char **argv)
{
	// 1.��ȡ�ļ�
	// 2.����dosͷ
	// 3.����option header
	// 4.����section table
	char * file_name = argv[1];
	FILE *fp = NULL;
	DosHeader *dosHeader = NULL; //dosͷ
	NtHeader *ntHeader = NULL;  // ntͷ
	//ImageDataDirectory *dataDirectory = NULL;
	ImageSectionHeader *imageSectionHeader = NULL;
    ImageSectionHeader *importSectionHeader = NULL;
    ImageSectionHeader *exportSectionHeader = NULL;
	ImageDataDirectory *importDataDirectory = NULL;
	ImageDataDirectory *exportDataDirectory = NULL;
	ImageImportDirectory *imageImportDirectory = NULL;
	ImageExportDirectory *imageExportDirectory = NULL;
	int numberOfSections = 0;
	int filesize = 0;  //�ļ���С
	int ntoffset = 0;  //ntͷ�ı�����
	char *data = NULL;  //����ָ������
	// ��ȡ����
	if (!file_name)
	{
		printf("�봫������ļ���");
		goto end;
	}
	if ((fp = fopen(file_name, "rb")) == NULL)
	{
		printf("���ܴ��ļ�");
		exit(1);
	}
	filesize = getFileLen(fp);
	data = (char *)malloc(filesize + 1);
	if (data == NULL)
	{
		printf("�����ڴ�ʧ��");
		goto end;
	}
	getFileData(fp, data, filesize);
	// ����dosheader
	dosHeader = (DosHeader *)data;
	ntoffset = getNtHeaderoffset(dosHeader);
	// printf("PEͷλ��:0x%02x\n", getNtHeaderoffset(dosheader));
	// ntͷ��Ϣ
	ntHeader = (NtHeader *)(data + ntoffset);
	printImageFileHeaderInfo(ntHeader);
	printNtHeaderInfo(ntHeader);
	exportDataDirectory = &(ntHeader->imageOptionalHeader32.dataDirectory[0]); // ��������Ϣ
	importDataDirectory = &(ntHeader->imageOptionalHeader32.dataDirectory[1]); // �������Ϣ

	imageSectionHeader = (ImageSectionHeader *)(data + ntoffset + sizeof(NtHeader));
	printSectionInfo(imageSectionHeader, getNumberOfSections(ntHeader));
	numberOfSections = getNumberOfSections(ntHeader);
    exportSectionHeader = getSectionHeaderByRva(exportDataDirectory->virtualAddress, imageSectionHeader, numberOfSections); // ���������ڽ�
	importSectionHeader = getSectionHeaderByRva(importDataDirectory->virtualAddress, imageSectionHeader, numberOfSections); // ���������ڽ�
	imageExportDirectory = (ImageExportDirectory *)(data + rva2fileoffset(exportDataDirectory->virtualAddress, exportSectionHeader, numberOfSections));
	imageImportDirectory = (ImageImportDirectory *)(data + rva2fileoffset(importDataDirectory->virtualAddress, importSectionHeader, numberOfSections));
	printExportSectionInfo(data, imageExportDirectory, exportSectionHeader, imageSectionHeader, exportDataDirectory->size, numberOfSections);
	printImportSectionInfo(data, imageImportDirectory, importSectionHeader, importDataDirectory->size, numberOfSections);

	//rva2fileoffset(*func_rva, imageSectionHeader, numberOfSections)
	printImportInfo(data, imageImportDirectory, imageSectionHeader, numberOfSections);
	getchar();
end:
	if (data != NULL)
	{
		free(data);
	}
	if (fp != NULL)
	{
		fclose(fp);
	}
}
