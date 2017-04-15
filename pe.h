//#define NULL 0
#define BUF_SIZE 0x100
#define BYTE char
#define WORD short
#define DWORD int
#define BOOLEAN int
#define TRUE 1
#define FALSE 0
#define IMAGE_SIZEOF_SHORT_NAME 8
#define IMAGE_DOS_SIGNATURE 0x5A4D
#define IMAGE_NT_SIGNATURE  0x00004550

typedef struct _DOS_HEADER {
	WORD signtrue;
	WORD e_cblp;
	WORD e_cp;
	WORD e_crlc;
	WORD e_cparhdr;
	WORD e_minalloc;
	WORD e_maxalloc;
	WORD e_ss;
	WORD e_sp;
	WORD e_csum;
	WORD e_ip;
	WORD e_cs;
	WORD e_lfarlc;
	WORD e_ovno;
	WORD e_res[4];
	WORD e_oemid;
	WORD e_oeminfo;
	WORD e_res2[10];
	DWORD e_lfanew;
} DosHeader;

typedef struct _IMAGE_FILE_HEADER {
	WORD    machine; // ����ƽ̨
	WORD    numberOfSections; // �ļ���������Ŀ
	DWORD   timeDateStamp; // �ļ��������ں�ʱ��  
	DWORD   pointerToSymbolTable;  // ָ����ű�(��Ҫ���ڵ���)
	DWORD   numberOfSymbols;  // ���ű��з��Ÿ���(ͬ��)  
	WORD    sizeOfOptionalHeader;   // IMAGE_OPTIONAL_HEADER32 �ṹ��С  
	WORD    characteristics;  // �ļ����� 
} ImageFileHeader, *PImageFileHeader;

typedef struct _IMAGE_DATA_DIRECTORY {
	DWORD   virtualAddress; //���ݵ���ʼRVA
	DWORD   size;  //���ݿ�ĳ���
} ImageDataDirectory, *PImageDataDirectory;

typedef struct _IMAGE_OPTIONAL_HEADER {
	WORD    magic;  // 10B 32λPE 20B 64λPE 107 ROMӳ��
	BYTE    majorLinkerVersion;  // �������汾��
	BYTE    minorLinkerVersion;  // ���������汾��
	DWORD   sizeOfCode;  // ���д���ڵ��ܺ�  �ô�С�ǻ����ļ������Ĵ�С
	DWORD   sizeOfInitializedData;  // ���к��ѳ�ʼ�����ݵĽڵ��ܴ�С
	DWORD   sizeOfUninitializedData;  // ���к�δ��ʼ�����ݵĽڵĴ�С
	DWORD   addressOfEntryPoint;  // ����ִ�����RVA
	DWORD   baseOfCode;  // ����ڵ���ʼRVA
	DWORD   baseOfData;  // ���ݽڵ���ʼRVA
	DWORD   imageBase;  // ���������װ�ص�ַ
	DWORD   sectionAlignment; // �ڴ��нڵĶ�������
	DWORD   fileAlignment;  // �ļ��нڵĶ�������
	WORD    majorOperatingSystemVersion; // ����ϵͳ���汾��
	WORD    minorOperatingSystemVersion;  // ����ϵͳ���汾��
	WORD    majorImageVersion;  // PE�ļ�ӳ��İ汾��
	WORD    minorImageVersion;
	WORD    majorSubsystemVersion;  // ��ϵͳ�İ汾��
	WORD    minorSubsystemVersion;
	DWORD   win32VersionValue;  // δ�� ��������0
	DWORD   sizeOfImage;  // �ڴ�������PE�ļ���ӳ��ߴ�
	DWORD   sizeOfHeaders; // ���нڱ����ļ��������Ⱥ�Ĵ�С
	DWORD   checkSum; // У���
	WORD    subsystem; // ָ��ʹ�ý������ϵͳ
	WORD    dllCharacteristics; // DLL�ļ�����
	DWORD   sizeOfStackReserve; // ��ʼ��ʱ������ջ�Ĵ�С
	DWORD   sizeOfStackCommit; // ��ʼ��ʱʵ���ύ��ջ�Ĵ�С
	DWORD   sizeOfHeapReserve; // ��ʼ��ʱ�����ĶѵĴ�С
	DWORD   sizeOfHeapCommit; // ��ʼ��ʱʵ���ύ�ĶѵĴ�С
	DWORD   loaderFlags; // ���ر�־  δ��
	DWORD   numberOfRvaAndSizes; // ���������Ŀ¼�ṹ������
	ImageDataDirectory dataDirectory[16];
} ImageOptionalHeader32, *PImageOptionalHeader32;

typedef struct _NT_HEADER
{
	DWORD signature;
	ImageFileHeader imageFileHeader;
	ImageOptionalHeader32 imageOptionalHeader32;
} NtHeader, *PNtHeader;

typedef struct _IMAGE_SECTION_HEADER
{
	BYTE name[IMAGE_SIZEOF_SHORT_NAME];
	union
	{
		DWORD physicalAddress; //�����ַ
		DWORD virtualSize; //��ʵ����
	} misc;
	DWORD virtualAddress; // ������ RVA ��ַ
	DWORD sizeOfRawData; // ���ļ��ж����ĳߴ�
	DWORD pointerToRawData; // ���ļ��е�ƫ����
	DWORD pointerToRelocations; // ��OBJ�ļ���ʹ�ã��ض�λ��ƫ��
	DWORD pointerToLinenumbers; // �кű��ƫ�ƣ�������ʹ�õأ�
	WORD numberOfRelocations; // ��OBJ�ļ���ʹ�ã��ض�λ����Ŀ
	WORD numberOfLinenumbers; // �кű����кŵ���Ŀ
	DWORD characteristics; // ��������ɶ�����д����ִ�е�
} ImageSectionHeader, *PImageSectionHeader;

typedef struct _IMAGE_EXPORT_DIRECTORY {
	DWORD   characteristics;
	DWORD   timeDateStamp;
	WORD    majorVersion;
	WORD    minorVersion;
	DWORD   name;
	DWORD   base;
	DWORD   numberOfFunctions;
	DWORD   numberOfNames;
	DWORD   addressOfFunctions;
	DWORD   addressOfNames;
	DWORD   addressOfNameOrdinals;
} ImageExportDirectory, *PImageExportDirectory;

typedef struct _IMAGE_IMPORT_DIRECTORY {
	union {
		DWORD   importThunk;
		DWORD   originalFirstThunk;
	} dummy;
	DWORD   timeDateStamp;
	DWORD   forwarderChain;
	DWORD   name;   //����
	DWORD   firstThunk;
} ImageImportDirectory, *PImageImportDirectory;


typedef struct _IMAGE_THUNK_DATA32 {
	union {
		DWORD forwarderString;      // PBYTE
		DWORD function;             // PDWORD
		DWORD ordinal;
		DWORD addressOfData;        // PIMAGE_IMPORT_BY_NAME
	} u1;
} ImageThunkData32, *PImageThunkData32;

typedef struct _IMAGE_IMPORT_BY_NAME {
	WORD    hint;
	BYTE    name[1];
} ImageImportByName, *PimageImportByName;

/*
typedef struct _PE
{
DosHeader dosHeader;
NtHeader ntHeader;
} PE,  *PPE;
*/
