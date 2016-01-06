#include "stdlib.h"
#include "stdio.h"
#include "mxio.h"

#include "gomxio.h"

// Init the system so that we can get driver.
int initIF(WORD *wIFCount) {
	int err = MXEIO_Init();
	if ( err != MXIO_OK ){
		return err;
	}
	return MXIO_ListIF( wIFCount );
}

// Init and GetIF info.
char *getIFInfo(WORD *wIFCount, int *err){

	char *IFInfo = NULL;
	*err = initIF( wIFCount );
	if( wIFCount == 0 ){
		return NULL;
	}
	if( *err != MXIO_OK ){
		return NULL;
	}
	IFInfo = malloc( *wIFCount * MX_IF_ONE_IF_SIZE );
	*err = MXIO_GetIFInfo( *wIFCount, IFInfo );
	if( err != MXIO_OK ){
		free(IFInfo);
		IFInfo = NULL;
		return NULL;
	}
	return IFInfo;
}

// Select IF will select the interface that we need.
char *selectIF(DWORD index, int* err){
	WORD wIFCount = 0;
	*err = initIF( &wIFCount );
	if( wIFCount == 0 ){
		return NULL;
	}
	char *IFInfo = malloc( wIFCount * MX_IF_ONE_IF_SIZE );
	*err = MXIO_GetIFInfo( wIFCount, IFInfo );
	if( *err != MXIO_OK ){
		free(IFInfo);
		IFInfo = NULL;
		return NULL;
	}
	*err = MXIO_SelectIF( wIFCount, IFInfo, index );
	if( *err != MXIO_OK ){
		free(IFInfo);
		IFInfo = NULL;
		return NULL;
	}
	return IFInfo;
}

// Auto Search 
MODULE_LIST *autoSearch(int deviceType,int IFidx, char *IFInfo, int *deviceCount, int *err ) {
	// char *IFInfo = NULL;
	MODULE_LIST *ml = (MODULE_LIST*) malloc(sizeof(MODULE_LIST) * 256);

	IFInfo = selectIF(IFidx,err);
	if( *err != MXIO_OK ){
		free(ml);
		return NULL;
	}
	
	*err = MXIO_AutoSearch(deviceType, 3, 5000, deviceCount, (char *)ml);
	if( *err != MXIO_OK){
		free(ml);
		return NULL;
	}
	return ml;
}

