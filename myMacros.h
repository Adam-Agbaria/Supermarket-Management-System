#pragma once

//#define DETAIL_PRINT

typedef  unsigned char BYTE;

#define ERROR 0


#define CHECK_RETRUN_0(value) if(value==NULL){return 0;}
#define CHECK_MSG_RETURN_0(value,msg) if(value==NULL){puts(msg); return 0;}

#define FREE_CLOSE_FILE_RETURN_0(ptr,fp) {free(ptr),fclose(fp);return 0;}
#define CLOSE_RETURN_0(fp) {fclose(fp);return 0;}
#define CHECK_NULL_CLOSE_FILE_RETURN_0(ptr, fp) if(ptr==NULL){fclose(fp);return 0;}